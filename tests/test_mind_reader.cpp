#include <iostream>
#include <stdexcept>

#include "mind_reader_logic.hpp"

namespace {

int failures = 0;

void expect_true(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        failures++;
    }
}

void test_next_guess_returns_midpoint() {
    expect_true(mind_reader::next_guess(1, 1000) == 500, "next_guess(1, 1000) should be 500");
    expect_true(mind_reader::next_guess(501, 1000) == 750, "next_guess(501, 1000) should be 750");
}

void test_update_range_for_higher_hint() {
    mind_reader::Range r = mind_reader::update_range(1, 1000, 500, 'y');
    expect_true(r.low == 501 && r.high == 1000, "higher hint should move low to guess + 1");
}

void test_update_range_for_lower_hint() {
    mind_reader::Range r = mind_reader::update_range(1, 1000, 500, 'a');
    expect_true(r.low == 1 && r.high == 499, "lower hint should move high to guess - 1");
}

void test_update_range_for_exact_hint() {
    mind_reader::Range r = mind_reader::update_range(1, 1000, 500, 'e');
    expect_true(r.low == 1 && r.high == 1000, "exact hint should keep range unchanged");
}

void test_inconsistent_hints_raise_error() {
    bool threw = false;
    try {
        (void)mind_reader::update_range(500, 500, 500, 'y');
    } catch (const std::logic_error&) {
        threw = true;
    }
    expect_true(threw, "inconsistent hint should throw logic_error");
}

}  // namespace

int main() {
    test_next_guess_returns_midpoint();
    test_update_range_for_higher_hint();
    test_update_range_for_lower_hint();
    test_update_range_for_exact_hint();
    test_inconsistent_hints_raise_error();

    if (failures > 0) {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }

    std::cout << "All tests passed.\n";
    return 0;
}
