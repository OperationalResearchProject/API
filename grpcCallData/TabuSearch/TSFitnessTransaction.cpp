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

    mongocxx::stdx::optional<bsoncxx::document::value> view = transac_coll.find_one(filterTransactionToSearch.view());

    if(view) {
        // get the best fitness of the transaction
        bsoncxx::document::view viewFitnessToSearch = *view;
        bsoncxx::document::element eltBestFitnessId = viewFitnessToSearch["best_fitness_id"];

        auto filterFitnessToSearch = bsoncxx::builder::stream::document{};
        filterFitnessToSearch << "_id" << eltBestFitnessId.get_oid();

        auto docFitnessFindOne = fitness_coll.find_one(filterFitnessToSearch.view());
        auto bestFitness = docFitnessFindOne.value().view()["fitness"].get_double().value;


        // If the new fitness is better (Case of minimization) than the actual best fitness, then we replace the solution
        if (bestFitness > bestFitnessNeighbor) {

            bsoncxx::builder::stream::document documentFitnessToInsert{};
            documentFitnessToInsert << "transaction_id" << request_.id();
            documentFitnessToInsert << "solution" << request_.solutions(bestINeighbor);
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

    addMoveInTabuList(tabu_list_coll, neighbor_coll, request_.solutions(bestINeighbor), getIteration(transac_coll, bsoncxx::oid(request_.id())) - 1, bsoncxx::oid(request_.id()));
    std::cout << "best fitness local  : " << bestFitnessNeighbor << std::endl;
    std::cout << "best solution local : " << request_.solutions(bestINeighbor) << std::endl;

    /*
     * set the response with neighbors of the best current Neighbors
     */
    reply_.set_id(request_.id());

    std::vector<Neighbor> neighborsOfBest = getAllNeighbors(request_.solutions(bestINeighbor), bsoncxx::oid(request_.id()), neighbor_coll, transac_coll, tabu_list_coll);

    for (Neighbor neighbor:neighborsOfBest){
        reply_.add_solutions(neighbor.solution());
    }


    responder_.Finish(reply_, Status::OK, this);
}