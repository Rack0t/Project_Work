//
// Created by Rafael
//

#include "NoteSearchManager.h"
#include <iostream>

// Конструктор

NoteSearchManager::NoteSearchManager(DatabaseManager &dbMgr) : dbManager(dbMgr) {}

// Создание FTS-таблицы для полнотекстового поиска
void NoteSearchManager::createFTSTable() {
    sqlite3* db = dbManager.getDB();
    const char *sqlCreateFTS = "CREATE VIRTUAL TABLE IF NOT EXISTS NOTES_FTS USING fts5(ID, CONTENT);";
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sqlCreateFTS, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка создания FTS таблицы: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "FTS таблица успешно создана" << std::endl;
    }
}

// Вставка заметок в FTS таблицу для последующего поиска
void NoteSearchManager::insertNoteForSearch(int id, const std::string &content) {
    sqlite3* db = dbManager.getDB();
    std::string sqlInsertFTS = "INSERT INTO NOTES_FTS (ID, CONTENT) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlInsertFTS.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Привязываем параметры (ID и текст)
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, content.c_str(), -1, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка добавления в FTS таблицу: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Запись добавлена в таблицу для поиска" << std::endl;
    }
    sqlite3_finalize(stmt);
}

// Поиск заметок по тексту
void NoteSearchManager::searchNotes(const std::string &query) {
    sqlite3* db = dbManager.getDB();
    std::string sqlSearch = "SELECT ID, CONTENT FROM NOTES_FTS WHERE CONTENT LIKE '%' || ? || '%';";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlSearch.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса поиска: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Привязываем поисковый запрос
    sqlite3_bind_text(stmt, 1, query.c_str(), -1, SQLITE_STATIC);

    std::cout << "Результаты поиска для запроса: " << query << std::endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* content = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::cout << "ID: " << id << std::endl;
        std::cout << "Содержание: " << (content ? content : "NULL") << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
    sqlite3_finalize(stmt);
}



