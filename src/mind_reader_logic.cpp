#include "mind_reader_logic.hpp"

namespace mind_reader {

int next_guess(int low, int high) {
    return low + (high - low) / 2;
}

bool is_valid_hint(char hint) {
    return hint == 'b' || hint == 'k' || hint == 'd';
}

Range update_range(int low, int high, int guess, char hint) {
    if (!is_valid_hint(hint)) {
        throw std::invalid_argument("Gecersiz ipucu. Sadece b/k/d kullan.");
    }

    if (hint == 'b') {
        low = guess + 1;
    } else if (hint == 'k') {
        high = guess - 1;
    }

    if (low > high) {
        throw std::logic_error("Tutarsiz ipucu: Bu aralikta sayi kalmadi.");
    }

    return {low, high};
}

HintCheckResult verify_two_hints(char first, char second, bool lie_already_used) {
    if (!is_valid_hint(first) || !is_valid_hint(second)) {
        throw std::invalid_argument("Gecersiz ipucu. Sadece b/k/d kullan.");
    }

    if (first == second) {
        return {first, false};
    }

    if (lie_already_used) {
        throw std::logic_error(
            "Birden fazla tutarsiz ipucu algilandi. En fazla 1 kez yanlis ipucu verebilirsin.");
    }

    return {'\0', true};
}

char resolve_third_hint(char first, char second, char third) {
    if (!is_valid_hint(first) || !is_valid_hint(second) || !is_valid_hint(third)) {
        throw std::invalid_argument("Gecersiz ipucu. Sadece b/k/d kullan.");
    }

    if (first == second || first == third) {
        return first;
    }
    if (second == third) {
        return second;
    }

    throw std::logic_error(
        "Uc farkli cevap alindi. Bu, birden fazla yanlis ipucu verildigini gosteriyor.");
}

}  // namespace mind_reader
