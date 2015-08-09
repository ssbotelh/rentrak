#ifndef RENTRAK_SELECT_OPERATION
#define RENTRAK_SELECT_OPERATION

#include "Operation.h"
#include <string>

class SelectOperation : public Operation {
public:
    SelectOperation(std::string const &cmd, size_t const priority, DataStore &dataStore);
    ~SelectOperation();

    void Run(std::vector<Record> &vRecords) const;
};

#endif //RENTRAK_SELECT_OPERATION
