#ifndef MOVE_SEMANTICS_LIB_H
#define MOVE_SEMANTICS_LIB_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace MoveSemantics {
    
    // A simple class to demonstrate move semantics
    class MoveDemo {
    private:
        std::string name_;
        std::vector<int> data_;
        std::unique_ptr<int> ptr_;
        
    public:
        // Constructor
        MoveDemo(const std::string& name, const std::vector<int>& data, int ptr_value);
        
        // Copy constructor
        MoveDemo(const MoveDemo& other);
        
        // Move constructor
        MoveDemo(MoveDemo&& other) noexcept;
        
        // Copy assignment operator
        MoveDemo& operator=(const MoveDemo& other);
        
        // Move assignment operator
        MoveDemo& operator=(MoveDemo&& other) noexcept;
        
        // Destructor
        ~MoveDemo();
        
        // Utility methods
        void print_state() const;
        bool is_moved_from() const;
        const std::string& get_name() const { return name_; }
    };
    
    // Factory function that returns by value (to demonstrate RVO/move)
    MoveDemo create_demo(const std::string& name);
    
    // Function that takes parameter by value (to demonstrate move)
    void process_demo(MoveDemo demo);
    
    // Educational demonstrations
    void demonstrate_copy_vs_move();
    void demonstrate_move_constructor();
    void demonstrate_move_assignment();
    void demonstrate_rvalue_references();
    void demonstrate_std_move();
    void demonstrate_perfect_forwarding();
    
    // Main demonstration function
    void run_all_demonstrations();
}

#endif // MOVE_SEMANTICS_LIB_H
