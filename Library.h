#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "Book.h"

// Manages an in-memory collection of Book objects.
// Phase 1: no file/database persistence - data resets on program exit.
class Library {
public:
    void addBook(const std::string& title, const std::string& author);
    bool removeBook(int id);
    void listBooks() const;
    void searchByTitle(const std::string& keyword) const;

    bool borrowBook(int id);
    bool returnBook(int id);

private:
    std::vector<Book> m_books;
    int m_nextId = 1; // simple auto-incrementing id generator
};

#endif // LIBRARY_H
