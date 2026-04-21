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

}  // namespace mind_reader
