# Library Management System (C++)

A phased C++ practice project. Each phase adds capability on top of the last.

## Phase 1 — Core structure, in-memory storage

- `Book` class: represents a single book (id, title, author, borrowed status).
- `Library` class: manages a collection of `Book` objects (`std::vector<Book>`), with add/list/search/borrow/return/remove.
- Console menu in `main.cpp` drives everything.
- No persistence — data was lost when the program exited.

## Phase 2 — File persistence

- Added `Library::saveToFile(filename)` and `Library::loadFromFile(filename)`.
- Simple pipe-delimited text format: `id|title|author|borrowedFlag`, one book per line.
- Menu options 7 (save) and 8 (load) added.
- Known limitation: a `|` character in a title/author would break parsing — fine for now, a real system would use CSV-with-escaping or JSON (something to revisit in a later phase).

## Build & run

```bash
mkdir build && cd build
cmake ..
cmake --build .
./library_system
```

## Roadmap

- Phase 1: In-memory core ✅
- Phase 2: File persistence (save/load books to disk) ✅ (this phase)
- Phase 3: Better search/sort, input validation, unit tests
- Phase 4: SQLite database backend
- Phase 5 (optional): GUI with Qt, or REST API layer
