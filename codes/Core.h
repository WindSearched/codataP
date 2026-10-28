//
// Created by ws on 2026/10/12.
//

#ifndef CODATAP_CORE_H
#define CODATAP_CORE_H
#include <QApplication>
#include <qstring.h>

#include "../CdPlugin/codes/Branch.h"
#include "Event.h"
#include "MainWindow.h"
#include "SEvent.h"


class Core {
    public:
    static Branch branch; // root of commands
    static SEvent<> OnStart, OnStop;

    static MainWindow* mainWindow;

    static QString ToQString(std::string text);
    static std::string ToString(QString text);
    static  std::vector<std::string> Split(const std::string& str, char delimiter);
    static std::string ToString(int n);
    static std::string ToString(bool n);
};


#endif //CODATAP_CORE_H
