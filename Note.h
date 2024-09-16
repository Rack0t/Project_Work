//
// Created by Rafael
//

#ifndef NOTE_TASK_JOB_NOTE_H
#define NOTE_TASK_JOB_NOTE_H

#include <string>

class Note {
public:
    std::string date;
    std::string user;
    std::string content;

    Note(const std::string& content);
    static std::string getCurrentDateTime();
    static std::string getUserName();
};


#endif //NOTE_TASK_JOB_NOTE_H
