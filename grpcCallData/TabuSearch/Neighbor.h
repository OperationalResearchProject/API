
#ifndef OPERATIONALRESEARCHAPI_NEIGHBOR_H
#define OPERATIONALRESEARCHAPI_NEIGHBOR_H

#include <iostream>
#include <string>
#include <mongocxx/collection.hpp>
#include "../../libs/helper.h"


class Neighbor {

public:
    Neighbor(mongocxx::collection neighbor_collection, mongocxx::collection transac_collection, bsoncxx::oid  transaction_id, std::string solution, int move_i, int move_j);

    const std::string solution(){ return solution_;};
private:
    std::string solution_;
    int moveI;
    int moveJ;

};


#endif //OPERATIONALRESEARCHAPI_NEIGHBOR_H
