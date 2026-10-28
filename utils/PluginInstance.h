//
// Created by ws on 2026/10/28.
//

#ifndef CODATA_PLUGININSTANCE_H
#define CODATA_PLUGININSTANCE_H
#include "../CdPlugin/Center.h"


class PluginInstance : public CenterInstance{
public:
    void log(std::string output) const override;
    void setBranch(Branch& branch) const override;
};


#endif //CODATA_PLUGININSTANCE_H
