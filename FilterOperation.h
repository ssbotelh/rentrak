#ifndef RENTRAK_FILTER_OPERATION
#define RENTRAK_FILTER_OPERATION

#include "Operation.h"
#include <string>

class FilterOperation : public Operation {
public:
    FilterOperation(std::vector<std::string> const &vCmds,
                    size_t                   const  priority,
                    DataStore                const &dataStore);

    ~FilterOperation();

    void Run(std::vector<Record> &vRecords);
};

#endif //RENTRAK_FILTER_OPERATION
