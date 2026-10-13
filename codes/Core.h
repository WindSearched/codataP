//
// Created by ws on 2026/10/12.
//

#ifndef CODATAP_CORE_H
#define CODATAP_CORE_H
#include "Branch.h"
#include "Event.h"


class Core {
    public:
    static Branch branch; // root of commands
    static Event<void> OnStart, OnStop;

};


#endif //CODATAP_CORE_H
