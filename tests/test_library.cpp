// Dependency-free unit tests. Each test is a function; CHECK records a failure
// instead of aborting, so one run reports everything that's broken.
#include "Library.h"
#include "Utils.h"
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

static int g_checks = 0;
static int g_failures = 0;

#define CHECK(cond)                                                                                \
  do {                                                                                             \
    ++g_checks;                                                                                    \
    if (!(cond)) {                                                                                 \
      ++g_failures;                                                                                \
      std::cerr << "  FAILED: " #cond " (line " << __LINE__ << ")\n";                              \
    }                                                                                              \
  } while (0)

#define CHECK_THROWS(expr)                                                                         \
  do {                                                                                             \
    ++g_checks;                                                                                    \
    bool threw = false;                                                                            \
    try {                                                                                          \
      expr;                                                                                        \
    } catch (const std::exception &) {                                                             \
      threw = true;                                                                                \
    }                                                                                              \
    if (!threw) {                                                                                  \
      ++g_failures;                                                                                \
      std::cerr << "  FAILED: expected exception from " #expr << " (line " << __LINE__ << ")\n";   \
    }                                                                                              \
  } while (0)

static std::string tempFile(const std::string &name) {
  return (std::filesystem::temp_directory_path() / name).string();
}

static void testUtils() {
  CHECK(utils::toLower("HeLLo") == "hello");
  CHECK(utils::trim("  hi there \n") == "hi there");
  CHECK(utils::trim("   ").empty());
  CHECK(utils::containsIgnoreCase("The Hobbit", "hOBB"));
  CHECK(!utils::containsIgnoreCase("The Hobbit", "dune"));
}

static void testAddAndFind() {
  Library lib;
  int id = lib.addBook("  Dune ", "Frank Herbert");
  CHECK(lib.size() == 1);
  auto book = lib.findById(id);
  CHECK(book.has_value());
  CHECK(book->getTitle() == "Dune"); // surrounding whitespace was trimmed
  CHECK(!lib.findById(999).has_value());
}

static void testIdsAreUnique() {
  Library lib;
  int a = lib.addBook("A", "X");
  int b = lib.addBook("B", "X");
  lib.removeBook(a);
  int c = lib.addBook("C", "X");
  CHECK(a != b);
  CHECK(c != b && c != a); // ids are never reused after a removal
}

static void testAddValidation() {
  Library lib;
  CHECK_THROWS(lib.addBook("", "Someone"));
  CHECK_THROWS(lib.addBook("Title", "   "));
  CHECK_THROWS(lib.addBook("Bad|Title", "Someone"));
  CHECK_THROWS(lib.addBook("Title", "Bad|Author"));
  CHECK(lib.size() == 0); // nothing was added by the failed calls
}

static void testSearch() {
  Library lib;
  lib.addBook("The Hobbit", "J.R.R. Tolkien");
  lib.addBook("The Lord of the Rings", "J.R.R. Tolkien");
  lib.addBook("1984", "George Orwell");
  CHECK(lib.searchByTitle("the").size() == 2); // case-insensitive
  CHECK(lib.searchByTitle("HOBBIT").size() == 1);
  CHECK(lib.searchByAuthor("tolkien").size() == 2);
  CHECK(lib.searchByAuthor("nobody").empty());
}

static void testSorting() {
  Library lib;
  lib.addBook("banana", "Zed");
  lib.addBook("Apple", "Yan");
  lib.addBook("cherry", "Xia");
  auto byTitle = lib.sortedByTitle();
  CHECK(byTitle[0].getTitle() == "Apple"); // case-insensitive ordering
  CHECK(byTitle[1].getTitle() == "banana");
  CHECK(byTitle[2].getTitle() == "cherry");
  auto byAuthor = lib.sortedByAuthor();
  CHECK(byAuthor[0].getAuthor() == "Xia");
  CHECK(lib.getAllBooks()[0].getTitle() == "banana"); // original order untouched
}

