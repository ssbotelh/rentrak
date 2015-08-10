#include "SelectOperation.h"
#include "Field.h"
#include "Utility.h"
#include <cassert>

SelectOperation::SelectOperation(std::vector<std::string> const &vCmds,
                                 size_t                   const  priority,
                                 DataStore                const &dataStore)
    : Operation(vCmds, priority, dataStore)
{}

SelectOperation::~SelectOperation()
{}

void SelectOperation::Run(std::vector<Record> &vRecords)
{
    //Collect all field names we want to select
    assert(m_vsCommands.size() == 1);
    std::vector<std::string> const tokens(Utility::Tokenize(m_vsCommands.front(), ","));

    std::vector<Field::Name> vFieldNames;
    for (std::string const &token : tokens)
        vFieldNames.emplace_back(Field::ToEnum(Utility::Tokenize(token, ":").front()));

    //Fetch all DB records, populating only the selected fields
    m_dataStore.FetchRecords(vFieldNames, vRecords);
}

