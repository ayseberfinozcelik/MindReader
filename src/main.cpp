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

bool prompt_hint(int guess, const char* phase, char& hint) {
    while (true) {
        if (phase != nullptr && phase[0] != '\0') {
            std::cout << phase << " | ";
        }

        std::cout << "Tahminim " << guess
                  << ". Tuttugun sayi daha buyukse [b], daha kucukse [k], dogruysa [d]: ";

        std::string input;
        if (!std::getline(std::cin, input)) {
            return false;
        }

        hint = normalize_hint(input);
        if (mind_reader::is_valid_hint(hint)) {
            return true;
        }

        std::cout << "Gecersiz giris. Lutfen sadece b, k veya d kullan.\n";
    }
}

bool prompt_verified_hint(int guess, char& verified_hint, bool& lie_used) {
    char first = '\0';
    char second = '\0';

    if (!prompt_hint(guess, "Kontrol 1/2", first)) {
        return false;
    }
    if (!prompt_hint(guess, "Kontrol 2/2", second)) {
        return false;
    }

    mind_reader::HintCheckResult pair = mind_reader::verify_two_hints(first, second, lie_used);
    if (!pair.needs_third) {
        verified_hint = pair.hint;
        return true;
    }

    std::cout << "Ipuclari farkli geldi. Ek bir dogrulama sorusu soruyorum.\n";
    char third = '\0';
    if (!prompt_hint(guess, "Dogrulama 3/3", third)) {
        return false;
    }

    verified_hint = mind_reader::resolve_third_hint(first, second, third);
    lie_used = true;
    std::cout << "Not: 1 kez yanlis ipucu hakki kullanildi.\n";
    return true;
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
    bool lie_used = false;

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
        try {
            if (!prompt_verified_hint(guess, hint, lie_used)) {
                std::cout << "\nGiris sonlandi. Oyun kapatiliyor.\n";
                return 0;
            }
        } catch (const std::invalid_argument&) {
            std::cout << "\nGecersiz bir ipucu algilandi. Oyun bitiyor.\n";
            return 0;
        } catch (const std::logic_error& err) {
            std::cout << "\n" << err.what() << "\n";
            std::cout << "Oyun bitiyor.\n";
            return 0;
        }

        if (hint == 'd') {
            std::cout << "\nBuldum! Sayin " << guess << ". "
                      << attempts << " tahminde yakaladim.\n";
            if (attempts <= kTheoreticalMaxSteps) {
                std::cout << "Iste ikili arama gucu: cok az adim, net sonuc.\n";
            }
            if (lie_used) {
                std::cout << "Yine de yakaladim: 1 yanlis ipucunu tolere ettim.\n";
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
