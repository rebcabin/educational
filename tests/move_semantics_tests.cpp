// tests/move_semantics_tests.cpp
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <vector>
#include "../move_semantics_lib.h"

namespace {

struct CoutCapture {
    std::streambuf* old = nullptr;
    std::ostringstream oss;
    CoutCapture() { old = std::cout.rdbuf(oss.rdbuf()); }
    ~CoutCapture() { std::cout.rdbuf(old); }
    std::string str() const { return oss.str(); }
};

TEST(MoveSemantics, RunAllDemonstrations_PrintsHeader) {
    CoutCapture cap;
    EXPECT_NO_THROW(MoveSemantics::run_all_demonstrations());
    auto out = cap.str();
    EXPECT_NE(out.find("C++ MOVE SEMANTICS EDUCATIONAL DEMO"), std::string::npos);
    EXPECT_NE(out.find("DEMONSTRATIONS COMPLETE"), std::string::npos);
}

TEST(MoveSemantics, CopyVsMove_DoesNotThrowAndMentionsSections) {
    CoutCapture cap;
    EXPECT_NO_THROW(MoveSemantics::demonstrate_copy_vs_move());
    auto out = cap.str();
    EXPECT_NE(out.find("COPY vs MOVE DEMONSTRATION"), std::string::npos);
    EXPECT_NE(out.find("Copy Constructor"), std::string::npos) << "Should print something about copy";
    EXPECT_NE(out.find("Move Constructor"), std::string::npos) << "Should print something about move";
}

TEST(MoveSemantics, MoveConstructor_DoesNotThrow) {
    CoutCapture cap;
    EXPECT_NO_THROW(MoveSemantics::demonstrate_move_constructor());
    auto out = cap.str();
    EXPECT_NE(out.find("MOVE CONSTRUCTOR DEMONSTRATION"), std::string::npos);
}

TEST(MoveSemantics, MoveAssignment_DoesNotThrow) {
    CoutCapture cap;
    EXPECT_NO_THROW(MoveSemantics::demonstrate_move_assignment());
    auto out = cap.str();
    EXPECT_NE(out.find("MOVE ASSIGNMENT DEMONSTRATION"), std::string::npos);
}

TEST(MoveSemantics, RvalueReferences_DoesNotThrow) {
    CoutCapture cap;
    EXPECT_NO_THROW(MoveSemantics::demonstrate_rvalue_references());
    auto out = cap.str();
    EXPECT_NE(out.find("RVALUE REFERENCES DEMONSTRATION"), std::string::npos);
}

TEST(MoveSemantics, StdMove_PrintsExplanation) {
    CoutCapture cap;
    EXPECT_NO_THROW(MoveSemantics::demonstrate_std_move());
    auto out = cap.str();
    EXPECT_NE(out.find("STD::MOVE DEMONSTRATION"), std::string::npos);
    EXPECT_NE(out.find("std::move"), std::string::npos);
}

TEST(MoveSemantics, PerfectForwarding_DoesNotThrow) {
    CoutCapture cap;
    EXPECT_NO_THROW(MoveSemantics::demonstrate_perfect_forwarding());
    auto out = cap.str();
    EXPECT_NE(out.find("PERFECT FORWARDING DEMONSTRATION"), std::string::npos);
}

// A basic sanity test constructing and moving a MoveDemo to ensure API usability in tests.
TEST(MoveSemantics, MoveDemo_ConstructAndMove) {
    std::vector<int> data = {1, 2, 3};
    MoveSemantics::MoveDemo a("A", data, 42);
    // Move-construct
    MoveSemantics::MoveDemo b(std::move(a));
    // Just ensure we can call print_state() and the moved-from object remains valid
    CoutCapture cap;
    a.print_state();
    b.print_state();
    auto out = cap.str();
    EXPECT_FALSE(out.empty());
}

} // namespace
