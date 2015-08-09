#ifndef RENTRAK_SORT_OPERATION
#define RENTRAK_SORT_OPERATION

#include "Operation.h"
#include <string>

class SortOperation : public Operation {
public:
    SortOperation(std::string const &cmd, size_t const priority, DataStore &dataStore);
    ~SortOperation();

    void Run(std::vector<Record> &vRecords) const;
};

#endif //RENTRAK_SORT_OPERATION
