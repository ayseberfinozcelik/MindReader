# Mind Reader - Tersine Muhendislik Sayi Tahmin Oyunu

Bu projede roller degisiyor: kullanici bir sayi tutuyor, bilgisayar Binary Search
(Ikili Arama) ile sayiyi minimum adimda buluyor.

Bu surum C++ ile yazildi.

## Oyun Akisi

1. Program senden `1-1000` arasi bir sayi tutmani ister.
2. Bilgisayar ortadaki sayiyi tahmin eder.
3. Sen su sekilde cevap verirsin:
   - `b`: Tuttugum sayi daha buyuk
   - `k`: Tuttugum sayi daha kucuk
   - `d`: Dogru tahmin
4. Bilgisayar araligi her adimda ikiye bolerek hizla sonuca gider.

## Ozellikler

- Binary Search ile en fazla 10-11 adimda sonucu bulma
- Gecersiz tuslama kontrolu (`b/k/d` disinda giris)
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
