#ifndef RENTRAK_FILTER_OPERATION
#define RENTRAK_FILTER_OPERATION

#include "Operation.h"
#include <string>

class FilterOperation : public Operation {

private:
    std::string m_sFilterParams;

public:
    FilterOperation(std::string const &selectFlds,
                    size_t      const  priority,
                    DataStore   const &dataStore);

    ~FilterOperation();

    void Run(std::vector<Record> &vRecords);
    void SetExtraParam(std::string const &param) override { m_sFilterParams = param; }
};

#endif //RENTRAK_FILTER_OPERATION
