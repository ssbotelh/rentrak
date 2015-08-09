#include "SortOperation.h"
#include "Field.h"
#include "Utility.h"
#include <algorithm>

SortOperation::SortOperation(std::string const &cmd, size_t const priority, DataStore &dataStore)
    : Operation(cmd, priority, dataStore)
{}

SortOperation::~SortOperation()
{}

void SortOperation::Run(std::vector<Record> &vRecords) const
{
    //Collect all field names we want to sort by
    std::vector<std::string> tokens;
    Utility::Tokenize(m_sCommands, tokens, ",");

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

