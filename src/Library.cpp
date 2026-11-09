#include "Library.h"
#include "Utils.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

int Library::addBook(const std::string &title, const std::string &author) {
  std::string cleanTitle = utils::trim(title);
  std::string cleanAuthor = utils::trim(author);

  if (cleanTitle.empty() || cleanAuthor.empty()) {
    throw std::invalid_argument("Title and author must not be empty");
  }
  if (cleanTitle.find('|') != std::string::npos || cleanAuthor.find('|') != std::string::npos) {
    throw std::invalid_argument("Title and author must not contain the '|' character");
  }

  int id = m_nextId++;
  m_books.emplace_back(id, cleanTitle, cleanAuthor);
  return id;
}

bool Library::removeBook(int id) {
  auto it =
      std::find_if(m_books.begin(), m_books.end(), [id](const Book &b) { return b.getId() == id; });
  if (it == m_books.end()) {
    return false;
  }
  m_books.erase(it);
  return true;
}

ActionResult Library::borrowBook(int id) {
  Book *book = findMutable(id);
  if (!book) {
    return ActionResult::NotFound;
  }
  if (book->isBorrowed()) {
    return ActionResult::InvalidState;
  }
  book->setBorrowed(true);
  return ActionResult::Ok;
}

ActionResult Library::returnBook(int id) {
  Book *book = findMutable(id);
  if (!book) {
    return ActionResult::NotFound;
  }
  if (!book->isBorrowed()) {
    return ActionResult::InvalidState;
  }
  book->setBorrowed(false);
  return ActionResult::Ok;
}

std::optional<Book> Library::findById(int id) const {
  for (const auto &book : m_books) {
    if (book.getId() == id) {
      return book;
    }
  }
  return std::nullopt;
}

std::vector<Book> Library::getAllBooks() const { return m_books; }

std::vector<Book> Library::searchByTitle(const std::string &keyword) const {
  std::vector<Book> results;
  for (const auto &book : m_books) {
    if (utils::containsIgnoreCase(book.getTitle(), keyword)) {
      results.push_back(book);
    }
  }
  return results;
}

std::vector<Book> Library::searchByAuthor(const std::string &keyword) const {
  std::vector<Book> results;
  for (const auto &book : m_books) {
    if (utils::containsIgnoreCase(book.getAuthor(), keyword)) {
      results.push_back(book);
    }
  }
  return results;
}

std::vector<Book> Library::sortedByTitle() const {
  std::vector<Book> sorted = m_books;
  std::sort(sorted.begin(), sorted.end(), [](const Book &a, const Book &b) {
    return utils::toLower(a.getTitle()) < utils::toLower(b.getTitle());
  });
  return sorted;
}

std::vector<Book> Library::sortedByAuthor() const {
  std::vector<Book> sorted = m_books;
  std::sort(sorted.begin(), sorted.end(), [](const Book &a, const Book &b) {
    return utils::toLower(a.getAuthor()) < utils::toLower(b.getAuthor());
  });
  return sorted;
}

std::size_t Library::size() const { return m_books.size(); }

bool Library::saveToFile(const std::string &filename) const {
  std::ofstream outFile(filename);
  if (!outFile) {
    return false;
  }
  for (const auto &book : m_books) {
    outFile << book.getId() << "|" << book.getTitle() << "|" << book.getAuthor() << "|"
            << (book.isBorrowed() ? 1 : 0) << "\n";
  }
  return static_cast<bool>(outFile);
}

LoadResult Library::loadFromFile(const std::string &filename) {
  LoadResult result;
  std::ifstream inFile(filename);
  if (!inFile) {
    return result;
  }
  result.opened = true;

  // Parse into temporary variables first, so a half-read file never
  // leaves the library in a broken state.
  std::vector<Book> loaded;
  int nextId = 1;

  std::string line;
  while (std::getline(inFile, line)) {
    if (utils::trim(line).empty()) {
      continue;
    }

    std::stringstream ss(line);
    std::string idStr, title, author, borrowedStr;
    std::getline(ss, idStr, '|');
    std::getline(ss, title, '|');
    std::getline(ss, author, '|');
    std::getline(ss, borrowedStr, '|');

    try {
      int id = std::stoi(idStr); // throws if idStr isn't a number
      if (title.empty() || author.empty() || (borrowedStr != "0" && borrowedStr != "1")) {
        throw std::invalid_argument("bad line");
      }
      loaded.emplace_back(id, title, author, borrowedStr == "1");
      nextId = std::max(nextId, id + 1);
      ++result.loaded;
    } catch (const std::exception &) {
      ++result.skipped;
    }
  }

  m_books = std::move(loaded);
  m_nextId = nextId;
  return result;
}

Book *Library::findMutable(int id) {
  for (auto &book : m_books) {
    if (book.getId() == id) {
      return &book;
    }
  }
  return nullptr;
}
