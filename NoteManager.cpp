//
// Created by Rafael
//

#include "NoteManager.h"
#include "NoteSearchManager.h"
#include <iostream>

NoteManager::NoteManager(DatabaseManager &dbMgr) : dbManager(dbMgr) {}

void NoteManager::addNote(const Note &note) {
    sqlite3* db = dbManager.getDB();
    sqlite3_stmt* stmt;
    std::string sqlInsert = "INSERT INTO NOTES (DATE, USER, CONTENT) VALUES (?, ?, ?);";

    int rc = sqlite3_prepare_v2(db, sqlInsert.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK){
        std::cerr << "Не удалось подготовить запрос: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, note.date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, note.user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, note.content.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE){
        std::cerr << "Ошибка выполнения: " << sqlite3_errmsg(db) << "\n";
    } else {
        // Получаем ID последней добавленной записи
        int lastId = sqlite3_last_insert_rowid(db);

        // Добавляем запись в FTS таблицу для поиска
        NoteSearchManager searchManager(dbManager);
        searchManager.insertNoteForSearch(lastId, note.content);

        std::cout << "Заметка успешно добавлена\n";
    }

    sqlite3_finalize(stmt);
}

void NoteManager::viewAllNotes() {
    sqlite3* db = dbManager.getDB();
    const char* sqlSelect = "SELECT ID, DATE, USER, CONTENT FROM NOTES;";
    char* errMsg;

    // Callback-функция для обработки каждой строки, полученной из базы данных
    // argc - количество столбцов в результатах запроса
    // argv - массив значений столбцов текущей строки
    // azColName - массив с названиями столбцов
    auto callback = [](void* NotUsed, int argc, char** argv, char** azColName) -> int {
        for (int i = 0; i < argc; i++) {
            std::cout << azColName[i] << ":" << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        std::cout << "---------------------------------------\n";
        return 0;
    };

    // Выполняем SQL-запрос для получения всех записей из таблицы NOTES
    int rc = sqlite3_exec(db, sqlSelect, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка SQL: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
}

void NoteManager::deleteNoteById(int id) {
    sqlite3* db = dbManager.getDB();
    std::string sqlDelete = "DELETE FROM NOTES WHERE ID = ?;";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sqlDelete.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Не удалось подготовить запрос: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    // Привязываем ID к запросу
    sqlite3_bind_int(stmt, 1, id);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка удаления записи: " << sqlite3_errmsg(db) << "\n";
    } else {
        std::cout << "Запись успешно удалена\n";
    }
    sqlite3_finalize(stmt);
}
