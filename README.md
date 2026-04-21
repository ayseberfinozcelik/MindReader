# Mind Reader - Tersine Muhendislik Sayi Tahmin Oyunu

Bu projede roller degisiyor: kullanici bir sayi tutuyor, bilgisayar Binary Search
(Ikili Arama) ile sayiyi minimum adimda buluyor.

Bu surum C++ ile yazildi.

## Oyun Akisi

1. Program senden `1-1000` arasi bir sayi tutmani ister.
2. Mod secersin: `1` = Klasik Binary (garantili hizli), `2` = Rastgele Pivot (daha az tekrar eden tahminler).
3. Bilgisayar tahmin eder.
4. Sen su sekilde cevap verirsin:
   - `b`: Tuttugum sayi daha buyuk
   - `k`: Tuttugum sayi daha kucuk
   - `d`: Dogru tahmin
   - `u`: Son adimi geri al
5. Bilgisayar secilen moda gore araligi daraltarak sonuca gider.

## Ozellikler

- Klasik Binary Search modu (en hizli)
- Rastgele Pivot Search modu (daha surprizli tahmin akisi)
- Gecersiz tuslama kontrolu (`b/k/d` disinda giris)
- Yanlis tuslama icin `u` ile son adimi geri alma
- Celiskili/yalan ipucu tespiti (imkansiz aralik olusursa oyun biter)
- Baslangicta ASCII logo
- GitHub Actions ile otomatik test (CI)
- Is mantigi (`next_guess`, `update_range`) ayri modulde oldugu icin test edilebilir yapi

## Calistirma

```bash
make run
```

## Test

```bash
make test
```

## Elle Derleme

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude src/main.cpp src/mind_reader_logic.cpp -o build/mind_reader
```
