#include "move_semantics_lib.h"
#include <utility>
#include <chrono>

namespace MoveSemantics {
    
    // Constructor
    MoveDemo::MoveDemo(const std::string& name, const std::vector<int>& data, int ptr_value) 
        : name_(name), data_(data), ptr_(std::make_unique<int>(ptr_value)) {
        std::cout << "  [Constructor] Created '" << name_ << "' with " << data_.size() << " elements\n";
    }
    
    // Copy constructor
    MoveDemo::MoveDemo(const MoveDemo& other) 
        : name_(other.name_), data_(other.data_), 
          ptr_(other.ptr_ ? std::make_unique<int>(*other.ptr_) : nullptr) {
        std::cout << "  [Copy Constructor] Copied '" << name_ << "' with " << data_.size() << " elements\n";
    }
    
    // Move constructor
    MoveDemo::MoveDemo(MoveDemo&& other) noexcept 
        : name_(std::move(other.name_)), data_(std::move(other.data_)), ptr_(std::move(other.ptr_)) {
        std::cout << "  [Move Constructor] Moved from '" << other.name_ << "' (now empty)\n";
        other.name_ = "[moved-from]";
    }
    
    // Copy assignment operator
    MoveDemo& MoveDemo::operator=(const MoveDemo& other) {
        if (this != &other) {
            name_ = other.name_;
            data_ = other.data_;
            ptr_ = other.ptr_ ? std::make_unique<int>(*other.ptr_) : nullptr;
            std::cout << "  [Copy Assignment] Copied '" << name_ << "' with " << data_.size() << " elements\n";
        }
        return *this;
    }
    
    // Move assignment operator
    MoveDemo& MoveDemo::operator=(MoveDemo&& other) noexcept {
        if (this != &other) {
            name_ = std::move(other.name_);
            data_ = std::move(other.data_);
            ptr_ = std::move(other.ptr_);
            std::cout << "  [Move Assignment] Moved from '" << other.name_ << "' (now empty)\n";
            other.name_ = "[moved-from]";
        }
        return *this;
    }
    
    // Destructor
    MoveDemo::~MoveDemo() {
        std::cout << "  [Destructor] Destroying '" << name_ << "'\n";
    }
    
    // Print current state
    void MoveDemo::print_state() const {
        std::cout << "  Object '" << name_ << "': " 
                  << data_.size() << " elements, "
                  << (ptr_ ? "valid pointer" : "null pointer") << "\n";
    }
    
    // Check if object has been moved from
    bool MoveDemo::is_moved_from() const {
        return name_ == "[moved-from]" || (data_.empty() && !ptr_);
    }
    
    // Factory function
    MoveDemo create_demo(const std::string& name) {
        std::cout << "Creating demo object in factory function...\n";
        std::vector<int> data = {1, 2, 3, 4, 5};
        return MoveDemo(name, data, 42);
    }
    
    // Function that takes parameter by value
    void process_demo(MoveDemo demo) {
        std::cout << "Processing demo object...\n";
        demo.print_state();
    }
    
    void demonstrate_copy_vs_move() {
        std::cout << "\n=== COPY vs MOVE DEMONSTRATION ===\n";
        
        std::cout << "\n1. Creating original object:\n";
        std::vector<int> data = {10, 20, 30, 40, 50};
        MoveDemo original("Original", data, 100);
        original.print_state();
        
        std::cout << "\n2. Copy construction:\n";
        MoveDemo copied = original;
        std::cout << "After copy - Original: ";
        original.print_state();
        std::cout << "After copy - Copy: ";
        copied.print_state();
        
        std::cout << "\n3. Move construction:\n";
        MoveDemo moved = std::move(original);
        std::cout << "After move - Original: ";
        original.print_state();
        std::cout << "After move - Moved: ";
        moved.print_state();
    }
    
