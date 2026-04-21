#include "mind_reader_logic.hpp"

namespace mind_reader {

int next_guess(int low, int high) {
    return low + (high - low) / 2;
}

bool is_valid_hint(char hint) {
    return hint == 'y' || hint == 'a' || hint == 'e';
}

Range update_range(int low, int high, int guess, char hint) {
    if (!is_valid_hint(hint)) {
        throw std::invalid_argument("Gecersiz ipucu. Sadece y/a/e kullan.");
    }

    if (hint == 'y') {
        low = guess + 1;
    } else if (hint == 'a') {
        high = guess - 1;
    }

    if (low > high) {
        throw std::logic_error("Tutarsiz ipucu: Bu aralikta sayi kalmadi.");
    }

    return {low, high};
}

}  // namespace mind_reader
