#include "Record.h"
#include "Utility.h"
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

    AddField(Field(Field::STB,       vValues[0]));
    AddField(Field(Field::TITLE,     vValues[1]));
    AddField(Field(Field::PROVIDER,  vValues[2]));
    AddField(Field(Field::DATE,      vValues[3]));
    AddField(Field(Field::REV,       Utility::LeftPart(vValues[4], '.'), Utility::RightPart(vValues[4], '.')));
    AddField(Field(Field::VIEW_TIME, Utility::LeftPart(vValues[5], ':'), Utility::RightPart(vValues[5], ':')));
}

Record::~Record()
{}

bool Record::operator==(Record const &that) const
{
    return ((GetField(Field::STB)   == that.GetField(Field::STB))   &&
            (GetField(Field::TITLE) == that.GetField(Field::TITLE)) &&
            (GetField(Field::DATE)  == that.GetField(Field::DATE)));
}

void Record::AddField(Field const &field)
{
    auto findIt(std::find_if(m_Fields.begin(), m_Fields.end(),
                [&field](Field const &f) { return f.GetName() == field.GetName(); }));

    if (findIt != m_Fields.cend())
        *findIt = field;
    else
        m_Fields.emplace_back(field);
}

void Record::AddField(Field &&field)
{
    auto findIt(std::find_if(m_Fields.begin(), m_Fields.end(),
                [&field](Field const &f) { return f.GetName() == field.GetName(); }));

    if (findIt != m_Fields.cend())
        *findIt = field;
    else
        m_Fields.emplace_back(std::move(field));
}

bool Record::HasField(Field::Name const &name) const
{
    auto const findIt(std::find_if(m_Fields.begin(), m_Fields.end(),
                      [&name](Field const &f) { return f.GetName() == name; }));
    return (findIt != m_Fields.cend());
}

Field Record::GetField(Field::Name const &name) const
{
    Field field;

    auto const findIt(std::find_if(m_Fields.begin(), m_Fields.end(),
                      [&name](Field const &f) { return f.GetName() == name; }));

    if (findIt != m_Fields.cend())
        field = *findIt;

    return field;
}

std::string Record::GetFieldValue(Field::Name const &name) const
{
    return GetField(name).GetValue();
}

void Record::Print() const
{
    size_t counter(0);

    for (Field const &f : m_Fields) {
        std::cout << f.GetValue();

        if (counter < m_Fields.size() - 1)
            std::cout << ",";

        ++counter;
    }

    std::cout << std::endl;
}

