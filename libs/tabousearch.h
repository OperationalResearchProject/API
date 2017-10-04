#ifndef OPERATIONALRESEARCHAPI_TABOUSEARCH_H
#define OPERATIONALRESEARCHAPI_TABOUSEARCH_H

#include "helper.h"
#include "../grpcCallData/TabuSearch/Neighbor.h"
#include <random>

void addMoveInTabuList(mongocxx::collection tabu_coll, mongocxx::collection neighbor_coll, std::string solution, int iteration, bsoncxx::oid transaction_id);
bool moveIsAllowed(mongocxx::collection tabu_coll, int i, int j, bsoncxx::oid transaction_id, int iteration);
std::vector<Neighbor> getAllNeighbors(std::string solution, bsoncxx::oid  transaction_id, mongocxx::collection neighbor_coll, mongocxx::collection transac_coll, mongocxx::collection tabu_coll);
void cleanAllNeighbors(mongocxx::collection neighbor_coll, bsoncxx::oid  transaction_id);

#endif //OPERATIONALRESEARCHAPI_TABOUSEARCH_H