    void demonstrate_move_constructor() {
        std::cout << "\n=== MOVE CONSTRUCTOR DEMONSTRATION ===\n";
        
        std::cout << "\nCreating object and moving it:\n";
        std::vector<int> data = {1, 2, 3, 4, 5};
        MoveDemo source("Source", data, 50);
        
        std::cout << "\nBefore move:\n";
        source.print_state();
        
        std::cout << "\nPerforming move:\n";
        MoveDemo destination = std::move(source);
        
        std::cout << "\nAfter move:\n";
        std::cout << "Source: ";
        source.print_state();
        std::cout << "Destination: ";
        destination.print_state();
    }
    
    void demonstrate_move_assignment() {
        std::cout << "\n=== MOVE ASSIGNMENT DEMONSTRATION ===\n";
        
        std::cout << "\nCreating two objects:\n";
        std::vector<int> data1 = {1, 2, 3};
        std::vector<int> data2 = {4, 5, 6, 7};
        MoveDemo obj1("Object1", data1, 10);
        MoveDemo obj2("Object2", data2, 20);
        
        std::cout << "\nBefore move assignment:\n";
        obj1.print_state();
        obj2.print_state();
        
        std::cout << "\nPerforming move assignment (obj1 = std::move(obj2)):\n";
        obj1 = std::move(obj2);
        
        std::cout << "\nAfter move assignment:\n";
        std::cout << "obj1: ";
        obj1.print_state();
        std::cout << "obj2: ";
        obj2.print_state();
    }
    
    void demonstrate_rvalue_references() {
        std::cout << "\n=== RVALUE REFERENCES DEMONSTRATION ===\n";
        
        std::cout << "\nFunction returning by value (RVO/move):\n";
        MoveDemo demo = create_demo("Factory");
        demo.print_state();
        
        std::cout << "\nPassing by value (move):\n";
        process_demo(std::move(demo));
        
        std::cout << "\nAfter passing to function:\n";
        demo.print_state();
    }
    
    void demonstrate_std_move() {
        std::cout << "\n=== STD::MOVE DEMONSTRATION ===\n";
        
        std::cout << "\nCreating vector and moving it:\n";
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::cout << "Original vector size: " << vec.size() << "\n";
        
        std::vector<int> moved_vec = std::move(vec);
        std::cout << "After std::move:\n";
        std::cout << "Original vector size: " << vec.size() << "\n";
        std::cout << "Moved vector size: " << moved_vec.size() << "\n";
        
        std::cout << "\nNote: std::move doesn't actually move anything!\n";
        std::cout << "It just casts to rvalue reference, enabling move semantics.\n";
    }
    
    template<typename T>
    void perfect_forward_demo(T&& value) {
        std::cout << "Forwarding value...\n";
        process_demo(std::forward<T>(value));
    }
    
    void demonstrate_perfect_forwarding() {
        std::cout << "\n=== PERFECT FORWARDING DEMONSTRATION ===\n";
        
        std::cout << "\nCreating object for forwarding:\n";
        std::vector<int> data = {7, 8, 9};
        MoveDemo demo("Forwarded", data, 77);
        
        std::cout << "\nPerfect forwarding with std::move:\n";
        perfect_forward_demo(std::move(demo));
        
        std::cout << "\nAfter perfect forwarding:\n";
        demo.print_state();
    }
    
    void run_all_demonstrations() {
        std::cout << "========================================\n";
        std::cout << "    C++ MOVE SEMANTICS EDUCATIONAL DEMO\n";
        std::cout << "========================================\n";
        
        demonstrate_copy_vs_move();
        demonstrate_move_constructor();
        demonstrate_move_assignment();
        demonstrate_rvalue_references();
        demonstrate_std_move();
        demonstrate_perfect_forwarding();
        
        std::cout << "\n========================================\n";
        std::cout << "         DEMONSTRATIONS COMPLETE\n";
        std::cout << "========================================\n";
        
        std::cout << "\nKey Takeaways:\n";
        std::cout << "• Move semantics transfer ownership instead of copying\n";
        std::cout << "• Moved-from objects are in a valid but unspecified state\n";
        std::cout << "• std::move is just a cast to rvalue reference\n";
        std::cout << "• Move operations should be noexcept when possible\n";
        std::cout << "• Perfect forwarding preserves value categories\n";
    }
}
