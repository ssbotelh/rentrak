#ifndef RENTRAK_FIELD
#define RENTRAK_FIELD

#include <string>

class Field {
private: //enums
    enum Type {
        INVALID_DATA,
        STRING_DATA,
        DOLLAR_DATA,
        TIME_DATA
    } m_type;

public: //enums
    enum Name {
        INVALID_NAME,
        STB,
        TITLE,
        PROVIDER,
        DATE,
        REV,
        VIEW_TIME
    };

private: //fields
    Name m_name;

    union {
        std::string                   *asString;
        std::pair<unsigned, unsigned> *asNumberPair;
    } m_value;


private: //methods
    void Invalidate();

public:
    Field();
    Field(Name const &name, std::string const &value);
    Field(Name const &name, unsigned const left, unsigned const right);
    Field(Field const &that);
    Field(Field &&that) noexcept;
    ~Field();

    Field &operator=(Field const &that);
    Field &operator=(Field &&that) noexcept;
    Field  operator+(Field const &that) const;

    bool operator==(Field const &that) const;
    bool operator!=(Field const &that) const;
    bool operator< (Field const &that) const;
    bool operator> (Field const &that) const;

    bool IsValid() const { return m_type != INVALID_DATA; }
    Name GetName() const { return m_name; }
    std::string GetValue() const;

    static std::string ToString(Name        const &name);
    static Name        ToEnum  (std::string const &name);
};

#endif //RENTRAK_FIELD
