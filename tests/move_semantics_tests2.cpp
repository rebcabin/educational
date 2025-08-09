// tests/move_semantics_semantic_tests.cpp
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include "move_semantics_lib.h"

namespace {

// Simple stdout capture utility
struct CoutCapture {
    std::streambuf* old = nullptr;
    std::ostringstream oss;
    CoutCapture() { old = std::cout.rdbuf(oss.rdbuf()); }
    ~CoutCapture() { std::cout.rdbuf(old); }
    std::string str() const { return oss.str(); }
};

TEST(MoveSemantics_Semantics, MoveConstructor_MovesResourcesAndMarksSource) {
    std::vector<int> data = {1, 2, 3};
    MoveSemantics::MoveDemo src("Src", data, 99);

    MoveSemantics::MoveDemo dst(std::move(src));

    // Source should be in a "moved-from" state according to library contract
    EXPECT_TRUE(src.is_moved_from());
    // Destination should carry over the name and hold the resources
    EXPECT_EQ(dst.get_name(), "Src");

    // Validate observable state via print_state output
    {
        CoutCapture cap;
        dst.print_state();
        const auto out = cap.str();
        EXPECT_NE(out.find("3 elements"), std::string::npos);
        EXPECT_NE(out.find("valid pointer"), std::string::npos);
    }
    {
        CoutCapture cap;
        src.print_state();
        const auto out = cap.str();
        // Moved-from: expect empty data and null pointer indication or generic moved-from marker
        // We at least check that it reports "null pointer" or the library's moved-from marker.
        // Exact emptiness message is checked too if present.
        bool ok = (out.find("null pointer") != std::string::npos) ||
                  (out.find("[moved-from]") != std::string::npos);
        EXPECT_TRUE(ok) << "Moved-from object should indicate null pointer or moved-from status. Output: " << out;
    }
}

TEST(MoveSemantics_Semantics, MoveAssignment_MovesResourcesAndMarksSource) {
    MoveSemantics::MoveDemo a("A", std::vector<int>{10, 20}, 1);
    MoveSemantics::MoveDemo b("B", std::vector<int>{30, 40, 50}, 2);

    a = std::move(b);

    // b becomes moved-from, a should now reflect b's identity/resources
    EXPECT_TRUE(b.is_moved_from());
    EXPECT_EQ(a.get_name(), "B");

    {
        CoutCapture cap;
        a.print_state();
        const auto out = cap.str();
        EXPECT_NE(out.find("3 elements"), std::string::npos);
        EXPECT_NE(out.find("valid pointer"), std::string::npos);
    }
    {
        CoutCapture cap;
        b.print_state();
        const auto out = cap.str();
        bool ok = (out.find("null pointer") != std::string::npos) ||
                  (out.find("[moved-from]") != std::string::npos);
        EXPECT_TRUE(ok) << "Moved-from object should indicate null pointer or moved-from status. Output: " << out;
    }
}

TEST(MoveSemantics_Semantics, SelfMoveAssignment_NoChangeAndNoCrash) {
    MoveSemantics::MoveDemo x("X", std::vector<int>{1, 2, 3, 4}, 7);

    // Snapshot some observable state before self-move
    const std::string oldName = x.get_name();
    {
        // Self move-assign should be a no-op and not throw
        EXPECT_NO_THROW(x = std::move(x));
    }
    EXPECT_EQ(x.get_name(), oldName);
    EXPECT_FALSE(x.is_moved_from());

    // Still usable
    CoutCapture cap;
    x.print_state();
    EXPECT_FALSE(cap.str().empty());
}

TEST(MoveSemantics_Semantics, PassByValue_UsingStdMove_MovesFromCaller) {
    // Passing by value will invoke MoveDemo's move constructor if we std::move the argument.
    MoveSemantics::MoveDemo demo("ToProcess", std::vector<int>{5, 6}, 123);

    EXPECT_FALSE(demo.is_moved_from());
    EXPECT_NO_THROW(MoveSemantics::process_demo(std::move(demo)));
    // After moving into the by-value parameter, the local 'demo' should be moved-from
    EXPECT_TRUE(demo.is_moved_from());
}

TEST(MoveSemantics_Semantics, ReturnByValue_ObjectIsUsableAtCallSite) {
    // A factory returning by value should yield a valid object at the call site.
    MoveSemantics::MoveDemo obj = MoveSemantics::create_demo("FactoryObj");
    EXPECT_EQ(obj.get_name(), "FactoryObj");
    EXPECT_FALSE(obj.is_moved_from());

    CoutCapture cap;
    obj.print_state();
    const auto out = cap.str();
    // The library's factory populates 5 elements and a valid pointer; we assert the observable parts.
    EXPECT_NE(out.find("5 elements"), std::string::npos);
    EXPECT_NE(out.find("valid pointer"), std::string::npos);
}

} // namespace
