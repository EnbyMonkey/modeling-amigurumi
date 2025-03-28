#include "Pattern.h"

/** Constructor **/
Pattern::Pattern()
{

}
Pattern::~Pattern() {}

std::vector<Row*> Pattern::get_rows() { return rows; }

Pattern* Pattern::init_pattern() {
    // Read pattern file
    std::ifstream pattern_file;
    std::string pattern_path = "../code/include/pattern.txt";
    std::string row_text;
    int row_num = 1;
    bool mc_first = false;

    pattern_file.open(pattern_path);

    // Get actions for each row
    while (std::getline(pattern_file, row_text)) {
        std::vector<Action*> row_actions = extract_actions(row_text);
        Row* new_row = new Row(row_num, row_actions);
        rows.push_back(new_row);

        // Quick first two row checks for now
        // Check if first row is mc
        if(row_num == 1 && row_actions.size() == 1 && row_actions[0]->get_type() == Action_type::mc) {
            mc_first = true;
        }
        // If first is mc and second (n)sc, change second to sc (n-1)sc_back
        if(row_num == 2 && mc_first) {
            bool all_sc = true;
            for(const auto action : row_actions) {
                if(action->get_type() != Action_type::sc) {
                    all_sc = false;
                    break;
                }
            }
            if(all_sc) {
                for(int a = 1; a < row_actions.size(); ++a) {
                    row_actions[a]->set_type(Action_type::sc_back);
                }
            }
        }

        row_num++;
    }

    pattern_file.close();
    return this;
}

// Print out the pattern actions row by row
void Pattern::print_pattern() {
    // Print each row
    for(const auto row : rows) {
        // Print row label and action #
        std::cout << std::endl << std::endl << "Row " << std::to_string(row->get_label()) << ": (" << std::to_string(row->get_actions().size()) << ")" << std::endl;

        for(const auto action : row->get_actions()) {
            switch(action->get_type()) {
                case Action_type::mc:
                    std::cout << "mc ";
                    break;
                case Action_type::sc:
                    std::cout << "sc ";
                    break;
                case Action_type::sc_back:
                    std::cout << "sc_back ";
                    break;
                case Action_type::sc_forward:
                    std::cout << "sc_forward ";
                    break;
                case Action_type::fo:
                    std::cout << "fo ";
                    break;
            }
        }
    }
}


// Get all actions in a string by breaking it down into blocks
std::vector<Action*> Pattern::extract_actions(std::string text) {
    std::regex block_finder("\\d*[a-z]+|\\d*\\[.+?\\]");
    std::regex has_bracket("\\d*\\[.+");
    std::regex inside_bracket("(?=\\[).+(?=\\])");
    std::regex multiplier("\\d+(?=\\[)");
    std::smatch match;
    std::smatch bracket_match;

    std::vector<Action*> new_actions;

    // Keep looking for blocks until none left
    while(regex_search(text, match, block_finder)) {
        std::string match_text = match.str(0);

        // Check if bracketed block
        if(regex_match(match_text, has_bracket)) {
            // Get bracket multiplier
            regex_search(match_text, bracket_match, multiplier);
            int multiplier = stoi(bracket_match.str(0));

            // Get blocks inside bracket
            regex_search(match_text, bracket_match, inside_bracket);
            std::string inside_text = bracket_match.str(0);
            std::vector<Action*> bracket_actions = extract_actions(inside_text);

            for(int i = 0; i < multiplier; ++i) {
                new_actions.insert(new_actions.end(), bracket_actions.begin(), bracket_actions.end());
            }
        }
        else { // Non-bracket, single block
            std::vector<Action*> actions = create_actions(match_text);
            // Probably a better way to do this
            new_actions.insert(new_actions.end(), actions.begin(), actions.end());
        }

        // Remove current block from search string
        text = match.suffix().str();
    }
    return new_actions;
}


// Turn a single block into actions
std::vector<Action*> Pattern::create_actions(std::string text) {
    std::regex multiplier("\\d+"); // Digits
    std::regex elem("[a-zA-Z]+"); // Letters
    std::smatch match;
    int times = 1;
    std::string type_string;
    enum Action_type type;
    std::vector<Action*> new_actions;

    // Check for multiplier
    bool hasNum = regex_search(text, match, multiplier);
    if(hasNum) times = stoi(match.str(0));

    // Get stitch type
    regex_search(text, match, elem);
    type_string = match.str(0);

    // Set type enum
    if(type_string == "sc") {
        type = Action_type::sc;
    }
    else if(type_string == "mc") {
        type = Action_type::mc;
    }
    else if(type_string == "inc") {
        type = Action_type::sc_back;
    }
    else if(type_string == "dec") {
        type = Action_type::sc_forward;
    }
    else if(type_string == "fo") {
        type = Action_type::fo;
    }

    // Create the action multiplier times
    for(int i = 0; i < times; ++i) {
        // todo: check if stitch & make stitch object
        // If an inc or a dec add an sc first
        if(type == Action_type::sc_back || type == Action_type::sc_forward) {
            new_actions.push_back(new Action(Action_type::sc));
        }
        new_actions.push_back(new Action(type));
    }

    return new_actions;
}
