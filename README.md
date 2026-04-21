# MindReader

In this project, roles are reversed: the player thinks of a number, and the
computer finds it quickly using search strategy.

This version is written in C++.

## Game Flow

1. Think of a number between `1` and `1000`.
2. Choose mode: `1` = Classic Binary Search, `2` = Random Pivot.
3. The computer makes a guess.
4. Respond with:
   - `h`: My number is higher
   - `l`: My number is lower
   - `c`: Correct guess
   - `u`: Undo last step
5. The computer narrows the range and keeps guessing.

## Features

- Classic Binary Search mode (fastest)
- Random Pivot mode (less repetitive, more surprising flow)
- Input validation
- Undo support with `u`
- Contradiction detection (when no valid number remains)
- ASCII logo at startup
- GitHub Actions CI for automated tests
- Separated logic module (`next_guess`, `apply_hint`) for easy testing

## Run

```bash
make run
```

## Test

```bash
make test
```

## Manual Build

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude src/main.cpp src/mind_reader_logic.cpp -o build/mind_reader
```
