//
// Created by Rafael
//

#include "NoteManager.h"
#include "NoteSearchManager.h"
#include <iostream>

NoteManager::NoteManager(DatabaseManager &dbMgr) : dbManager(dbMgr) {
    // Подготавливаем  выражение для добавления заметки
    std::string sqlInsert  =  "INSERT INTO NOTES (USER, DATE,  CONTENT)  VALUES  (?,  ?, ?);";
    int rc = sqlite3_prepare_v2(dbManager.getDB(), sqlInsert.c_str(), -1, &stmtInsertNote, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка подготовки выражения для добавления заметки: " << sqlite3_errmsg(dbManager.getDB()) << std::endl;
    }

    // Подготавливаем выражение для удаления заметки
    std::string sqlDelete = "DELETE FROM NOTES WHERE ID = ?;";
    rc = sqlite3_prepare_v2(dbManager.getDB(), sqlDelete.c_str(), -1, &stmtDeleteNote, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка подготовки выражения для удаления заметки: " << sqlite3_errmsg(dbManager.getDB()) << std::endl;
    }
}

// Освобождение подготовленных выражений в деструкторе
NoteManager::~NoteManager() {
    sqlite3_finalize(stmtInsertNote);
    sqlite3_finalize(stmtDeleteNote);
}
void NoteManager::addNote(const Note &note) {
    // Сброс параметров перед новым использованием
    sqlite3_reset(stmtInsertNote);

    // Привязываем параметры
    sqlite3_bind_text(stmtInsertNote, 1, note.date.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtInsertNote, 2, note.user.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtInsertNote, 3, note.content.c_str(), -1, SQLITE_STATIC);

    // Выполняем подготовленное выражение
    int rc = sqlite3_step(stmtInsertNote);
    if (rc != SQLITE_DONE){
        std::cerr << "Ошибка выполнения: " << sqlite3_errmsg(dbManager.getDB()) << std::endl;
    } else {
        // Получаем ID последней добавленной записи
        int lastId = sqlite3_last_insert_rowid(dbManager.getDB());

        // Добавляем запись в FTS таблицу для поиска
        NoteSearchManager searchManager(dbManager);
        searchManager.insertNoteForSearch(lastId, note.content);

        std::cout << "Заметка успешно добавлена" << std::endl;
    }
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
            std::cout << azColName[i] << ":" << (argv[i] ? argv[i] : "NULL") << std::endl;
        }
        std::cout << "---------------------------------------" << std::endl;
        return 0;
    };

    // Выполняем SQL-запрос для получения всех записей из таблицы NOTES
    int rc = sqlite3_exec(db, sqlSelect, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Ошибка SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    // Не нужно финализировать выражение, так как мы его будем использовать повторно
}

void NoteManager::deleteNoteById(int id) {
    sqlite3_reset(stmtDeleteNote); // Сброс для повторного использования
    sqlite3_bind_int(stmtDeleteNote, 1, id); // Привязываем ID

    int rc = sqlite3_step(stmtDeleteNote);
    if (rc != SQLITE_DONE) {
        std::cerr << "Ошибка удаления заметки: " << sqlite3_errmsg(dbManager.getDB()) << std::endl;
    } else {
        std::cout << "Заметка успешно удалена" << std::endl;
    }
}
