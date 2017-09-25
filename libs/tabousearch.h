#ifndef OPERATIONALRESEARCHAPI_TABOUSEARCH_H
#define OPERATIONALRESEARCHAPI_TABOUSEARCH_H

#include "helper.h"
#include "../grpcCallData/TabuSearch/Neighbor.h"

void addMoveInTabuList();
bool moveIsAllowed();
std::vector<Neighbor> getAllNeighbors(std::string solution);

#endif //OPERATIONALRESEARCHAPI_TABOUSEARCH_H
