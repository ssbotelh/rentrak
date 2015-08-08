#include "Field.h"
#include <stdexcept>
#include <cassert>

std::string Field::ToString(Field::Name const &name)
{
    switch(name) {
        case Field::STB:        return "STB";
        case Field::TITLE:      return "TITLE";
        case Field::PROVIDER:   return "PROVIDER";
        case Field::DATE:       return "DATE";
        case Field::REV:        return "REV";
        case Field::VIEW_TIME:  return "VIEW_TIME";
        default:                assert(0);
    }

    return std::string(); //never happens
}

Field::Name Field::ToEnum(std::string const &name)
{
    if      (name == "STB")         return Field::STB;
    else if (name == "TITLE")       return Field::TITLE;
    else if (name == "PROVIDER")    return Field::PROVIDER;
    else if (name == "DATE")        return Field::DATE;
    else if (name == "REV")         return Field::REV;
    else if (name == "VIEW_TIME")   return Field::VIEW_TIME;
    else
        throw std::runtime_error("Invalid field name: " + name);

    return Field::STB; //never happens
}


