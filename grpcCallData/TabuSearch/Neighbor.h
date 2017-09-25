
#ifndef OPERATIONALRESEARCHAPI_NEIGHBOR_H
#define OPERATIONALRESEARCHAPI_NEIGHBOR_H

#include <iostream>
#include <string>


class Neighbor {

public:
    Neighbor(const std::string & solution, int move_i, int move_j)
        :solution_(solution), moveI(move_i), moveJ(move_j){};

    const std::string solution(){ return solution_;};
private:
    std::string solution_;
    int moveI;
    int moveJ;

};


#endif //OPERATIONALRESEARCHAPI_NEIGHBOR_H
