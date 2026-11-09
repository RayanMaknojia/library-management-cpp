# Library Management System (C++)

A phased C++ practice project. Each phase adds capability on top of the last.

## Phase 1 — Core structure, in-memory storage
- `Book` and `Library` classes, console menu, `std::vector` storage.

## Phase 2 — File persistence
- Save/load books to a pipe-delimited text file: `id|title|author|borrowedFlag`.

## Phase 3 — Validation, better search/sort, unit tests
- `Library` no longer prints; it returns data (`std::vector<Book>`, `std::optional<Book>`) and `main.cpp` displays it.
- Case-insensitive search by title and by author.
- Sorting by title or author (on a copy; original order untouched).
- Validation: blank or `|`-containing title/author is rejected (`std::invalid_argument`), input is trimmed.
- Menu input is read line-by-line and converted safely, so `abc` or `12abc` never breaks the program.
- Borrow/return report precise outcomes via `enum class ActionResult` (Ok / NotFound / InvalidState).
- Loading is safe: a missing file never wipes data, damaged lines are skipped and counted.
- Unit tests in `tests/` (no external framework), wired into CTest.
- Build restructured: core code in a `library_core` library shared by the app and the tests.

## Build, test, run

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure   # run the unit tests
./library_system            # run the program
```

## Roadmap

- Phase 1: In-memory core ✅
- Phase 2: File persistence ✅
- Phase 3: Validation, search/sort, unit tests ✅ (this phase)
- Phase 4: SQLite database backend
- Phase 5 (optional): GUI with Qt, or REST API layer
