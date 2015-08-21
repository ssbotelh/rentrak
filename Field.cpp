#include "Field.h"
#include "Utility.h"
#include <stdexcept>
#include <cassert>

//Default constructor
Field::Field()
    : m_type(INVALID_DATA)
    , m_name(INVALID_NAME)
    , m_value()
{}

// Constructor from string
Field::Field(Name const &name, std::string const &value)
    : m_type(STRING_DATA)
    , m_name(name)
    , m_value()
{
    m_value.asString = new std::string(value);
    assert(m_value.asString != nullptr);
}

//Constructor from two numbers
Field::Field(Name const &name, unsigned const left, unsigned const right)
    : m_type(name == REV ? DOLLAR_DATA : TIME_DATA)
    , m_name(name)
    , m_value()
{
    m_value.asNumberPair = new std::pair<unsigned, unsigned>(std::make_pair(left, right));
    assert(m_value.asNumberPair != nullptr);
}

//Copy constructor
Field::Field(Field const &that)
    : m_type(that.m_type)
    , m_name(that.m_name)
    , m_value() //let assignment operator take care of that
{
    *this = that;
}

//Move constructor
Field::Field(Field &&that) noexcept
    : m_type(INVALID_DATA)
    , m_name(INVALID_NAME)
    , m_value()
{
    std::swap(m_type, that.m_type);
    std::swap(m_name, that.m_name);
    std::swap(m_value, that.m_value);
}

//Destructor
Field::~Field()
{
    Invalidate();
}

//Copy assignment operator
Field &Field::operator=(Field const &that)
{
    if (this != &that) {
        Invalidate();
        m_type = that.m_type;
        m_name = that.m_name;

        switch(m_type) {
            case STRING_DATA:
                m_value.asString = new std::string(*that.m_value.asString);
                assert(m_value.asString != nullptr);
                break;

            case DOLLAR_DATA:
            case TIME_DATA:
                m_value.asNumberPair = new std::pair<unsigned, unsigned>(*that.m_value.asNumberPair);
                assert(m_value.asNumberPair != nullptr);
                break;

            case INVALID_DATA:
                //do nothing
                break;
        }
    }

    return *this;
}

//Move assignment operator
Field &Field::operator=(Field &&that) noexcept
{
    std::swap(m_type, that.m_type);
    std::swap(m_name, that.m_name);
    std::swap(m_value, that.m_value);
    return *this;
}

//Summation operator
Field Field::operator+(Field const &that) const
{
    assert(m_type == that.m_type);
    assert(m_name == that.m_name);

    switch (m_type) {
        case STRING_DATA: {
            Field const sum(m_name, *m_value.asString + *that.m_value.asString);
            return sum;
        }
        case DOLLAR_DATA: {
                unsigned const dolr1(m_value.asNumberPair->first);
                unsigned const dolr2(that.m_value.asNumberPair->first);
                unsigned const cent1(m_value.asNumberPair->second);
                unsigned const cent2(that.m_value.asNumberPair->second);

                unsigned const dolr_sum(dolr1 + dolr2 + ((cent1 + cent2) / 100));
                unsigned const cent_sum((cent1 + cent2) % 100);

                Field const sum(m_name, dolr_sum, cent_sum);
                return sum;
        }
        case TIME_DATA: {
                unsigned const hr1(m_value.asNumberPair->first);
                unsigned const hr2(that.m_value.asNumberPair->first);
                unsigned const mn1(m_value.asNumberPair->second);
                unsigned const mn2(that.m_value.asNumberPair->second);

                unsigned const hr_sum(hr1 + hr2 + ((mn1 + mn2) / 60));
                unsigned const mn_sum((mn1 + mn2) % 60);

                Field const sum(m_name, hr_sum, mn_sum);
                return sum;
        }
        case INVALID_DATA: break;
    }

    return Field(); //never happens
}

//Is-equal-to operator
bool Field::operator==(Field const &that) const
{
    if (m_type != that.m_type)
        return false;

    switch (m_type) {
        case STRING_DATA: return (*m_value.asString == *that.m_value.asString);
        case DOLLAR_DATA:
        case TIME_DATA:   return (*m_value.asNumberPair == *that.m_value.asNumberPair);
        case INVALID_DATA: return true;
    }

    return true; //never happens
}

//Is-different-from operator
bool Field::operator!=(Field const &that) const
{
    return !(*this == that);
}

//Less-than operator
bool Field::operator<(Field const &that) const
{
    assert(m_type == that.m_type);
    assert(m_type != INVALID_DATA);

    switch (m_type) {
        case STRING_DATA: return (*m_value.asString < *that.m_value.asString);
        case DOLLAR_DATA:
        case TIME_DATA:   {
            auto const pp1(*m_value.asNumberPair);
            auto const pp2(*that.m_value.asNumberPair);

            return (pp1.first == pp2.first ? pp1.second < pp2.second : pp1.first < pp2.first);
        }
        case INVALID_DATA: return true; //never happens (asserted above)
    }

    return false; //never happens
}

//Greater-than operator
bool Field::operator>(Field const &that) const
{
    return !(*this < that);
}

void Field::Invalidate()
{
    switch (m_type) {
        case STRING_DATA:
            delete m_value.asString;
            m_value.asString = nullptr;
            break;

        case DOLLAR_DATA:
        case TIME_DATA:
            delete m_value.asNumberPair;
            m_value.asNumberPair = nullptr;
            break;

        case INVALID_DATA:
            //do nothing
            break;
    }

    m_type = INVALID_DATA;
}

std::string Field::GetValue() const
{
    std::string value;

    switch (m_type) {
        case STRING_DATA:
            if (m_value.asString != nullptr)
                value = *m_value.asString;
            break;

        case DOLLAR_DATA:
        case TIME_DATA:
            if (m_value.asNumberPair != nullptr) {
                auto const pp = *m_value.asNumberPair;
                value = std::to_string(pp.first) +
                        (m_type == DOLLAR_DATA ? "." : ":") +
                        (pp.second < 10 ? "0" : "") +
                        std::to_string(pp.second);
            }
            break;

        case INVALID_DATA:
            //do nothing
            break;
    }

    return value;
}

std::string Field::ToString(Field::Name const &name)
{
    switch(name) {
        case Field::STB:        return "STB";
        case Field::TITLE:      return "TITLE";
        case Field::PROVIDER:   return "PROVIDER";
        case Field::DATE:       return "DATE";
        case Field::REV:        return "REV";
        case Field::VIEW_TIME:  return "VIEW_TIME";
        default:                return "INVALID_NAME";
    }

    return std::string(); //never happens
}

Field::Name Field::ToEnum(std::string const &name)
{
    std::string const Name(Utility::ToUpper(name));

    if      (Name == "STB")         return STB;
    else if (Name == "TITLE")       return TITLE;
    else if (Name == "PROVIDER")    return PROVIDER;
    else if (Name == "DATE")        return DATE;
    else if (Name == "REV")         return REV;
    else if (Name == "VIEW_TIME")   return VIEW_TIME;
    else {
        throw std::runtime_error("Unknown field \"" + name + "\"");
    }

    return INVALID_NAME; //never happens
}

