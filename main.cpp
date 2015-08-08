#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "CmdLineArgs.h"
#include "TaskExecutor.h"
//#include <algorithm>
//#include <vector>
//#include <map>
//#include <set>
#include <iostream>
#include <exception>

//#include <sys/time.h>
//#include <time.h>
//using namespace std;

///////////////////////////

int main(int argc, char *argv[])
{

    try {
        CmdLineArgs cmdParams(argc, argv);
//        cmdParams.Print();

        TaskExecutor exec(cmdParams);
        exec.Run();
/*
        Record r;
        r.AddField(Field::STB, "SomeField");
        Field::Value const v(r.GetFieldValue(Field::STB));
        std::cout << "v = " << v << std::endl;
*/
    }
    catch (std::exception &e) {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
    }

    return 0;
}
