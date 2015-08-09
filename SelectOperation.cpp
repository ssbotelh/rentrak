#include "SelectOperation.h"
#include "Field.h"
#include "Utility.h"

SelectOperation::SelectOperation(std::string const &cmd, size_t const priority, DataStore &dataStore)
    : Operation(cmd, priority, dataStore)
{}

SelectOperation::~SelectOperation()
{}

void SelectOperation::Run(std::vector<Record> &vRecords) const
{
    //Collect all field names we want to select
    std::vector<std::string> tokens;
    Utility::Tokenize(m_sCommands, tokens, ",");

    std::vector<Field::Name> vFieldNames;
    for (std::string const &token : tokens)
        vFieldNames.emplace_back(Field::ToEnum(token));

    //Fetch all DB records, populating only the selected fields
    m_dataStore.FetchRecords(vFieldNames, vRecords);
}

