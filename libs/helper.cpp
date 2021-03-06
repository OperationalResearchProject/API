#include "helper.h"

std::string generateId(){
    std::string res = "";
    std::random_device rd;

    for (int i = 0; i < 20; ++i) {
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(48,90);
        auto random_integer = uni(rng);
        res = res + (char) random_integer;
    }

    return res;
};

std::string getNeighbourSolution(const std::string& solution, const bool isKnapSack){
    // convert solution from string to vector
    std::vector< std::string> v{explode(solution, '-')};

    std::random_device rd;
    std::mt19937 rng(rd());    // random-number engine used Mersenne-Twister
    std::uniform_int_distribution<int> uni(0, (int) v.size() - 1);

    if(!isKnapSack) {
        // swap 2 elements
        auto random_integer1 = uni(rng);
        auto random_integer2 = uni(rng);

        std::string str_tmp = v[random_integer1];
        v[random_integer1] = v[random_integer2];
        v[random_integer2] = str_tmp;
    }else{
        auto random_integer = uni(rng);
        v[random_integer] = v[random_integer] == "0" ? "1" : "0";
    }

    return  vectorToString(v);
}

const std::vector<std::string> explode(const std::string& s, const char& c)
{
    std::string buff{""};
    std::vector<std::string> v;

    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);

    return v;
}

const std::string vectorToString(std::vector<std::string> v){
    // convert solution from vector to string
    std::ostringstream oss;
    std::copy(v.begin(), v.end()-1, std::ostream_iterator<std::string>(oss, "-"));
    oss << v.back();

    return  oss.str();
}

const int getIteration(mongocxx::collection transac_collection, bsoncxx::oid  transaction_id){

    auto filterTransactionToSearch = bsoncxx::builder::stream::document{} ;
    filterTransactionToSearch << "_id" << transaction_id;

    mongocxx::stdx::optional<bsoncxx::document::value> view = transac_collection.find_one(filterTransactionToSearch.view());

    if(view){
        bsoncxx::document::view viewTransac = *view;
        bsoncxx::document::element elt = viewTransac["iteration"];

        return elt.get_int32();
    } else{
        return -1;
    }
}