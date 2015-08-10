#ifndef RENTRAK_FILTER_OPERATION
#define RENTRAK_FILTER_OPERATION

#include "Operation.h"
#include <string>

class FilterOperation : public Operation {
public:
    FilterOperation(std::string const &cmd, size_t const priority, DataStore &dataStore);
    ~FilterOperation();

    void Run(std::vector<Record> &vRecords) const;
};

#endif //RENTRAK_FILTER_OPERATION
