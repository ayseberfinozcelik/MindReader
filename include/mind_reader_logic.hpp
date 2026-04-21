#ifndef MIND_READER_LOGIC_HPP
#define MIND_READER_LOGIC_HPP

namespace mind_reader {

struct Range {
    int low;
    int high;
};

int next_guess(int low, int high);
bool is_valid_hint(char hint);
Range apply_hint(int low, int high, int guess, char hint);

}  // namespace mind_reader

#endif
