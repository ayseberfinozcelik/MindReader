#include <cctype>
#include <iostream>
#include <string>

#include "mind_reader_logic.hpp"

namespace {

constexpr int kLowerBound = 1;
constexpr int kUpperBound = 1000;
constexpr int kTheoreticalMaxSteps = 11;

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

bool prompt_hint(int guess, char& hint) {
    while (true) {
        std::cout << "Tahminim " << guess
                  << ". Tuttugun sayi daha buyukse [y], daha kucukse [a], dogruysa [e]: ";

        std::string input;
        if (!std::getline(std::cin, input)) {
            return false;
        }

        hint = normalize_hint(input);
        if (mind_reader::is_valid_hint(hint)) {
            return true;
        }

        std::cout << "Gecersiz giris. Lutfen sadece y, a veya e kullan.\n";
    }
}

}  // namespace

int main() {
    std::cout << logo() << "\n";
    std::cout << "Aklindan 1 ile 1000 arasinda bir sayi tut. Bana soyleme.\n";
    std::cout << "Hazirsan ENTER'a bas: ";

    std::string dummy;
    if (!std::getline(std::cin, dummy)) {
        std::cout << "\nGiris sonlandi. Oyun kapatiliyor.\n";
        return 0;
    }

    int low = kLowerBound;
    int high = kUpperBound;
    int attempts = 0;

    while (true) {
        if (low > high) {
            std::cout << "\nHmm... burada bir celiski var.\n";
            std::cout << "Verdigin ipuclariyla bu aralikta sayi kalmadi.\n";
            std::cout << "Oyunu yeniden baslatip tekrar deneyelim.\n";
            return 0;
        }

        int guess = mind_reader::next_guess(low, high);
        attempts++;

        char hint = '\0';
        if (!prompt_hint(guess, hint)) {
            std::cout << "\nGiris sonlandi. Oyun kapatiliyor.\n";
            return 0;
        }

        if (hint == 'e') {
            std::cout << "\nBuldum! Sayin " << guess << ". "
                      << attempts << " tahminde yakaladim.\n";
            if (attempts <= kTheoreticalMaxSteps) {
                std::cout << "Iste ikili arama gucu: cok az adim, net sonuc.\n";
            }
            return 0;
        }

        try {
            mind_reader::Range updated = mind_reader::update_range(low, high, guess, hint);
            low = updated.low;
            high = updated.high;
        } catch (const std::invalid_argument&) {
            std::cout << "\nGecersiz bir ipucu algilandi. Oyun bitiyor.\n";
            return 0;
        } catch (const std::logic_error&) {
            std::cout << "\nHey, yalan soyluyorsun ya da yanlis tusladin.\n";
            std::cout << "Bu ipuclari birbiriyle uyusmuyor. Oyun bitiyor.\n";
            return 0;
        }
    }
}
