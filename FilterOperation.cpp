#include "FilterOperation.h"
#include "Field.h"
#include "Utility.h"
#include <map>
#include <stdexcept>
#include <cassert>

FilterOperation::FilterOperation(std::vector<std::string> const &vCmds,
                                 size_t                   const  priority,
                                 DataStore                const &dataStore)
    : Operation(vCmds, priority, dataStore)
{}

FilterOperation::~FilterOperation()
{}

void FilterOperation::Run(std::vector<Record> &vRecords)
{
    assert(m_vsCommands.size() == 1);
    std::string const sFilterParams(m_vsCommands.front());
    if (sFilterParams.empty())
        throw std::runtime_error("Filter parameter (-f) expects an argument");

    //Collect all filters we want to apply
    std::vector<std::string> const tokens(Utility::Tokenize(sFilterParams, ","));

    std::map<Field::Name, std::string> mapFilters;
    for (std::string const &token : tokens) {
        std::vector<std::string> const vNameValue(Utility::Tokenize(token, "="));

        if (vNameValue.size() != 2)
            throw std::runtime_error("Invalid filter specification: " + token);

        mapFilters.insert(std::make_pair(Field::ToEnum(vNameValue[0]), vNameValue[1]));
    }

    //Try one filter at a time. A successfull match must pass all filters
    std::vector<Record> vFilteredRecords;
    for (Record &rec : vRecords) {
        bool bMatch(true);

        for (auto const &pp : mapFilters) {
            Field::Name const &filterName (pp.first);
            std::string const &filterValue(pp.second);

            if (rec.GetField(filterName).GetValue() != filterValue) {
                bMatch = false;
                break;
            }
        }

        if (bMatch)
            vFilteredRecords.emplace_back(rec);
    }

    vRecords.swap(vFilteredRecords);
}

