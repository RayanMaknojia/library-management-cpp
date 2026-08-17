#include "Library.h"
#include <iostream>
#include <algorithm>

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
