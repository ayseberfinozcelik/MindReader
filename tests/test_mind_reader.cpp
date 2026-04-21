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
    mind_reader::Range r = mind_reader::update_range(1, 1000, 500, 'b');
    expect_true(r.low == 501 && r.high == 1000, "higher hint should move low to guess + 1");
}

void test_update_range_for_lower_hint() {
    mind_reader::Range r = mind_reader::update_range(1, 1000, 500, 'k');
    expect_true(r.low == 1 && r.high == 499, "lower hint should move high to guess - 1");
}

void test_update_range_for_exact_hint() {
    mind_reader::Range r = mind_reader::update_range(1, 1000, 500, 'd');
    expect_true(r.low == 1 && r.high == 1000, "exact hint should keep range unchanged");
}

void test_inconsistent_hints_raise_error() {
    bool threw = false;
    try {
        (void)mind_reader::update_range(500, 500, 500, 'b');
    } catch (const std::logic_error&) {
        threw = true;
    }
    expect_true(threw, "inconsistent hint should throw logic_error");
}

void test_verify_two_hints_returns_direct_hint_when_matching() {
    mind_reader::HintCheckResult result = mind_reader::verify_two_hints('b', 'b', false);
    expect_true(result.needs_third == false, "matching hints should not need a third answer");
    expect_true(result.hint == 'b', "matching hints should resolve to that hint");
}

void test_verify_two_hints_requests_third_when_different() {
    mind_reader::HintCheckResult result = mind_reader::verify_two_hints('b', 'k', false);
    expect_true(result.needs_third == true, "different hints should need a third answer");
}

void test_verify_two_hints_throws_when_lie_budget_used() {
    bool threw = false;
    try {
        (void)mind_reader::verify_two_hints('b', 'k', true);
    } catch (const std::logic_error&) {
        threw = true;
    }
    expect_true(threw, "second mismatch after lie usage should throw logic_error");
}

void test_resolve_third_hint_uses_majority() {
    char hint = mind_reader::resolve_third_hint('b', 'k', 'b');
    expect_true(hint == 'b', "majority hint should win in 3-way validation");
}

void test_resolve_third_hint_throws_on_three_way_mismatch() {
    bool threw = false;
    try {
        (void)mind_reader::resolve_third_hint('b', 'k', 'd');
    } catch (const std::logic_error&) {
        threw = true;
    }
    expect_true(threw, "all-different answers should throw logic_error");
}

}  // namespace

int main() {
    test_next_guess_returns_midpoint();
    test_update_range_for_higher_hint();
    test_update_range_for_lower_hint();
    test_update_range_for_exact_hint();
    test_inconsistent_hints_raise_error();
    test_verify_two_hints_returns_direct_hint_when_matching();
    test_verify_two_hints_requests_third_when_different();
    test_verify_two_hints_throws_when_lie_budget_used();
    test_resolve_third_hint_uses_majority();
    test_resolve_third_hint_throws_on_three_way_mismatch();

    if (failures > 0) {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }

    std::cout << "All tests passed.\n";
    return 0;
}
