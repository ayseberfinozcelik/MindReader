#ifndef MIND_READER_LOGIC_HPP
#define MIND_READER_LOGIC_HPP

#include <stdexcept>

namespace mind_reader {

struct Range {
    int low;
    int high;
};

struct HintCheckResult {
    char hint;
    bool needs_third;
};

int next_guess(int low, int high);
bool is_valid_hint(char hint);
Range update_range(int low, int high, int guess, char hint);
HintCheckResult verify_two_hints(char first, char second, bool lie_already_used);
char resolve_third_hint(char first, char second, char third);

}  // namespace mind_reader

#endif
