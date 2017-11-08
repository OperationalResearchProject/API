//
// Created by geoffrey on 15/04/17.
//

#ifndef OPERATIONALRESEARCHAPI_HELPER_H
#define OPERATIONALRESEARCHAPI_HELPER_H
#include <random>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/collection.hpp>

std::string generateId();
std::string getNeighbourSolution(const std::string& solution);
const std::vector<std::string> explode(const std::string& s, const char& c);
const std::string vectorToString(std::vector<std::string> v);
const int getIteration(mongocxx::collection transac_collection, bsoncxx::oid  transaction_id);

#endif //OPERATIONALRESEARCHAPI_HELPER_H
