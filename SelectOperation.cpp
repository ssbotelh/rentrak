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

    //Iterate over all DB records, populating our records only with the desired fields
    vRecords.clear();
    m_dataStore.Open();

    for (DataStore::const_iterator it = m_dataStore.begin(); it != m_dataStore.end(); ++it) {
        Record const rec(*it);

        Record newRec;
        if (vFieldNames.empty()) {
            newRec = rec;
        } else {
            for (Field::Name const &name : vFieldNames)
                newRec.AddField(rec.GetField(name));
        }

        vRecords.emplace_back(newRec);
    }

    m_dataStore.Close();
}

