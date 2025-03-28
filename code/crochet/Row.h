#pragma once

#include "defs.h"
#include "Action.h"

class Row {
public:
    Row(int label, std::vector<Action*> actions);
    ~Row();

    std::vector<Action*> get_actions();
    int get_label();

private:
    int label;
    int num_stitches;
    std::vector<Action*> actions;
};
