#include "Action.h"

/** Constructor **/
Action::Action() {}
Action::Action(enum Action_type type) :
    type(type)
{
    
}
Action::~Action() {}

enum Action_type Action::get_type() 
{ 
    return type; 
}

void Action::set_type(enum Action_type new_type)
{
    type = new_type;
}