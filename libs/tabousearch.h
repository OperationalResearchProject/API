#ifndef OPERATIONALRESEARCHAPI_TABOUSEARCH_H
#define OPERATIONALRESEARCHAPI_TABOUSEARCH_H

#include "helper.h"
#include "../grpcCallData/TabuSearch/Neighbor.h"
#include <random>

void addMoveInTabuList(mongocxx::collection tabu_coll, mongocxx::collection neighbor_coll, std::string solution, int iteration, bsoncxx::oid transaction_id);
bool moveIsAllowed(mongocxx::collection tabu_coll, int i, int j, bsoncxx::oid transaction_id, int iteration);
std::vector<Neighbor> getAllNeighbors(const std::string & solution, bsoncxx::oid  transaction_id, mongocxx::collection neighbor_coll, mongocxx::collection transac_coll, mongocxx::collection tabu_coll);
void cleanAllNeighbors(mongocxx::collection neighbor_coll, bsoncxx::oid  transaction_id); // todo : call at the end of the transaction ?
Neighbor getMotherSolution(mongocxx::collection neighbor_coll, const std::string & neighbor_solution, const int & iteration, bsoncxx::oid  transaction_id);
Neighbor getNeighbor(mongocxx::collection neighbor_coll, const int & move_i, const int & move_j, const int & iteration, bsoncxx::oid  transaction_id);

#endif //OPERATIONALRESEARCHAPI_TABOUSEARCH_H
