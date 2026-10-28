//
// Created by ws on 2026/10/12.
//

#include "Core.h"

#include <qstring.h>
#include <sstream>

#include "Sets.h"

using string = std::string;

Branch Core::branch = Branch ("codata")
    .AddBranch(Branch("exit")
        .AddExecution([](const CommandArgument&) -> std::string {
            QApplication::quit();
            return "exit";
            })
        )
    .AddBranch(Branch("user")
        .AddArgument("name")
        .AddExecution([](CommandArgument a)-> string {
            string name;
            if (!a.TryGet("name", name))  return "The argument name is not exists";
            Sets::SetUser(name);
            return "Success";
        })
        )
;

SEvent<> Core::OnStart;
SEvent<> Core::OnStop;

MainWindow* Core::mainWindow = nullptr;

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
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    // 即便空格在末尾，此时 start 正好等于 str.length()
    // str.substr(start) 会正确返回一个空串 "" 并加入 vector
    result.push_back(str.substr(start));

    return result;
}

std::string Core::ToString(int n) {
    return std::to_string(n);
}

std::string Core::ToString(bool n) {
    return n ? "true" : "false";
}
