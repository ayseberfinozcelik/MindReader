#include "mind_reader_logic.hpp"

namespace mind_reader {

int next_guess(int low, int high) {
    return low + (high - low) / 2;
}

bool is_valid_hint(char hint) {
    return hint == 'b' || hint == 'k' || hint == 'd';
}

Range apply_hint(int low, int high, int guess, char hint) {
    if (hint == 'b') {
        low = guess + 1;
    } else if (hint == 'k') {
        high = guess - 1;
    }
    return {low, high};
}

}  // namespace mind_reader
