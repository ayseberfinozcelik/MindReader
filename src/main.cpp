#include <algorithm>
#include <cctype>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "mind_reader_logic.hpp"

namespace {

constexpr int kBaslangicAlt = 1;
constexpr int kBaslangicUst = 1000;
constexpr int kTeorikAdimLimiti = 11;

const char* logo() {
    return R"(
 __  __ _           _   ____                _
|  \/  (_)_ __   __| | |  _ \ ___  __ _  __| | ___ _ __
| |\/| | | '_ \ / _` | | |_) / _ \/ _` |/ _` |/ _ \ '__|
| |  | | | | | | (_| | |  _ <  __/ (_| | (_| |  __/ |
|_|  |_|_|_| |_|\__,_| |_| \_\___|\__,_|\__,_|\___|_|
)";
}

char ilk_karakter(const std::string& metin) {
    for (char ch : metin) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }
    }
    return '\0';
}

char secim_al(const std::string& soru, const std::string& izinli) {
    while (true) {
        std::cout << soru;
        std::string giris;
        if (!std::getline(std::cin, giris)) {
            return '\0';
        }
        char secim = ilk_karakter(giris);
        if (izinli.find(secim) != std::string::npos) {
            return secim;
        }
        std::cout << "Geçersiz giriş. Tekrar dene.\n";
    }
}

int rastgele_tahmin(int alt, int ust, std::mt19937& rng, int onceki) {
    int orta = mind_reader::next_guess(alt, ust);
    int genislik = ust - alt + 1;
    if (genislik <= 3) {
        return orta;
    }

    int sapma = std::max(1, genislik / 6);
    std::uniform_int_distribution<int> dagilim(-sapma, sapma);
    int tahmin = std::clamp(orta + dagilim(rng), alt, ust);

    if (tahmin == onceki && alt != ust) {
        tahmin = (tahmin < ust) ? tahmin + 1 : tahmin - 1;
    }
    return tahmin;
}

}  // namespace

int main() {
    std::cout << logo() << "\n";
    std::cout << "Aklından 1 ile 1000 arasında bir sayı tut. Bana söyleme.\n";
    std::cout << "Hazırsan ENTER'a bas: ";

    std::string satir;
    if (!std::getline(std::cin, satir)) {
        std::cout << "\nGiriş sonlandı. Oyun kapatılıyor.\n";
        return 0;
    }

    char mod = secim_al(
        "Mod seçimi: [1] Klasik İkili Arama, [2] Rastgele Pivot (önerilen): ",
        "12");
    if (mod == '\0') {
        std::cout << "\nGiriş sonlandı. Oyun kapatılıyor.\n";
        return 0;
    }

    int alt = kBaslangicAlt;
    int ust = kBaslangicUst;
    int adim = 0;
    int onceki_tahmin = -1;
    std::vector<std::tuple<int, int, int, int>> gecmis;

    std::random_device rd;
    std::mt19937 rng(rd());

    while (true) {
        if (alt > ust) {
            std::cout << "\nİpuçların çelişti, bu aralıkta sayı kalmadı.\n";
            return 0;
        }

        int tahmin = (mod == '1') ? mind_reader::next_guess(alt, ust)
                                  : rastgele_tahmin(alt, ust, rng, onceki_tahmin);
        onceki_tahmin = tahmin;

        char yanit = secim_al(
            "Tahminim " + std::to_string(tahmin) +
                ". Büyükse [b], küçükse [k], doğruysa [d], geri almak için [u]: ",
            "bkdu");
        if (yanit == '\0') {
            std::cout << "\nGiriş sonlandı. Oyun kapatılıyor.\n";
            return 0;
        }

        if (yanit == 'u') {
            if (gecmis.empty()) {
                std::cout << "Geri alınacak adım yok.\n";
            } else {
                auto [eski_alt, eski_ust, eski_adim, eski_tahmin] = gecmis.back();
                gecmis.pop_back();
                alt = eski_alt;
                ust = eski_ust;
                adim = eski_adim;
                onceki_tahmin = eski_tahmin;
                std::cout << "Son adım geri alındı.\n";
            }
            continue;
        }

        adim++;
        if (yanit == 'd') {
            std::cout << "\nBuldum! Tuttuğun sayı " << tahmin << ". "
                      << adim << " tahminde yakaladım.\n";
            if (mod == '1' && adim <= kTeorikAdimLimiti) {
                std::cout << "İkili arama ile minimuma yakın adımda sonuç.\n";
            }
            if (mod == '2') {
                std::cout << "Rastgele pivot ile daha sürprizli akış.\n";
            }
            return 0;
        }

        gecmis.push_back({alt, ust, adim - 1, onceki_tahmin});
        mind_reader::Range yeni = mind_reader::apply_hint(alt, ust, tahmin, yanit);
        alt = yeni.low;
        ust = yeni.high;
    }
}
