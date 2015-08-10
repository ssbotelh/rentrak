#include "CmdLineArgs.h"
#include "TaskExecutor.h"
#include <iostream>
#include <stdexcept>

///////////////////////////

int main(int argc, char *argv[])
{

    try {
        CmdLineArgs cmdParams(argc, argv);
//        cmdParams.Print();

        TaskExecutor exec(cmdParams);
        exec.Run();
    }
    catch (std::exception &e) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
