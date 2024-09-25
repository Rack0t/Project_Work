//
// Created by Rafael
//

#include "NoteSearchManager.h"
#include <iostream>

// Конструктор

NoteSearchManager::NoteSearchManager(DatabaseManager &dbMgr) : dbManager(dbMgr) {
    // Подготавливаем выражение для поиска заметок
    std::string sqlSearch = "SELECT ID, CONTENT FROM NOTES_FTS WHERE CONTENT LIKE '%' || ? || '%';";
    int rc = sqlite3_prepare_v2(dbManager.getDB(), sqlSearch.c_str(), -1, &stmtSearchNote, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка подготовки выражения для поиска заметок: " << sqlite3_errmsg(dbManager.getDB()) << std::endl;
    }

    // Подготавливаем выражение для вставки заметки в FTS таблицу
    std::string sqlInsertNote = "INSERT INTO NOTES_FTS (ID, CONTENT) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(dbManager.getDB(), sqlInsertNote.c_str(), -1, &stmtInsertNoteForSearch, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(dbManager.getDB()) << std::endl;
    }

}

// Освобождение ресурсов
NoteSearchManager::~NoteSearchManager() {
    sqlite3_finalize(stmtSearchNote);
    sqlite3_finalize(stmtInsertNoteForSearch);
}


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
    sqlite3_reset(stmtInsertNoteForSearch);

    //Привязываем параметры
    sqlite3_bind_int(stmtInsertNoteForSearch, 1, id);
    sqlite3_bind_text(stmtInsertNoteForSearch, 2, content.c_str(), -1, NULL);

    int rc = sqlite3_step(stmtInsertNoteForSearch);
    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка добавления заметки в FTS таблицу: " << sqlite3_errmsg(dbManager.getDB()) << std::endl;
    } else {
        std::cout << "Заметка успешно добавлена в FTS таблицу" << std::endl;
    }
}

// Поиск заметок по тексту
void NoteSearchManager::searchNotes(const std::string &query) {
    sqlite3_reset(stmtSearchNote); // Сброс выражения для повторного использования

    // Привязываем поисковый запрос
    sqlite3_bind_text(stmtSearchNote, 1, query.c_str(), -1, SQLITE_STATIC);

    std::cout << "Результаты поиска для запроса: " << query << std::endl;
    while (sqlite3_step(stmtSearchNote) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmtSearchNote, 0);
        const char* content = reinterpret_cast<const char*>(sqlite3_column_text(stmtSearchNote, 1));
        std::cout << "ID: " << id << std::endl;
        std::cout << "Содержание: " << (content ? content : "NULL") << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}
