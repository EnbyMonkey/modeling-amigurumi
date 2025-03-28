#pragma once

#include <regex>

#include <defs.h>
#include "Row.h"

class Pattern {
public:
    Pattern();
    ~Pattern();

    std::vector<Row*> get_rows();
    Pattern* init_pattern();
    void print_pattern(); // For testing

private:
    std::vector<Row*> rows;

    std::vector<Action*> extract_actions(std::string text);
    std::vector<Action*> create_actions(std::string text);
};
