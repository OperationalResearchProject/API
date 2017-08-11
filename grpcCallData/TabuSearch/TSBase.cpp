#include "TSBase.h"

TSBase::TSBase(ts::TabouSearchService::AsyncService *service, ServerCompletionQueue *cq, mongocxx::database db):
        CallData(cq), db_(db), service_(service){
    transac_coll = db[TRANSACTION_COLLECTION];
    fitness_coll = db[FITNESS_COLLECTION];
}
