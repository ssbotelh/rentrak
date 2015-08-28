#ifndef RENTRAK_SORT_OPERATION
#define RENTRAK_SORT_OPERATION

#include "Operation.h"
#include <string>

class SortOperation : public Operation {
private:
    std::string m_sSortParams;

public:
    SortOperation(std::string const &selectFlds,
                  size_t      const  priority,
                  DataStore   const &dataStore);

    ~SortOperation();

    void Run(std::vector<Record> &vRecords);
    void SetExtraParam(std::string const &param) override { m_sSortParams = param; }
};

#endif //RENTRAK_SORT_OPERATION
