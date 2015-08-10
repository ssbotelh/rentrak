#ifndef RENTRAK_SORT_OPERATION
#define RENTRAK_SORT_OPERATION

#include "Operation.h"
#include <string>

class SortOperation : public Operation {
public:
    SortOperation(std::vector<std::string> const &vCmds,
                  size_t                   const  priority,
                  DataStore                const &dataStore);

    ~SortOperation();

    void Run(std::vector<Record> &vRecords);
};

#endif //RENTRAK_SORT_OPERATION
