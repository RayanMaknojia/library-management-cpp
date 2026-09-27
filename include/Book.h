#ifndef BOOK_H
#define BOOK_H

#include <string>

// Represents a single book in the library.
// Phase 1: pure data + simple behavior, no persistence.
class Book {
public:
    Book(int id, std::string title, std::string author, bool isBorrowed = false);

    int getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    bool isBorrowed() const;

    void setBorrowed(bool borrowed);

    // Prints book details in a readable format.
    void print() const;

private:
    int m_id;
    std::string m_title;
    std::string m_author;
    bool m_isBorrowed;
};

#endif // BOOK_H
