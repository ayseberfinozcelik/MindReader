"""Mind Reader game using binary search.

The computer tries to guess the number the user keeps in mind.
"""

from __future__ import annotations

from dataclasses import dataclass


LOWER_BOUND = 1
UPPER_BOUND = 1000
VALID_HINTS = {"y", "a", "e"}


def next_guess(low: int, high: int) -> int:
    """Return midpoint guess for current range."""
    return (low + high) // 2


def update_range(low: int, high: int, guess: int, hint: str) -> tuple[int, int]:
    """Update search range based on user hint.

    Hint meanings:
    - y: my number is higher
    - a: my number is lower
    - e: exact match
    """
    if hint == "y":
        low = guess + 1
    elif hint == "a":
        high = guess - 1
    elif hint == "e":
        return low, high
    else:
        raise ValueError("Gecersiz ipucu. Sadece y/a/e kullan.")

    if low > high:
        raise ValueError("Tutarsiz ipucu: Bu aralikta sayi kalmadi.")

    return low, high


@dataclass
class GameState:
    low: int = LOWER_BOUND
    high: int = UPPER_BOUND
    attempts: int = 0


def logo() -> str:
    return r"""
 __  __ _           _   ____                _
|  \/  (_)_ __   __| | |  _ \ ___  __ _  __| | ___ _ __
| |\/| | | '_ \ / _` | | |_) / _ \/ _` |/ _` |/ _ \ '__|
| |  | | | | | | (_| | |  _ <  __/ (_| | (_| |  __/ |
|_|  |_|_|_| |_|\__,_| |_| \_\___|\__,_|\__,_|\___|_|
"""


def prompt_hint(guess: int) -> str:
    while True:
        answer = input(
            f"Tahminim {guess}. "
            "Tuttugun sayi daha buyukse [y], daha kucukse [a], dogruysa [e]: "
        ).strip().lower()

        if answer in VALID_HINTS:
            return answer

        print("Gecersiz giris. Lutfen sadece y, a veya e kullan.")


def play() -> None:
    print(logo())
    print("Aklindan 1 ile 1000 arasinda bir sayi tut. Bana soyleme.")
    input("Hazirsan ENTER'a bas: ")

    state = GameState()
    theoretical_max = 11  # ceil(log2(1000)) ~= 10, safe upper bound with inclusive range.

    while True:
        if state.low > state.high:
            print("\nHmm... burada bir celiski var.")
            print("Verdigin ipuclariyla bu aralikta sayi kalmadi.")
            print("Oyunu yeniden baslatip tekrar deneyelim.")
            return

        guess = next_guess(state.low, state.high)
        state.attempts += 1
        hint = prompt_hint(guess)

        if hint == "e":
            print(
                f"\nBuldum! Sayin {guess}. "
                f"{state.attempts} tahminde yakaladim."
            )
            if state.attempts <= theoretical_max:
                print("Iste ikili arama gucu: cok az adim, net sonuc.")
            return

        try:
            state.low, state.high = update_range(state.low, state.high, guess, hint)
        except ValueError:
            print("\nHey, yalan soyluyorsun ya da yanlis tusladin.")
            print("Bu ipuclari birbiriyle uyusmuyor. Oyun bitiyor.")
            return


if __name__ == "__main__":
    play()
