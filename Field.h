#ifndef RENTRAK_FIELD
#define RENTRAK_FIELD

#include <string>

class Field {
public:
    enum Name {
        STB,
        TITLE,
        PROVIDER,
        DATE,
        REV,
        VIEW_TIME
    };

    typedef std::string Value;

    static std::string ToString(Name        const &name);
    static Name        ToEnum  (std::string const &name);
};

#endif //RENTRAK_FIELD
