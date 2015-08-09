#ifndef RENTRAK_FILE_MANAGER
#define RENTRAK_FILE_MANAGER

#include "DataStore.h"
#include "Record.h"
#include <string>
#include <vector>

class FileManager {
private: //fields
    DataStore m_dataStore;

public:
    FileManager(std::string const &dbFileName);
    ~FileManager();

    void ImportDataFromFile(std::string const &file);
    void FetchRecords(std::vector<Field::Name> const &vFieldNames, std::vector<Record> &vRecords);
};

#endif //RENTRAK_FILE_MANAGER
