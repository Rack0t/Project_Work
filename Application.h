//
// Created by Rafael
//

#ifndef NOTE_TASK_JOB_APPLICATION_H
#define NOTE_TASK_JOB_APPLICATION_H

#include "NoteManager.h"

class Application {
private:
    DatabaseManager dbManager;
    NoteManager noteManager;

public:
    Application();
    void run();
    void addNewNote();
    void deleteNote();
};

#endif //NOTE_TASK_JOB_APPLICATION_H
