#include "GroupOperation.h"
#include "Field.h"
#include "Utility.h"
#include <map>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <iomanip>      // std::setprecision
#include <sstream>

//Helper functions
//
std::string ConvertToStringWithPrecision(double const value)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << value;
    return stream.str();
}

bool IsLessThan(std::string const &l, std::string const &r)
{
    //First assume they're numbers
    try {
        double const dl(std::stod(l));
        double const dr(std::stod(r));
        return (dl < dr);
    } catch(...) {
        return (l < r); //compare as strings
    }

    return false; //never happens
}

///////////////////////////////////////////////////////////////////////////

GroupOperation::GroupOperation(std::vector<std::string> const &vCmds,
                             size_t                   const  priority,
                             DataStore                const &dataStore)
    : Operation(vCmds, priority, dataStore)
    , m_vFieldToAggregate()
{}

GroupOperation::~GroupOperation()
{}

GroupOperation::Aggregate GroupOperation::GetAggregate(std::string const &sToken) const
{
    if      (Utility::ToUpper(sToken) == "MIN")
        return Aggregate::MIN;
    else if (Utility::ToUpper(sToken) == "MAX")
        return Aggregate::MAX;
    else if (Utility::ToUpper(sToken) == "SUM")
        return Aggregate::SUM;
    else if (Utility::ToUpper(sToken) == "COUNT")
        return Aggregate::COUNT;
    else if (Utility::ToUpper(sToken) == "COLLECT")
        return Aggregate::COLLECT;
    else
        throw std::runtime_error("Invalid aggregate token: " + sToken);

    return Aggregate::MIN; //never happens
}

std::string GroupOperation::PerformAggregation(std::vector<Record> const &vRecords, Field::Name const field, Aggregate const aggr) const
{
    std::string result;
    std::set<std::string> setUniqueValues;

    for (Record const &rec : vRecords) {
        std::string const value(rec.GetFieldValue(field));

        switch (aggr) {
            case Aggregate::MIN: {
                try {
                    result = (result.empty() ? value
                                             : (IsLessThan(result, value) ? result : value));
                } catch(...) {
                    throw std::runtime_error("Failed to apply aggregate MIN to field " + Field::ToString(field));
                }
                break;
            }
            case Aggregate::MAX: {
                try {
                    result = (result.empty() ? value
                                             : (!IsLessThan(result, value) ? result : value));
                } catch(...) {
                    throw std::runtime_error("Failed to apply aggregate MAX to field " + Field::ToString(field));
                }
                break;
            }
            case Aggregate::SUM: {
                try {
                    result = (result.empty() ? value
                                             : ConvertToStringWithPrecision(std::stod(result) + std::stod(value)));
                } catch(...) {
                    throw std::runtime_error("Failed to apply aggregate SUM to field " + Field::ToString(field));
                }
                break;
            }
            case Aggregate::COUNT: {
                setUniqueValues.insert(value);
                result = std::to_string(setUniqueValues.size());
                break;
            }
            case Aggregate::COLLECT: {
                result = (result.empty() ? "[" + value + "]"
                                         : result.substr(0, result.size() - 1) + "," + value + "]");
                break;
            }
            default:
                throw std::runtime_error("Unknown aggregate token");
        }
    }

    return result;
}

void GroupOperation::Run(std::vector<Record> &vRecords)
{
    assert(m_vsCommands.size() == 2); // "-g" and "-s"
    std::string const sGroupParam (m_vsCommands.front());
    std::string const sSelectParam(m_vsCommands.back());

    if (sGroupParam.empty())
        throw std::runtime_error("Group parameter (-g) expects an argument");

    //Only one group parameter supported by -g
    if (Utility::Tokenize(sGroupParam, ",:").size() > 1)
        throw std::runtime_error("Group parameter (-g) must be a single field");

    //First collect the "-s" params
    std::vector<std::string> const vSelectTokens(Utility::Tokenize(sSelectParam, ","));
    if (vSelectTokens.empty())
        throw std::runtime_error("Group feature (-g) cannot be used without a SELECT list (-s)");

    //The field we want to group by must be on the SELECT list, and must not be associated with an aggregate
    if (std::find(vSelectTokens.begin(), vSelectTokens.end(), sGroupParam) == vSelectTokens.end())
        throw std::runtime_error("Group field (" + sGroupParam + ") must be on the SELECT list (-s) and cannot have aggregates");

    //Now find aggregates associated with each SELECT field
    for (std::string const &s : vSelectTokens) {
        std::vector<std::string> const vNameValue(Utility::Tokenize(s, ":"));
        assert(!vNameValue.empty());

        if (vNameValue.size() == 2)
            m_vFieldToAggregate.emplace_back(Field::ToEnum(vNameValue.front()), GetAggregate(vNameValue.back()));
        else if (vNameValue.size() == 1)
            m_vFieldToAggregate.emplace_back(Field::ToEnum(vNameValue.front()), Aggregate::NONE);
        else
            throw std::runtime_error("Invalid aggregate: \"" + s + "\"");
    }

    //Exactly one field must be in the SELECT list without aggregates
    if (std::count_if(m_vFieldToAggregate.begin(), m_vFieldToAggregate.end(),
                      [](std::pair<Field::Name, Aggregate> const &p) { return (p.second == Aggregate::NONE); }) != 1)
        throw std::runtime_error("Exactly one field must be in the SELECT list (-s) without aggregates");

    //Now let's do real work!
    //Step 1: Group records by the "group-by" field
    Field::Name const groupByField(Field::ToEnum(sGroupParam));
    std::map<std::string, std::vector<Record>> mapGroupedRecords;
    for (Record const &rec : vRecords)
        mapGroupedRecords[rec.GetFieldValue(groupByField)].push_back(rec);

    std::vector<Record> vOutputRecords;

    //For each group of records...
    for (auto const &group : mapGroupedRecords) {
        std::string         const &groupName(group.first);
        std::vector<Record> const &groupRecords(group.second);

        Record newRecord;

        //For each aggregate...
        for (auto const &aggregate : m_vFieldToAggregate) {
            Field::Name const &field(aggregate.first);
            Aggregate   const &aggr (aggregate.second);

            //Step 2: Aggregate field values across records in a group
            if (aggr == Aggregate::NONE) {
                assert(field == groupByField);
                newRecord.AddField(field, groupName);
            }
            else {
                std::string const sAggregatedValue(PerformAggregation(groupRecords, field, aggr));
                newRecord.AddField(field, sAggregatedValue);
            }
        }

        vOutputRecords.emplace_back(newRecord);
    }

    vRecords.swap(vOutputRecords);
}

