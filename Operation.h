#ifndef RENTRAK_OPERATION
#define RENTRAK_OPERATION

#include <string>

class Operation {
protected:
    std::string m_sCommands;

public:
    Operation(std::string const &cmd);
    virtual ~Operation();

    virtual void Run() = 0;
};

#endif //RENTRAK_OPERATION
