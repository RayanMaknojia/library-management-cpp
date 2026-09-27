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

    // Phase 2: persistence.
    // Writes every book to a plain text file, one line each.
    void saveToFile(const std::string& filename) const;
    // Reads books from a file previously written by saveToFile,
    // replacing whatever is currently in memory. Returns false if the file
    // couldn't be opened.
    bool loadFromFile(const std::string& filename);

private:
    std::vector<Book> m_books;
    int m_nextId = 1; // simple auto-incrementing id generator
};

#endif // LIBRARY_H
