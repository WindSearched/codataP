//
// Created by ws on 2026/10/12.
//

#include "Core.h"

#include <qstring.h>
#include <sstream>

Branch Core::branch ("codata");
SEvent<> Core::OnStart;
SEvent<> Core::OnStop;

QString Core::ToQString(std::string text) {
    QString qtext = QString::fromUtf8(
        text.data(),
        static_cast<qsizetype>(text.size())
    );
    return qtext;
}

std::string Core::ToString(QString text) {
    return  text.toStdString();
}

std::vector<std::string> Core::Split(const std::string &str, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}
