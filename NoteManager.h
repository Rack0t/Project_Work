//
// Created by Rafael
//

#ifndef NOTE_TASK_JOB_NOTEMANAGER_H
#define NOTE_TASK_JOB_NOTEMANAGER_H

#include "DatabaseManager.h"
#include "Note.h"

class NoteManager {
private:
    DatabaseManager& dbManager;

public:
    NoteManager(DatabaseManager& dbMgr);
    void addNote(const Note& note);
    void viewAllNotes();
    void deleteNoteById(int id);
};


#endif //NOTE_TASK_JOB_NOTEMANAGER_H
