#pragma once

#include "defs.h"

class Action {
public:
    Action();
    Action(enum Action_type type);
    ~Action();
    enum Action_type get_type();
    void set_type(enum Action_type new_type);

private:
    enum Action_type type;
};
