#ifndef RENTRAK_OPERATION
#define RENTRAK_OPERATION

#include "Record.h"
#include "DataStore.h"
#include "Utility.h"
#include <string>
#include <vector>

class Operation {
protected:
    std::string      m_sSelectFields;
    size_t           m_uPriority;
    DataStore const &m_dataStore;

    bool IsOnSelectList(Field::Name const &name) const {
        return Utility::ToUpper(m_sSelectFields).find(Field::ToString(name)) != std::string::npos;
    }

public:
    Operation(std::string const &selectFlds, size_t const priority, DataStore const &dataStore)
        : m_sSelectFields(selectFlds)
        , m_uPriority(priority)
        , m_dataStore(dataStore)
    {}

    virtual ~Operation()
    {}

    virtual void Run(std::vector<Record> &vRecords) = 0;
    virtual void SetExtraParam(std::string const &param) {}
    size_t       GetPriority() const { return m_uPriority; }
};

#endif //RENTRAK_OPERATION
