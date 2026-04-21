#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "mind_reader_logic.hpp"

namespace {

constexpr int kLowerBound = 1;
constexpr int kUpperBound = 1000;
constexpr int kTheoreticalMaxSteps = 11;

enum class GuessMode {
    Binary,
    Randomized
};

struct GameSnapshot {
    int low;
    int high;
    int attempts;
    int last_guess;
};

const char* logo() {
    return R"(
 __  __ _           _   ____                _
|  \/  (_)_ __   __| | |  _ \ ___  __ _  __| | ___ _ __
| |\/| | | '_ \ / _` | | |_) / _ \/ _` |/ _` |/ _ \ '__|
| |  | | | | | | (_| | |  _ <  __/ (_| | (_| |  __/ |
|_|  |_|_|_| |_|\__,_| |_| \_\___|\__,_|\__,_|\___|_|
)";
}

char normalize_hint(const std::string& input) {
    for (char ch : input) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }
    }
    return '\0';
}

int next_guess_randomized(int low, int high, std::mt19937& rng, int last_guess) {
    int mid = mind_reader::next_guess(low, high);
    int span = high - low + 1;
    if (span <= 3) {
        return mid;
    }

    int jitter_limit = std::max(1, span / 6);
    std::uniform_int_distribution<int> jitter(-jitter_limit, jitter_limit);
    int guess = std::clamp(mid + jitter(rng), low, high);

    if (span > 1 && guess == last_guess) {
        guess = (guess < high) ? guess + 1 : guess - 1;
    }
    return guess;
}

bool prompt_mode(GuessMode& mode) {
    while (true) {
        std::cout << "Mod seçimi: [1] Klasik İkili Arama (en hızlı), "
                  << "[2] Rastgele Pivot (daha sürprizli, önerilen): ";

        std::string input;
        if (!std::getline(std::cin, input)) {
            return false;
        }

        char choice = normalize_hint(input);
        if (choice == '\0' || choice == '2') {
            mode = GuessMode::Randomized;
            return true;
        }
        if (choice == '1') {
            mode = GuessMode::Binary;
            return true;
        }

        std::cout << "Geçersiz seçim. Lütfen 1 veya 2 gir.\n";
    }
}

bool prompt_hint(int guess, char& hint) {
    while (true) {
        std::cout << "Tahminim " << guess
                  << ". Daha büyükse [b], daha küçükse [k], doğruysa [d], "
                     "geri almak için [u]: ";

        std::string input;
        if (!std::getline(std::cin, input)) {
            return false;
        }

        hint = normalize_hint(input);
        if (hint == 'u' || mind_reader::is_valid_hint(hint)) {
            return true;
        }

        std::cout << "Geçersiz giriş. Lütfen sadece b, k, d veya u kullan.\n";
    }
}

}  // namespace

int main() {
    std::cout << logo() << "\n";
    std::cout << "Aklından 1 ile 1000 arasında bir sayı tut. Bana söyleme.\n";
    std::cout << "Hazırsan ENTER'a bas: ";

    std::string dummy;
    if (!std::getline(std::cin, dummy)) {
        std::cout << "\nGiriş sonlandı. Oyun kapatılıyor.\n";
        return 0;
    }

    GuessMode mode = GuessMode::Randomized;
    if (!prompt_mode(mode)) {
        std::cout << "\nGiriş sonlandı. Oyun kapatılıyor.\n";
        return 0;
    }

    int low = kLowerBound;
    int high = kUpperBound;
    int attempts = 0;
    int last_guess = -1;
    std::vector<GameSnapshot> history;

    std::random_device rd;
    std::mt19937 rng(rd());

    while (true) {
        if (low > high) {
            std::cout << "\nVerdiğin ipuçları birbiriyle çelişti. Bu aralıkta sayı kalmadı.\n";
            std::cout << "Oyunu yeniden başlatıp tekrar deneyelim.\n";
            return 0;
        }

        int guess = (mode == GuessMode::Binary)
                        ? mind_reader::next_guess(low, high)
                        : next_guess_randomized(low, high, rng, last_guess);
        last_guess = guess;

        char hint = '\0';
        if (!prompt_hint(guess, hint)) {
            std::cout << "\nGiriş sonlandı. Oyun kapatılıyor.\n";
            return 0;
        }

        if (hint == 'u') {
            if (history.empty()) {
                std::cout << "Geri alınacak önceki adım yok.\n";
            } else {
                GameSnapshot previous = history.back();
                history.pop_back();
                low = previous.low;
                high = previous.high;
                attempts = previous.attempts;
                last_guess = previous.last_guess;
                std::cout << "Son adım geri alındı.\n";
            }
            continue;
        }

        attempts++;
        if (hint == 'd') {
            std::cout << "\nBuldum! Tuttuğun sayı " << guess << ". "
                      << attempts << " tahminde yakaladım.\n";
            if (mode == GuessMode::Binary && attempts <= kTheoreticalMaxSteps) {
                std::cout << "İşte ikili arama gücü: minimuma yakın adımla sonuç.\n";
            } else if (mode == GuessMode::Randomized) {
                std::cout << "Rastgele pivot modu ile daha sürprizli bir akışta buldum.\n";
            }
            return 0;
        }

        history.push_back({low, high, attempts - 1, last_guess});

        try {
            mind_reader::Range updated = mind_reader::update_range(low, high, guess, hint);
            low = updated.low;
            high = updated.high;
        } catch (const std::invalid_argument&) {
            std::cout << "\nGeçersiz bir ipucu algılandı. Oyun bitiyor.\n";
            return 0;
        } catch (const std::logic_error&) {
            std::cout << "\nHey, ipuçları birbiriyle uyuşmuyor.\n";
            std::cout << "Bu akışta geçerli bir sayı kalmadı. Oyun bitiyor.\n";
            return 0;
        }
    }
}
