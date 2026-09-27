#include "Library.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

void Library::addBook(const std::string& title, const std::string& author) {
    m_books.emplace_back(m_nextId, title, author);
    std::cout << "Added book with id " << m_nextId << std::endl;
    m_nextId++;
}

bool Library::removeBook(int id) {
    auto it = std::find_if(m_books.begin(), m_books.end(),
                            [id](const Book& b) { return b.getId() == id; });
    if (it == m_books.end()) {
        return false;
    }
    m_books.erase(it);
    return true;
}

void Library::listBooks() const {
    if (m_books.empty()) {
        std::cout << "No books in the library yet." << std::endl;
        return;
    }
    for (const auto& book : m_books) {
        book.print();
    }
}

void Library::searchByTitle(const std::string& keyword) const {
    bool found = false;
    for (const auto& book : m_books) {
        // Simple case-sensitive substring search for Phase 1.
        if (book.getTitle().find(keyword) != std::string::npos) {
            book.print();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No books matched \"" << keyword << "\"." << std::endl;
    }
}

bool Library::borrowBook(int id) {
    for (auto& book : m_books) {
        if (book.getId() == id) {
            if (book.isBorrowed()) {
                std::cout << "Book is already borrowed." << std::endl;
                return false;
            }
            book.setBorrowed(true);
            return true;
        }
    }
    return false;
}

bool Library::returnBook(int id) {
    for (auto& book : m_books) {
        if (book.getId() == id) {
            book.setBorrowed(false);
            return true;
        }
    }
    return false;
}

// --- Phase 2: persistence ---
//
// File format: one book per line, fields separated by '|':
//   id|title|author|borrowedFlag
// Example line: 3|The Hobbit|J.R.R. Tolkien|0
//
// Limitation: since '|' is the separator, a title or author containing '|'
// would break parsing. Fine for a learning project; a real system would use
// a proper format like CSV with escaping, or JSON.

void Library::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename); // opens (creates/overwrites) the file for writing

    if (!outFile) {
        // std::ofstream converts to false if opening failed
        // (e.g. bad path, no write permission).
        std::cerr << "Error: could not open file for writing: " << filename << std::endl;
        return;
    }

    for (const auto& book : m_books) {
        outFile << book.getId() << "|"
                << book.getTitle() << "|"
                << book.getAuthor() << "|"
                << book.isBorrowed() << "\n";
    }

    std::cout << "Saved " << m_books.size() << " book(s) to " << filename << std::endl;
    // outFile closes automatically when it goes out of scope (its destructor
    // flushes and closes the file) - no explicit outFile.close() needed.
}

bool Library::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename); // opens the file for reading

    if (!inFile) {
        std::cerr << "Error: could not open file for reading: " << filename << std::endl;
        return false;
    }

    m_books.clear();  // wipe whatever is currently in memory
    m_nextId = 1;      // will be recalculated from the loaded ids below

    std::string line;
    while (std::getline(inFile, line)) {
        if (line.empty()) {
            continue; // skip blank lines rather than crashing on them
        }

        std::stringstream ss(line); // lets us treat the line like an input stream
        std::string idStr, title, author, borrowedStr;

        std::getline(ss, idStr, '|');
        std::getline(ss, title, '|');
        std::getline(ss, author, '|');
        std::getline(ss, borrowedStr, '|');

        int id = std::stoi(idStr);          // string -> int conversion
        bool borrowed = (borrowedStr == "1");

        m_books.emplace_back(id, title, author, borrowed);

        if (id >= m_nextId) {
            m_nextId = id + 1; // keep auto-increment consistent with loaded data
        }
    }

    std::cout << "Loaded " << m_books.size() << " book(s) from " << filename << std::endl;
    return true;
}
