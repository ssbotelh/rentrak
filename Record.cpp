#include "Record.h"
#include <stdexcept>

Record::Record()
    : m_Fields()
{}

Record::~Record()
{}

void Record::AddField(Field::Name const &name, Field::Value const &value)
{
    m_Fields[name] = value;
}

bool Record::HasField(Field::Name const &name) const
{
    return (m_Fields.find(name) != m_Fields.cend());
}

Field::Value Record::GetFieldValue(Field::Name const &name) const
{
    auto const iter(m_Fields.find(name));
    if (iter == m_Fields.cend())
        throw std::runtime_error("Field not found: " + Field::ToString(name));

    return iter->second;
}
 
