#include "FilterOperation.h"
#include "Field.h"
#include "Utility.h"
#include <map>
#include <stdexcept>

FilterOperation::FilterOperation(std::string const &cmd, size_t const priority, DataStore &dataStore)
    : Operation(cmd, priority, dataStore)
{}

FilterOperation::~FilterOperation()
{}

void FilterOperation::Run(std::vector<Record> &vRecords) const
{
    //Collect all filters we want to apply
    std::vector<std::string> tokens;
    Utility::Tokenize(m_sCommands, tokens, ",");

    std::map<Field::Name, std::string> mapFilters;
    for (std::string const &token : tokens) {
        std::vector<std::string> vNameValue;
        Utility::Tokenize(token, vNameValue, "=");

        if (vNameValue.size() != 2)
            throw std::runtime_error("Invalid filter specification: " + token);

        mapFilters.insert(std::make_pair(Field::ToEnum(vNameValue[0]), vNameValue[1]));
    }

    //Try one filter at a time. For each match, increment the score.
    //At the end, records with a score equal to the number of filters get selected.
    std::vector<Record> vFilteredRecords;
    for (Record &rec : vRecords) {
        size_t score(0);

        for (auto const &pp : mapFilters) {
            Field::Name const &filterName (pp.first);
            std::string const &filterValue(pp.second);

            if (rec.GetFieldValue(filterName) == filterValue)
                ++score;
        }

        if (score == mapFilters.size())
            vFilteredRecords.emplace_back(rec);
    }

    vRecords.swap(vFilteredRecords);
}

