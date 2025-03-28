#include "Row.h"

/** Constructor **/
Row::Row(int label, std::vector<Action*> actions) :
    label(label),
    actions(actions)
{
    
}
Row::~Row() {}



std::vector<Action*> Row::get_actions() 
{ 
    return actions; 
}
int Row::get_label() 
{ 
    return label; 
}
