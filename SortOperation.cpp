#include "SortOperation.h"
#include "Field.h"
#include "Utility.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>

SortOperation::SortOperation(std::string const &selectFlds,
                             size_t      const  priority,
                             DataStore   const &dataStore)
    : Operation(selectFlds, priority, dataStore)
    , m_sSortParams()
{}

SortOperation::~SortOperation()
{}

void SortOperation::Run(std::vector<Record> &vRecords)
{
    if (m_sSortParams.empty())
        throw std::runtime_error("Sort parameter (-o) expects an argument");

    //Collect all field names we want to sort by
    std::vector<std::string> const tokens(Utility::Tokenize(m_sSortParams, ","));

    std::vector<Field::Name> vFieldNames;
    for (std::string const &token : tokens)
        vFieldNames.emplace_back(Field::ToEnum(token));

    //Sort vRecords by each of the requested fields.
    //WARNING: Must go through fields in "-o" list from last to first!
    for (std::vector<Field::Name>::const_reverse_iterator rit = vFieldNames.rbegin(); rit != vFieldNames.rend(); ++rit) {
        Field::Name const &name(*rit);

        if (!IsOnSelectList(name))
            throw std::runtime_error("Sort field \"" + Field::ToString(name) + "\" not on select list (-s)");

        std::sort(vRecords.begin(), vRecords.end(),
                  [&name](Record const &r1, Record const &r2) { return r1.GetField(name) < r2.GetField(name); });
    }
}

