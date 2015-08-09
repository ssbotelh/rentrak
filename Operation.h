#ifndef RENTRAK_OPERATION
#define RENTRAK_OPERATION

#include "Record.h"
#include "DataStore.h"
#include <string>
#include <vector>

class Operation {
protected:
    std::string  m_sCommands;
    DataStore   &m_dataStore;

public:
    Operation(std::string const &cmd, DataStore &dataStore)
        : m_sCommands(cmd)
        , m_dataStore(dataStore)
    {}

    virtual ~Operation()
    {}

    virtual void Run(std::vector<Record> &vRecords) const = 0;
};

#endif //RENTRAK_OPERATION
