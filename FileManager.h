#ifndef RENTRAK_FILE_MANAGER
#define RENTRAK_FILE_MANAGER

#include "DataStore.h"
#include "Record.h"
#include <string>
#include <vector>

class FileManager {
private: //fields
    DataStore m_dataStore;

private: //methods
    void Tokenize(std::string const &input, std::vector<std::string> &tokens, std::string const &delim, bool bAllowEmptyTokens = false);

public:
    FileManager(std::string const &dbFileName);
    ~FileManager();

    void ImportDataFromFile(std::string const &file);
};

#endif //RENTRAK_FILE_MANAGER
