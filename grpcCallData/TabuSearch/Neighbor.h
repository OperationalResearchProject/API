
#ifndef OPERATIONALRESEARCHAPI_NEIGHBOR_H
#define OPERATIONALRESEARCHAPI_NEIGHBOR_H

#include <iostream>
#include <string>
#include <mongocxx/collection.hpp>
#include "../../libs/helper.h"


class Neighbor {

public:
    Neighbor(bsoncxx::oid  transaction_id, std::string solution, int move_i, int move_j, int move_mother_i, int move_mother_j);
    Neighbor(int move_i, int move_j);
    const std::string solution(){ return solution_;};
    const int getI(){ return moveI;};
    const int getJ(){ return moveJ;};
    const int getMotherI(){ return moveMotherI;};
    const int getMotherJ(){ return moveMotherJ;};
    void save(mongocxx::collection neighbor_collection, mongocxx::collection transac_collection);
private:
    std::string solution_;
    int moveI;
    int moveJ;
    int moveMotherI;
    int moveMotherJ;
    bsoncxx::oid  transaction_id_;

};


#endif //OPERATIONALRESEARCHAPI_NEIGHBOR_H
