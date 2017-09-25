#include "tabousearch.h"


void addMoveInTabuList(){
    // TODO
}

bool moveIsAllowed(){
    // TODO
}

std::vector<Neighbor> getAllNeighbors(std::string solution){
    /**
     * Example : s = 1-2-3-4
     * N(s) with i=0 = {2-1-3-4; 3-1-2-4; 4-2-3-1}
     * N(s) with i=1 = {2-1-3-4; 1-3-2-4; 1-4-3-2}
     * N(s) with i=2 = {3-2-1-4; 1-3-2-4; 1-2-4-3}
     * N(s) with i=3 = {4-2-3-1; 1-4-3-2; 1-2-4-3}
     */
    /**
     * todo : save moves & neighbors & return {sol = ""; movei = ""; movej=""}
     */

    std::vector<std::string> vSolution{explode(solution, '-')};
    std::vector<Neighbor> vAllNeighbors;

    for (int i=0; i < vSolution.size(); i++) {
        for (int j=0; j < vSolution.size(); j++) {
            if (i != j){
                std::vector<std::string> vNeighbor = vSolution;
                std::string tmp = vNeighbor[i];
                vNeighbor[i] = vNeighbor[j];
                vNeighbor[j] = tmp;
                vAllNeighbors.push_back(Neighbor(vectorToString(vNeighbor), i, j));
            }
        }
    }

    return vAllNeighbors;
}