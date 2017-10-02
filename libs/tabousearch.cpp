#include "tabousearch.h"


void addMoveInTabuList(mongocxx::collection tabu_coll, mongocxx::collection neighbor_coll, const std::string solution, const int iteration, const bsoncxx::oid transaction_id){

    auto filter = bsoncxx::builder::stream::document{};
    filter << "transaction_id" << transaction_id;
    filter << "solution" << solution;
    filter << "iteration" << iteration;

    auto docFind = neighbor_coll.find_one(filter.view());

    int move_i = docFind.value().view()["move_i"].get_int32();
    int move_j = docFind.value().view()["move_j"].get_int32();

    bsoncxx::builder::stream::document documentTabu{};
    documentTabu << "transaction_id" << transaction_id;
    documentTabu << "move_i" << move_i;
    documentTabu << "move_j" << move_j;
    documentTabu << "time" << 30;

    tabu_coll.insert_one(documentTabu.view());

    bsoncxx::builder::stream::document documentTabuInversed{};
    documentTabuInversed << "transaction_id" << transaction_id;
    documentTabuInversed << "move_i" << move_j;
    documentTabuInversed << "move_j" << move_i;
    documentTabuInversed << "time" << 30;

    tabu_coll.insert_one(documentTabuInversed.view());
}

bool moveIsAllowed(mongocxx::collection tabu_coll, int i, int j, const bsoncxx::oid transaction_id){
    auto filter = bsoncxx::builder::stream::document{};
    filter << "move_i" << i;
    filter << "move_j" << j;
    filter << "transaction_id" << transaction_id;


    return !tabu_coll.find_one(filter.view());
}

std::vector<Neighbor> getAllNeighbors(std::string solution, bsoncxx::oid  transaction_id, mongocxx::collection neighbor_coll, mongocxx::collection transac_coll, mongocxx::collection tabu_coll){
    /**
     * Example : s = 1-2-3-4
     * N(s) with i=0 = {2-1-3-4; 3-1-2-4; 4-2-3-1}
     * N(s) with i=1 = {2-1-3-4; 1-3-2-4; 1-4-3-2}
     * N(s) with i=2 = {3-2-1-4; 1-3-2-4; 1-2-4-3}
     * N(s) with i=3 = {4-2-3-1; 1-4-3-2; 1-2-4-3}
     */

    std::vector<std::string> vSolution{explode(solution, '-')};
    std::vector<Neighbor> vAllNeighbors;

    for (int i=0; i < vSolution.size(); i++) {
        for (int j=0; j < vSolution.size(); j++) {
            if (i != j && moveIsAllowed(tabu_coll,i,j, transaction_id)){
                std::cout << "move "<< i <<"-"<<j <<"is allowed : "<< moveIsAllowed(tabu_coll,i,j, transaction_id) << std::endl;
                std::vector<std::string> vNeighbor = vSolution;
                std::string tmp = vNeighbor[i];
                vNeighbor[i] = vNeighbor[j];
                vNeighbor[j] = tmp;
                vAllNeighbors.push_back(Neighbor(neighbor_coll, transac_coll, transaction_id, vectorToString(vNeighbor), i, j));
            }
        }
    }

    return vAllNeighbors;
}

void cleanAllNeighbors(mongocxx::collection neighbor_coll, bsoncxx::oid  transaction_id){
    bsoncxx::stdx::optional<mongocxx::result::delete_result> result =
            neighbor_coll.delete_many(
                    bsoncxx::builder::stream::document{} << "transaction_id"
                                                         << transaction_id
                                                         << bsoncxx::builder::stream::finalize);

    /*if(result) {
        std::cout << result->deleted_count() << "\n";
    }*/
}