//
// Created by Rafael
//

#include "DatabaseManager.h"
#include <iostream>

// Подключаемся к базе данных или создаем ее в текущем каталоге
DatabaseManager::DatabaseManager(const std::string &dbName) {
    int rc = sqlite3_open(dbName.c_str(),&db);
    if (rc){
        std::cerr << "Не удалось открыть базу данных: " << sqlite3_errmsg(db) << "\n";
        exit(1);
    } else {
        std::cout << "База данных открыта успешно\n";
    }

    const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS NOTES("
                                 "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "DATE TEXT NOT NULL,"
                                 "USER TEXT NOT NULL,"
                                 "CONTENT TEXT NOT NULL);";
    rc = sqlite3_exec(db, sqlCreateTable, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка SQL: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

DatabaseManager::~DatabaseManager() {
    sqlite3_close(db);
}

sqlite3* DatabaseManager::getDB() {
    return db;
}
