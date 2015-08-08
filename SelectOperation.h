#ifndef RENTRAK_SELECT_OPERATION
#define RENTRAK_SELECT_OPERATION

#include "Operation.h"
#include <string>

class SelectOperation : public Operation {
public:
    SelectOperation(std::string const &cmd);
    ~SelectOperation();

    void Run();
};

#endif //RENTRAK_SELECT_OPERATION
