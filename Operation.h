#ifndef RENTRAK_OPERATION
#define RENTRAK_OPERATION

#include "Record.h"
#include "DataStore.h"
#include <string>
#include <vector>

class Operation {
protected:
    std::string  m_sCommands;
    size_t       m_uPriority;
    DataStore   &m_dataStore;

public:
    Operation(std::string const &cmd, size_t const priority, DataStore &dataStore)
        : m_sCommands(cmd)
        , m_uPriority(priority)
        , m_dataStore(dataStore)
    {}

    virtual ~Operation()
    {}

    virtual void Run(std::vector<Record> &vRecords) const = 0;
    size_t       GetPriority() const { return m_uPriority; }
};

#endif //RENTRAK_OPERATION
