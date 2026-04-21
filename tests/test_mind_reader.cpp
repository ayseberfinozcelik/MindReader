#include <iostream>
#include <stdexcept>

#include "mind_reader_logic.hpp"

namespace {

int failures = 0;

void expect_true(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "HATA: " << message << "\n";
        failures++;
    }
}

void test_next_guess_returns_midpoint() {
    expect_true(mind_reader::next_guess(1, 1000) == 500, "next_guess(1, 1000) 500 olmalı");
    expect_true(mind_reader::next_guess(501, 1000) == 750, "next_guess(501, 1000) 750 olmalı");
}

void test_hint_validation() {
    expect_true(mind_reader::is_valid_hint('b'), "'b' geçerli olmalı");
    expect_true(mind_reader::is_valid_hint('k'), "'k' geçerli olmalı");
    expect_true(mind_reader::is_valid_hint('d'), "'d' geçerli olmalı");
    expect_true(!mind_reader::is_valid_hint('u'), "'u' mantık ipucunda geçerli olmamalı");
}

void test_update_range_for_higher_hint() {
    mind_reader::Range r = mind_reader::update_range(1, 1000, 500, 'b');
    expect_true(r.low == 501 && r.high == 1000, "b ipucu alt sınırı guess + 1 yapmalı");
}

void test_update_range_for_lower_hint() {
    mind_reader::Range r = mind_reader::update_range(1, 1000, 500, 'k');
    expect_true(r.low == 1 && r.high == 499, "k ipucu üst sınırı guess - 1 yapmalı");
}

void test_update_range_for_exact_hint() {
    mind_reader::Range r = mind_reader::update_range(1, 1000, 500, 'd');
    expect_true(r.low == 1 && r.high == 1000, "d ipucu aralığı değiştirmemeli");
}

void test_invalid_hint_throws_error() {
    bool threw = false;
    try {
        (void)mind_reader::update_range(1, 1000, 500, 'x');
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    expect_true(threw, "geçersiz ipucu invalid_argument fırlatmalı");
}

void test_inconsistent_hints_raise_error() {
    bool threw = false;
    try {
        (void)mind_reader::update_range(500, 500, 500, 'b');
    } catch (const std::logic_error&) {
        threw = true;
    }
    expect_true(threw, "tutarsız ipucu logic_error fırlatmalı");
}

}  // namespace

int main() {
    test_next_guess_returns_midpoint();
    test_hint_validation();
    test_update_range_for_higher_hint();
    test_update_range_for_lower_hint();
    test_update_range_for_exact_hint();
    test_invalid_hint_throws_error();
    test_inconsistent_hints_raise_error();

    if (failures > 0) {
        std::cerr << failures << " test başarısız.\n";
        return 1;
    }

    std::cout << "Tüm testler başarılı.\n";
    return 0;
}
