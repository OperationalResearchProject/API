#ifndef OPERATIONALRESEARCHAPI_TABOUSEARCH_H
#define OPERATIONALRESEARCHAPI_TABOUSEARCH_H

#include "helper.h"
#include "../grpcCallData/TabuSearch/Neighbor.h"

void addMoveInTabuList(mongocxx::collection tabu_coll, std::string solution);
bool moveIsAllowed();
std::vector<Neighbor> getAllNeighbors(std::string solution, bsoncxx::oid  transaction_id, mongocxx::collection neighbor_coll, mongocxx::collection transac_coll);
void cleanAllNeighbors(mongocxx::collection neighbor_coll, bsoncxx::oid  transaction_id);

#endif //OPERATIONALRESEARCHAPI_TABOUSEARCH_H
