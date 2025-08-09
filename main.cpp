#include <iostream>
#include "move_semantics_lib.h"

int main() {
    std::cout << "Hello and welcome to C++ Move Semantics Educational Demo!\n\n";
    
    // Run all move semantics demonstrations
    MoveSemantics::run_all_demonstrations();
    
    std::cout << "\nAdditional Interactive Demo:\n";
    std::cout << "============================\n";
    
    // Create a simple interactive example
    std::vector<int> data = {100, 200, 300, 400, 500};
    MoveSemantics::MoveDemo example("Interactive", data, 999);
    
    std::cout << "\nCreated example object:\n";
    example.print_state();
    
    std::cout << "\nMoving example to new object:\n";
    MoveSemantics::MoveDemo moved_example = std::move(example);
    
    std::cout << "\nFinal states:\n";
    std::cout << "Original: ";
    example.print_state();
    std::cout << "Moved: ";
    moved_example.print_state();
    
    return 0;
}