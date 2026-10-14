//
// Created by ws on 2026/10/12.
//

#ifndef CODATAP_CORE_H
#define CODATAP_CORE_H
#include <qstring.h>

#include "Branch.h"
#include "Event.h"
#include "SEvent.h"


class Core {
    public:
    static Branch branch; // root of commands
    static SEvent<> OnStart, OnStop;

    static QString ToQString(std::string text);
    static std::string ToString(QString text);
    static  std::vector<std::string> Split(const std::string& str, char delimiter);
};


#endif //CODATAP_CORE_H