static void testBorrowAndReturn() {
  Library lib;
  int id = lib.addBook("Dune", "Frank Herbert");
  CHECK(lib.borrowBook(id) == ActionResult::Ok);
  CHECK(lib.findById(id)->isBorrowed());
  CHECK(lib.borrowBook(id) == ActionResult::InvalidState);
  CHECK(lib.returnBook(id) == ActionResult::Ok);
  CHECK(lib.returnBook(id) == ActionResult::InvalidState);
  CHECK(lib.borrowBook(42) == ActionResult::NotFound);
  CHECK(lib.returnBook(42) == ActionResult::NotFound);
}

static void testRemove() {
  Library lib;
  int id = lib.addBook("Dune", "Frank Herbert");
  CHECK(lib.removeBook(id));
  CHECK(!lib.removeBook(id));
  CHECK(lib.size() == 0);
}

static void testSaveAndLoadRoundTrip() {
  std::string path = tempFile("lms_p3_roundtrip.txt");
  std::filesystem::remove(path);

  Library original;
  original.addBook("The Hobbit", "J.R.R. Tolkien");
  int id = original.addBook("Dune", "Frank Herbert");
  original.borrowBook(id);
  CHECK(original.saveToFile(path));

  Library loaded;
  LoadResult r = loaded.loadFromFile(path);
  CHECK(r.opened);
  CHECK(r.loaded == 2);
  CHECK(r.skipped == 0);
  CHECK(loaded.size() == 2);
  CHECK(loaded.findById(id)->isBorrowed());
  CHECK(loaded.findById(id)->getTitle() == "Dune");

  // New ids must continue after the highest loaded id (no collisions).
  int newId = loaded.addBook("New", "Author");
  CHECK(newId > id);

  std::filesystem::remove(path);
}

static void testLoadMissingFileKeepsData() {
  Library lib;
  lib.addBook("Keep Me", "Author");
  LoadResult r = lib.loadFromFile(tempFile("lms_p3_does_not_exist.txt"));
  CHECK(!r.opened);
  CHECK(lib.size() == 1); // failed load must not wipe existing data
}

static void testLoadSkipsDamagedLines() {
  std::string path = tempFile("lms_p3_damaged.txt");
  {
    std::ofstream out(path);
    out << "1|Good Book|Good Author|0\n"
        << "garbage line\n"
        << "abc|Bad Id|Author|0\n"
        << "\n"
        << "3|Another Good|Author|1\n"
        << "4|Bad Flag|Author|7\n";
  }
  Library lib;
  LoadResult r = lib.loadFromFile(path);
  CHECK(r.opened);
  CHECK(r.loaded == 2);
  CHECK(r.skipped == 3);
  CHECK(lib.size() == 2);
  std::filesystem::remove(path);
}

static void testSaveToBadPathFails() {
  Library lib;
  lib.addBook("A", "B");
  CHECK(!lib.saveToFile("/this/path/does/not/exist/file.txt"));
}

int main() {
  const std::vector<std::pair<std::string, std::function<void()>>> tests = {
      {"utils", testUtils},
      {"add and find", testAddAndFind},
      {"ids are unique", testIdsAreUnique},
      {"add validation", testAddValidation},
      {"search", testSearch},
      {"sorting", testSorting},
      {"borrow and return", testBorrowAndReturn},
      {"remove", testRemove},
      {"save/load round trip", testSaveAndLoadRoundTrip},
      {"load missing file keeps data", testLoadMissingFileKeepsData},
      {"load skips damaged lines", testLoadSkipsDamagedLines},
      {"save to bad path fails", testSaveToBadPathFails},
  };

  for (const auto &[name, fn] : tests) {
    int before = g_failures;
    fn();
    std::cout << (g_failures == before ? "[PASS] " : "[FAIL] ") << name << "\n";
  }

  std::cout << "\n" << (g_checks - g_failures) << "/" << g_checks << " checks passed\n";
  return g_failures == 0 ? 0 : 1;
}
