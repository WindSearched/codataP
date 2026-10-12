#include <iostream>

#include "codes/Branch.h"
#include "codes/Core.h"
#include "codes/Event.h"
#include "codes/Tube.h"

int main() {
    static Tube<std::string> tube;
    static Event<void> OnStart, OnStop;
    static Branch branch("Codata");

    branch.AddBranch(
        Branch{"test"}
            .AddExecution([](CommandArgument args)-> std::string{
                return "d";
            })
        );

    std::string cmd ;
    std::getline(std::cin, cmd);

    CommandArgument args;
    Branch b =  branch.Parse({cmd},  args);
    std::cout << b.Run(args);

}