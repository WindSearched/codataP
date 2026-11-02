//
// Created by ws on 2026/10/28.
//

#ifndef CODATAP_INTERFACE_H
#define CODATAP_INTERFACE_H

#include "../codes/Sets.h"
#include "codes/Branch.h"


class CdaPI {
public:
    virtual std::string getName() const = 0;
    virtual std::string getAuthor() const = 0;
    virtual std::string getDescription();

    virtual void onStart();
    virtual void onEnd();

    Node GetYml(std::string path);
    static void SetBranch(Branch& branch);

    static void Log(std::string output);
};


#endif //CODATAP_INTERFACE_H
