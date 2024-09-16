//
// Created by Rafael
//

#include "Note.h"
#include <ctime>
#include <cstdlib>

#ifdef _WIN32
#define GET_USER_ENV "USERNAME"
#else
#define GET_USER_ENV "USER"
#endif

Note::Note(const std::string &content)
    : date(getCurrentDateTime()), user(getUserName()), content(content) {}

std::string Note::getCurrentDateTime() {
    time_t now = time(nullptr);
    char buf[80];
    struct tm* timeinfo = localtime(&now);
    strftime(buf, sizeof(buf),"%Y-%m-%d %H-%M-%S", timeinfo);
    return buf;
}

std::string Note::getUserName() {
    const char * user = std::getenv(GET_USER_ENV);
    if (user)
        return user;
    else
        return "Unknown";
}
