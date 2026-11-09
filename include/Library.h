#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

enum class ActionResult { Ok, NotFound, InvalidState };

struct LoadResult {
  bool opened = false;
  std::size_t loaded = 0;
  std::size_t skipped = 0;
};

class Library {
public:
  int addBook(const std::string &title, const std::string &author);
  bool removeBook(int id);
  ActionResult borrowBook(int id);
  ActionResult returnBook(int id);

  std::optional<Book> findById(int id) const;
  std::vector<Book> getAllBooks() const;
  std::vector<Book> searchByTitle(const std::string &keyword) const;
  std::vector<Book> searchByAuthor(const std::string &keyword) const;
  std::vector<Book> sortedByTitle() const;
  std::vector<Book> sortedByAuthor() const;
  std::size_t size() const;

  bool saveToFile(const std::string &filename) const;
  LoadResult loadFromFile(const std::string &filename);

private:
  Book *findMutable(int id);

  std::vector<Book> m_books;
  int m_nextId = 1;
};

#endif // LIBRARY_H
