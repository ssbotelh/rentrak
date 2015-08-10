#ifndef RENTRAK_SELECT_OPERATION
#define RENTRAK_SELECT_OPERATION

#include "Operation.h"
#include <string>

class SelectOperation : public Operation {
public:
    SelectOperation(std::vector<std::string> const &vCmds,
                    size_t                   const  priority,
                    DataStore                const &dataStore);

    ~SelectOperation();

    void Run(std::vector<Record> &vRecords);
};

#endif //RENTRAK_SELECT_OPERATION
