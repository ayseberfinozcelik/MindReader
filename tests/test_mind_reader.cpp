#include <iostream>

#include "mind_reader_logic.hpp"

int failures = 0;

void check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        failures++;
    }
}

int main() {
    check(mind_reader::next_guess(1, 1000) == 500, "midpoint should be 500");
    check(mind_reader::next_guess(501, 1000) == 750, "midpoint should be 750");

    check(mind_reader::is_valid_hint('h'), "'h' should be valid");
    check(mind_reader::is_valid_hint('l'), "'l' should be valid");
    check(mind_reader::is_valid_hint('c'), "'c' should be valid");
    check(!mind_reader::is_valid_hint('u'), "'u' should not be a range hint");

    auto higher = mind_reader::apply_hint(1, 1000, 500, 'h');
    check(higher.low == 501 && higher.high == 1000, "'h' should increase low bound");

    auto lower = mind_reader::apply_hint(1, 1000, 500, 'l');
    check(lower.low == 1 && lower.high == 499, "'l' should decrease high bound");

    auto correct = mind_reader::apply_hint(1, 1000, 500, 'c');
    check(correct.low == 1 && correct.high == 1000, "'c' should keep bounds unchanged");

    if (failures) {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }

    std::cout << "All tests passed.\n";
    return 0;
}
