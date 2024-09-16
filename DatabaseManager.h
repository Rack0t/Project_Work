//
// Created by Rafael
//

#ifndef NOTE_TASK_JOB_DATABASEMANAGER_H
#define NOTE_TASK_JOB_DATABASEMANAGER_H

#include <string>
#include <sqlite3.h>


class DatabaseManager {
private:
    sqlite3* db;
    char* errMsg;

public:
    DatabaseManager(const std::string& dbName);
    ~DatabaseManager();
    sqlite3* getDB();
};

#endif //NOTE_TASK_JOB_DATABASEMANAGER_H
