#include "TSFitnessTransaction.h"
#include "../../libs/tabousearch.h"

TSFitnessTransaction::TSFitnessTransaction(ts::TabouSearchService::AsyncService *service, ServerCompletionQueue *cq,  mongocxx::database db) :
        TSBase(service, cq, db), responder_(&ctx_) {
    service_->RequestSendFitness(&ctx_, &request_, &responder_, cq_, cq_, this);
}

void TSFitnessTransaction::Process() {
    new TSFitnessTransaction(service_, cq_, db_);

    // Test if solutions_size == fitnesses_size
    if(request_.solutions_size() != request_.fitnesses_size()){
        Status status_perso = Status(StatusCode::INVALID_ARGUMENT, "The number of solutions and the number of fitness have to be equals");
        responder_.Finish(reply_, status_perso, this);
        return;
    }

    // increment the transaction
    transac_coll.update_one(
            bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid(request_.id())<< bsoncxx::builder::stream::finalize,
            bsoncxx::builder::stream::document{} << "$inc" <<
                                                           bsoncxx::builder::stream::open_document
                                                                 <<"iteration" << 1
                                                           <<bsoncxx::builder::stream::close_document
                                                 << bsoncxx::builder::stream::finalize
    );

    int current_iteration =  getIteration(transac_coll, bsoncxx::oid(request_.id()));
    std::cout <<"Iteration n° "<<current_iteration<< std::endl;

    // Find the best neighbor for this iteration
    int bestINeighbor = 0;
    double bestFitnessNeighbor = request_.fitnesses(0);
    for (int i = 0; i < request_.solutions_size(); ++i) {
        if(request_.fitnesses(i) < bestFitnessNeighbor){
            bestFitnessNeighbor = request_.fitnesses(i);
            bestINeighbor = i;
        }
    }

    // if it is the best solution for the current transaction, replace it in database
    auto filterTransactionToSearch = bsoncxx::builder::stream::document{} ;
    filterTransactionToSearch << "_id" << bsoncxx::oid(request_.id());

    mongocxx::stdx::optional<bsoncxx::document::value> viewTransaction = transac_coll.find_one(filterTransactionToSearch.view());

    Neighbor bestLocal = getNeighbor(neighbor_coll,request_.solutions(bestINeighbor).i(), request_.solutions(bestINeighbor).j(), current_iteration - 1, bsoncxx::oid(request_.id()));

    if(viewTransaction) {
        // get the best fitness of the transaction
        bsoncxx::document::view viewFitnessToSearch = *viewTransaction;
        bsoncxx::document::element eltBestFitnessId = viewFitnessToSearch["best_fitness_id"];

        auto filterFitnessToSearch = bsoncxx::builder::stream::document{};
        filterFitnessToSearch << "_id" << eltBestFitnessId.get_oid();

        auto docFitnessFindOne = fitness_coll.find_one(filterFitnessToSearch.view());
        auto bestFitness = docFitnessFindOne.value().view()["fitness"].get_double().value;


        // If the new fitness is better (Case of minimization) than the actual best fitness, then we replace the solution
        //std::cout << "best fitness global  : " << bestFitness << std::endl;
        if (bestFitness > bestFitnessNeighbor) {

            bsoncxx::builder::stream::document documentFitnessToInsert{};
            documentFitnessToInsert << "transaction_id" << request_.id();
            documentFitnessToInsert << "solution" << bestLocal.solution();
            documentFitnessToInsert << "fitness" << bestFitnessNeighbor;
            documentFitnessToInsert << "params" << "";


            bsoncxx::types::value newBestFitnessId = fitness_coll.insert_one(
                    documentFitnessToInsert.view())->inserted_id();

            transac_coll.update_one(
                    bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid(request_.id())
                                                         << bsoncxx::builder::stream::finalize,
                    bsoncxx::builder::stream::document{} << "$set" << bsoncxx::builder::stream::open_document <<
                                                         "best_fitness_id" << newBestFitnessId.get_oid() <<
                                                         bsoncxx::builder::stream::close_document
                                                         << bsoncxx::builder::stream::finalize
            );

        }
    }


    addMoveInTabuList(tabu_list_coll, neighbor_coll, bestLocal.solution(), getIteration(transac_coll, bsoncxx::oid(request_.id())) - 1, bsoncxx::oid(request_.id()));

    Neighbor motherOfTheBest = getMotherSolution(neighbor_coll,bestLocal.solution(), current_iteration - 1, bsoncxx::oid(request_.id()));
//    std::cout << "best mother move : " << motherOfTheBest.getI() <<" / "<< motherOfTheBest.getJ() <<std::endl;
//    std::cout << "best fitness local  : " << bestFitnessNeighbor << std::endl;
//    std::cout << "best solution local : " << bestLocal.solution() << std::endl<< std::endl;


    /*
     * set the response with neighbors of the best current Neighbors
     */
    reply_.set_id(request_.id());

    std::vector<Neighbor> neighborsOfBestLocal = getAllNeighbors(bestLocal.solution(), bsoncxx::oid(request_.id()), neighbor_coll, transac_coll, tabu_list_coll);

    for (Neighbor neighbor:neighborsOfBestLocal){

        Solution* s = reply_.add_solutions();

        // the best local solution will be the future mother
        s->set_mother_solution(bestLocal.solution());
        s->set_mother_fitness(request_.fitnesses(bestINeighbor));
        s->set_mother_i(neighbor.getMotherI());
        s->set_mother_j(neighbor.getMotherJ());

        s->set_i(neighbor.getI());
        s->set_j(neighbor.getJ());

    }


    responder_.Finish(reply_, Status::OK, this);
}