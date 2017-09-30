#include "Neighbor.h"

Neighbor::Neighbor(mongocxx::collection neighbor_collection, mongocxx::collection transac_collection, bsoncxx::oid  transaction_id, std::string solution, int move_i, int move_j)
:solution_(solution), moveI(move_i), moveJ(move_j){

    bsoncxx::builder::stream::document documentNeighbor{};
    documentNeighbor << "transaction_id" << transaction_id;
    documentNeighbor << "iteration" << getIteration(transac_collection, transaction_id); // todo : why not passed a transaction ?
    documentNeighbor << "solution" << solution;
    documentNeighbor << "move_i" << move_i;
    documentNeighbor << "move_j" << move_j;

    neighbor_collection.insert_one(documentNeighbor.view());
}