//
// Created by Rafael
//

#ifndef NOTESEARCHMANAGER_H
#define NOTESEARCHMANAGER_H

#include "DatabaseManager.h"
#include <string>

class NoteSearchManager {
private:
    DatabaseManager& dbManager;
    sqlite3_stmt* stmtSearchNote; // Храним подготовленное выражение для поиска заметки
    sqlite3_stmt* stmtInsertNoteForSearch; // Храним подготовленное выражение для вставки заметки в FTS
    sqlite3_stmt* stmtDeleteFTSNote; // Подготовленное выражение для удаления заметки из FTS

public:
    NoteSearchManager(DatabaseManager& dbMgr);
    ~NoteSearchManager();
    //void createFTSTable();
    void insertNoteForSearch(int id, const std::string& content);
    void searchNotes(const std::string& query);
    void deleteNoteFromFTS(int id);
};


#endif //NOTESEARCHMANAGER_H