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

///////////////////////////////////////////////////////////////////////////

GroupOperation::GroupOperation(std::vector<std::string> const &vCmds,
                             size_t                   const  priority,
                             DataStore                const &dataStore)
    : Operation(vCmds, priority, dataStore)
    , m_vFieldToAggregate()
{}

GroupOperation::~GroupOperation()
{}

GroupOperation::Aggregate GroupOperation::GetAggregate(std::string const &token) const
{
    std::string const Token(Utility::ToUpper(token));

    if      (Token == "MIN")
        return Aggregate::MIN;
    else if (Token == "MAX")
        return Aggregate::MAX;
    else if (Token == "SUM")
        return Aggregate::SUM;
    else if (Token == "COUNT")
        return Aggregate::COUNT;
    else if (Token == "COLLECT")
        return Aggregate::COLLECT;
    else
        throw std::runtime_error("Invalid aggregate token: " + token);

    return Aggregate::MIN; //never happens
}

Field GroupOperation::PerformAggregation(std::vector<Record> const &vRecords, Field::Name const name, Aggregate const aggr) const
{
    Field newField;
    std::set<std::string> setUniqueValues;

    if (aggr == Aggregate::NONE) {
        assert(!vRecords.empty());
        return vRecords.front().GetField(name);
    }

    for (Record const &rec : vRecords) {
        Field const field(rec.GetField(name));

        switch (aggr) {
            case Aggregate::MIN: {
                try {
                    newField = (newField.IsValid() ? (newField < field ? newField : field)
                                                   : field);
                } catch(...) {
                    throw std::runtime_error("Failed to apply aggregate MIN to field " + Field::ToString(name));
                }
                break;
            }
            case Aggregate::MAX: {
                try {
                    newField = (newField.IsValid() ? (newField > field ? newField : field)
                                                   : field);
                } catch(...) {
                    throw std::runtime_error("Failed to apply aggregate MAX to field " + Field::ToString(name));
                }
                break;
            }
            case Aggregate::SUM: {
                try {
                    newField = (newField.IsValid() ? newField + field : field);
                } catch(...) {
                    throw std::runtime_error("Failed to apply aggregate SUM to field " + Field::ToString(name));
                }
                break;
            }
            case Aggregate::COUNT: {
                setUniqueValues.insert(field.GetValue());
                newField = Field(name, std::to_string(setUniqueValues.size()));
                break;
            }
            case Aggregate::COLLECT: {
                std::string collection(newField.IsValid() ? newField.GetValue() : "");
                collection = (collection.empty() ? "[" + field.GetValue() + "]"
                                                 : collection.substr(0, collection.size() - 1) + "," + field.GetValue() + "]");
                newField = Field(name, collection);
                break;
            }
            default:
                throw std::runtime_error("Unknown aggregate token");
        }
    }

    return newField;
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
        mapGroupedRecords[rec.GetField(groupByField).GetValue()].push_back(rec);

    std::vector<Record> vOutputRecords;

    //For each group of records...
    for (auto const &group : mapGroupedRecords) {
//        std::string         const &groupName(group.first);
        std::vector<Record> const &groupRecords(group.second);

        Record newRecord;

        //For each aggregate...
        for (auto const &aggregate : m_vFieldToAggregate) {
            Field::Name const &name(aggregate.first);
            Aggregate   const &aggr(aggregate.second);

            //Step 2: Aggregate field values across records in a group
            Field const newField(PerformAggregation(groupRecords, name, aggr));
            newRecord.AddField(newField);
        }

        vOutputRecords.emplace_back(newRecord);
    }

    vRecords.swap(vOutputRecords);
}

