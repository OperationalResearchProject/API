#include "Neighbor.h"

Neighbor::Neighbor(bsoncxx::oid  transaction_id, std::string solution, int move_i, int move_j, int move_mother_i, int move_mother_j)
:solution_(solution), transaction_id_(transaction_id), moveI(move_i), moveJ(move_j), moveMotherI(move_mother_i), moveMotherJ(move_mother_j){}

void Neighbor::save(mongocxx::collection neighbor_collection, mongocxx::collection transac_collection){
    bsoncxx::builder::stream::document documentNeighbor{};
    documentNeighbor << "transaction_id" << transaction_id_;
    documentNeighbor << "iteration" << getIteration(transac_collection, transaction_id_); // todo : why not passed a transaction ?
    documentNeighbor << "solution" << solution_;
    documentNeighbor << "move_i" << moveI;
    documentNeighbor << "move_j" << moveJ;
    documentNeighbor << "move_mother_i" << moveMotherI;
    documentNeighbor << "move_mother_j" << moveMotherJ;

    neighbor_collection.insert_one(documentNeighbor.view());
}