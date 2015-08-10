#ifndef RENTRAK_OPERATION
#define RENTRAK_OPERATION

#include "Record.h"
#include "DataStore.h"
#include <string>
#include <vector>

class Operation {
protected:
    std::vector<std::string>    m_vsCommands;
    size_t                      m_uPriority;
    DataStore const            &m_dataStore;

public:
    Operation(std::vector<std::string> const &vCmds, size_t const priority, DataStore const &dataStore)
        : m_vsCommands(vCmds)
        , m_uPriority(priority)
        , m_dataStore(dataStore)
    {}

    virtual ~Operation()
    {}

    virtual void Run(std::vector<Record> &vRecords) = 0;
    size_t       GetPriority() const { return m_uPriority; }
};

#endif //RENTRAK_OPERATION
