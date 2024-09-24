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

public:
    NoteSearchManager(DatabaseManager& dbMgr);
    void createFTSTable();
    void insertNoteForSearch(int id, const std::string& content);
    void searchNotes(const std::string& query);
};



#endif //NOTESEARCHMANAGER_H
