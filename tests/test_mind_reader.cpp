#include <iostream>

#include "mind_reader_logic.hpp"

int hata = 0;

void kontrol(bool kosul, const char* mesaj) {
    if (!kosul) {
        std::cerr << "HATA: " << mesaj << "\n";
        hata++;
    }
}

int main() {
    kontrol(mind_reader::next_guess(1, 1000) == 500, "Orta nokta 500 olmalı");
    kontrol(mind_reader::next_guess(501, 1000) == 750, "Orta nokta 750 olmalı");

    kontrol(mind_reader::is_valid_hint('b'), "b geçerli olmalı");
    kontrol(mind_reader::is_valid_hint('k'), "k geçerli olmalı");
    kontrol(mind_reader::is_valid_hint('d'), "d geçerli olmalı");
    kontrol(!mind_reader::is_valid_hint('u'), "u mantık ipucu olmamalı");

    auto yukari = mind_reader::apply_hint(1, 1000, 500, 'b');
    kontrol(yukari.low == 501 && yukari.high == 1000, "b ipucu alt sınırı artırmalı");

    auto asagi = mind_reader::apply_hint(1, 1000, 500, 'k');
    kontrol(asagi.low == 1 && asagi.high == 499, "k ipucu üst sınırı azaltmalı");

    auto dogru = mind_reader::apply_hint(1, 1000, 500, 'd');
    kontrol(dogru.low == 1 && dogru.high == 1000, "d ipucu aralığı korumalı");

    if (hata) {
        std::cerr << hata << " test başarısız.\n";
        return 1;
    }

    std::cout << "Tüm testler başarılı.\n";
    return 0;
}
