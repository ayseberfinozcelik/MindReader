#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "mind_reader_logic.hpp"

namespace {

constexpr int kLowerBound = 1;
constexpr int kUpperBound = 1000;
constexpr int kTheoreticalStepLimit = 11;

const char* logo() {
    return R"(
 __  __ _           _   ____                _
|  \/  (_)_ __   __| | |  _ \ ___  __ _  __| | ___ _ __
| |\/| | | '_ \ / _` | | |_) / _ \/ _` |/ _` |/ _ \ '__|
| |  | | | | | | (_| | |  _ <  __/ (_| | (_| |  __/ |
|_|  |_|_|_| |_|\__,_| |_| \_\___|\__,_|\__,_|\___|_|
)";
}

char first_non_space_char(const std::string& text) {
    for (char ch : text) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }
    }
    return '\0';
}

char read_choice(const std::string& prompt, const std::string& allowed) {
    while (true) {
        std::cout << prompt;
        std::string input;
        if (!std::getline(std::cin, input)) {
            return '\0';
        }

        char choice = first_non_space_char(input);
        if (allowed.find(choice) != std::string::npos) {
            return choice;
        }

        std::cout << "Invalid input. Please try again.\n";
    }
}

int randomized_guess(int low, int high, std::mt19937& rng, int previous_guess) {
    int midpoint = mind_reader::next_guess(low, high);
    int width = high - low + 1;
    if (width <= 3) {
        return midpoint;
    }

    int jitter = std::max(1, width / 6);
    std::uniform_int_distribution<int> dist(-jitter, jitter);
    int guess = std::clamp(midpoint + dist(rng), low, high);

    if (guess == previous_guess && low != high) {
        guess = (guess < high) ? guess + 1 : guess - 1;
    }
    return guess;
}

}  // namespace

int main() {
    std::cout << logo() << "\n";
    std::cout << "Think of a number between 1 and 1000. Do not tell me.\n";
    std::cout << "Press ENTER when you are ready: ";

    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cout << "\nInput closed. Game is ending.\n";
        return 0;
    }

    char mode = read_choice(
        "Mode: [1] Classic Binary Search, [2] Random Pivot (recommended): ",
        "12");
    if (mode == '\0') {
        std::cout << "\nInput closed. Game is ending.\n";
        return 0;
    }

    int low = kLowerBound;
    int high = kUpperBound;
    int steps = 0;
    int previous_guess = -1;
    std::vector<std::tuple<int, int, int, int>> history;

    std::random_device rd;
    std::mt19937 rng(rd());

    while (true) {
        if (low > high) {
            std::cout << "\nYour hints are contradictory. No number remains in range.\n";
            return 0;
        }

        int guess = (mode == '1') ? mind_reader::next_guess(low, high)
                                  : randomized_guess(low, high, rng, previous_guess);
        previous_guess = guess;

        char hint = read_choice(
            "My guess is " + std::to_string(guess) +
                ". If your number is higher [h], lower [l], correct [c], undo [u]: ",
            "hlcu");
        if (hint == '\0') {
            std::cout << "\nInput closed. Game is ending.\n";
            return 0;
        }

        if (hint == 'u') {
            if (history.empty()) {
                std::cout << "There is no step to undo.\n";
            } else {
                auto [old_low, old_high, old_steps, old_prev_guess] = history.back();
                history.pop_back();
                low = old_low;
                high = old_high;
                steps = old_steps;
                previous_guess = old_prev_guess;
                std::cout << "Last step undone.\n";
            }
            continue;
        }

        steps++;
        if (hint == 'c') {
            std::cout << "\nFound it! Your number is " << guess << ". "
                      << "I got it in " << steps << " guesses.\n";
            if (mode == '1' && steps <= kTheoreticalStepLimit) {
                std::cout << "Binary search power: near-minimum number of steps.\n";
            }
            if (mode == '2') {
                std::cout << "Random pivot mode gave a more surprising flow.\n";
            }
            return 0;
        }

        history.push_back({low, high, steps - 1, previous_guess});
        mind_reader::Range updated = mind_reader::apply_hint(low, high, guess, hint);
        low = updated.low;
        high = updated.high;
    }
}
