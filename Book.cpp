#include "Book.h"
#include <iostream>

Book::Book(int id, std::string title, std::string author, bool isBorrowed)
    : m_id(id), m_title(std::move(title)), m_author(std::move(author)), m_isBorrowed(isBorrowed) {}

int Book::getId() const {
    return m_id;
}

std::string Book::getTitle() const {
    return m_title;
}

std::string Book::getAuthor() const {
    return m_author;
}

bool Book::isBorrowed() const {
    return m_isBorrowed;
}

void Book::setBorrowed(bool borrowed) {
    m_isBorrowed = borrowed;
}

void Book::print() const {
    std::cout << "[" << m_id << "] \"" << m_title << "\" by " << m_author
              << (m_isBorrowed ? "  (borrowed)" : "  (available)") << std::endl;
}
