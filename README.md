# Library Management System (C++)

A phased C++ practice project. Each phase adds capability on top of the last.

## Phase 1 — Core structure, in-memory storage

- `Book` class: represents a single book (id, title, author, borrowed status).
- `Library` class: manages a collection of `Book` objects (`std::vector<Book>`), with add/list/search/borrow/return/remove.
- Console menu in `main.cpp` drives everything.
- **No persistence** — data is lost when the program exits. That's intentional; it's Phase 2's job.

## Build & run

```bash
mkdir build && cd build
cmake ..
cmake --build .
./library_system
```

## Roadmap

- Phase 1: In-memory core (this phase)
- Phase 2: File persistence (save/load books to disk)
- Phase 3: Better search/sort, input validation, unit tests
- Phase 4: SQLite database backend
- Phase 5 (optional): GUI with Qt, or REST API layer
