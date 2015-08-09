#include "Record.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string.h>
#include <iomanip>
#include <cassert>

Record::Record()
    : m_Fields()
{}

Record::Record(std::vector<std::string> const &vValues)
{
    assert(vValues.size() == 6);

    AddField(Field::STB,       vValues[0]);
    AddField(Field::TITLE,     vValues[1]);
    AddField(Field::PROVIDER,  vValues[2]);
    AddField(Field::DATE,      vValues[3]);
    AddField(Field::REV,       vValues[4]);
    AddField(Field::VIEW_TIME, vValues[5]);
}

Record::~Record()
{}

bool Record::operator==(Record const &that) const
{
    return ((GetFieldValue(Field::STB)   == that.GetFieldValue(Field::STB))   &&
            (GetFieldValue(Field::TITLE) == that.GetFieldValue(Field::TITLE)) &&
            (GetFieldValue(Field::DATE)  == that.GetFieldValue(Field::DATE)));
}

void Record::AddField(Field::Name const &name, Field::Value const &value)
{
    auto const findIt(std::find_if(m_Fields.begin(), m_Fields.end(),
                      [&name](std::pair<Field::Name, Field::Value> const &p) { return p.first == name; }));

    if (findIt != m_Fields.cend())
        findIt->second = value;
    else
        m_Fields.emplace_back(name, value);
}

bool Record::HasField(Field::Name const &name) const
{
    auto const findIt(std::find_if(m_Fields.begin(), m_Fields.end(),
                      [&name](std::pair<Field::Name, Field::Value> const &p) { return p.first == name; }));
    return (findIt != m_Fields.cend());
}

Field::Value Record::GetFieldValue(Field::Name const &name) const
{
    Field::Value value;

    auto const findIt(std::find_if(m_Fields.begin(), m_Fields.end(),
                      [&name](std::pair<Field::Name, Field::Value> const &p) { return p.first == name; }));

    if (findIt != m_Fields.cend())
        value = findIt->second;

    return value;
}

void Record::Print() const
{
    size_t counter(0);

    for (auto const &p : m_Fields) {
        if (p.second.empty())
            continue;

        std::cout << p.second;
        if (counter < m_Fields.size() - 1)
            std::cout << ",";

        ++counter;
    }

    std::cout << std::endl;
}

