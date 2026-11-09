#include "Library.h"
#include "Utils.h"
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

const std::string DEFAULT_FILE = "library_data.txt";

bool readLine(const std::string &prompt, std::string &out) {
  std::cout << prompt;
  return static_cast<bool>(std::getline(std::cin, out));
}

// Keeps asking until the user types a whole number. Returns nullopt on end-of-input.
// Reading a whole line and converting it ourselves avoids the cin >> / getline
std::optional<int> readInt(const std::string &prompt) {
  std::string line;
  while (readLine(prompt, line)) {
    std::string text = utils::trim(line);
    try {
      std::size_t consumed = 0;
      int value = std::stoi(text, &consumed);
      if (consumed == text.size()) {
        return value;
      }
    } catch (const std::exception &) {
    }
    std::cout << "Please enter a whole number.\n";
  }
  return std::nullopt;
}

void printBooks(const std::vector<Book> &books) {
  if (books.empty()) {
    std::cout << "No books found.\n";
    return;
  }
  for (const auto &book : books) {
    book.print();
  }
}

void printResult(ActionResult result, const std::string &okMessage,
                 const std::string &invalidStateMessage) {
  switch (result) {
  case ActionResult::Ok:
    std::cout << okMessage << "\n";
    break;
  case ActionResult::NotFound:
    std::cout << "No book with that id.\n";
    break;
  case ActionResult::InvalidState:
    std::cout << invalidStateMessage << "\n";
    break;
  }
}

void printMenu() {
  std::cout << "\n===== Library Management System =====\n"
            << " 1. Add book\n"
            << " 2. List all books\n"
            << " 3. List sorted by title\n"
            << " 4. List sorted by author\n"
            << " 5. Search by title\n"
            << " 6. Search by author\n"
            << " 7. Borrow book\n"
            << " 8. Return book\n"
            << " 9. Remove book\n"
            << "10. Save to file\n"
            << "11. Load from file\n"
            << " 0. Exit\n";
}

// Asks for a filename; pressing Enter alone uses the default.
bool askFilename(const std::string &prompt, std::string &filename) {
  if (!readLine(prompt + " [" + DEFAULT_FILE + "]: ", filename)) {
    return false;
  }
  filename = utils::trim(filename);
  if (filename.empty()) {
    filename = DEFAULT_FILE;
  }
  return true;
}

} // namespace

int main() {
  Library library;

  while (true) {
    printMenu();
    auto choice = readInt("Choose an option: ");
    if (!choice || *choice == 0) {
      break;
    }

    std::string a, b;
    try {
      switch (*choice) {
      case 1: {
        if (!readLine("Title: ", a) || !readLine("Author: ", b))
          return 0;
        int id = library.addBook(a, b);
        std::cout << "Added book with id " << id << ".\n";
        break;
      }
      case 2:
        printBooks(library.getAllBooks());
        break;
      case 3:
        printBooks(library.sortedByTitle());
        break;
      case 4:
        printBooks(library.sortedByAuthor());
        break;
      case 5:
        if (!readLine("Title keyword: ", a))
          return 0;
        printBooks(library.searchByTitle(utils::trim(a)));
        break;
      case 6:
        if (!readLine("Author keyword: ", a))
          return 0;
        printBooks(library.searchByAuthor(utils::trim(a)));
        break;
      case 7: {
        auto id = readInt("Book id to borrow: ");
        if (!id)
          return 0;
        printResult(library.borrowBook(*id), "Borrowed.", "That book is already borrowed.");
        break;
      }
      case 8: {
        auto id = readInt("Book id to return: ");
        if (!id)
          return 0;
        printResult(library.returnBook(*id), "Returned.", "That book is not currently borrowed.");
        break;
      }
      case 9: {
        auto id = readInt("Book id to remove: ");
        if (!id)
          return 0;
        std::cout << (library.removeBook(*id) ? "Removed.\n" : "No book with that id.\n");
        break;
      }
      case 10:
        if (!askFilename("Filename to save to", a))
          return 0;
        if (library.saveToFile(a)) {
          std::cout << "Saved " << library.size() << " book(s) to " << a << ".\n";
        } else {
          std::cout << "Error: could not write to " << a << ".\n";
        }
        break;
      case 11: {
        if (!askFilename("Filename to load from", a))
          return 0;
        LoadResult r = library.loadFromFile(a);
        if (!r.opened) {
          std::cout << "Error: could not open " << a << ".\n";
        } else {
          std::cout << "Loaded " << r.loaded << " book(s) from " << a << ".\n";
          if (r.skipped > 0) {
            std::cout << "Warning: skipped " << r.skipped << " damaged line(s).\n";
          }
        }
        break;
      }
      default:
        std::cout << "Unknown option, try again.\n";
      }
    } catch (const std::invalid_argument &e) {
      std::cout << "Invalid input: " << e.what() << "\n";
    }
  }

  std::cout << "Goodbye!\n";
  return 0;
}
