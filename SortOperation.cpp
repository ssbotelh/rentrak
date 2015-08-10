#include "SortOperation.h"
#include "Field.h"
#include "Utility.h"
#include <algorithm>
#include <cassert>

SortOperation::SortOperation(std::vector<std::string> const &vCmds,
                             size_t                   const  priority,
                             DataStore                const &dataStore)
    : Operation(vCmds, priority, dataStore)
{}

SortOperation::~SortOperation()
{}

void SortOperation::Run(std::vector<Record> &vRecords)
{
    assert(m_vsCommands.size() == 1);
    std::string const sSortParams(m_vsCommands.front());
    if (sSortParams.empty())
        throw std::runtime_error("Sort parameter (-o) expects an argument");

    //Collect all field names we want to sort by
    std::vector<std::string> const tokens(Utility::Tokenize(sSortParams, ","));

    std::vector<Field::Name> vFieldNames;
    for (std::string const &token : tokens)
        vFieldNames.emplace_back(Field::ToEnum(token));

    //Sort vRecords by each of the requested fields.
    //WARNING: Must go through fields in "-o" list from last to first!
    for (std::vector<Field::Name>::const_reverse_iterator rit = vFieldNames.rbegin(); rit != vFieldNames.rend(); ++rit) {
        Field::Name const &field(*rit);
        std::sort(vRecords.begin(), vRecords.end(),
                  [&field](Record const &r1, Record const &r2) { return r1.GetFieldValue(field) < r2.GetFieldValue(field); });
    }
}

