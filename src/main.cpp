#include <iostream>
#include <limits>
#include "Library.h"

void printMenu() {
    std::cout << "\n===== Library Management System =====\n"
              << "1. Add book\n"
              << "2. List all books\n"
              << "3. Search by title\n"
              << "4. Borrow book\n"
              << "5. Return book\n"
              << "6. Remove book\n"
              << "7. Save to file\n"
              << "8. Load from file\n"
              << "0. Exit\n"
              << "Choose an option: ";
}

// Clears bad input from std::cin so the program doesn't get stuck in a loop.
void clearInputError() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Library library;
    int choice = -1;

    while (choice != 0) {
        printMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            clearInputError();
            std::cout << "Invalid input, please enter a number.\n";
            continue;
        }
        std::cin.ignore(); // discard leftover newline before getline calls

        switch (choice) {
            case 1: {
                std::string title, author;
                std::cout << "Title: ";
                std::getline(std::cin, title);
                std::cout << "Author: ";
                std::getline(std::cin, author);
                library.addBook(title, author);
                break;
            }
            case 2:
                library.listBooks();
                break;
            case 3: {
                std::string keyword;
                std::cout << "Keyword: ";
                std::getline(std::cin, keyword);
                library.searchByTitle(keyword);
                break;
            }
            case 4: {
                int id;
                std::cout << "Book id to borrow: ";
                std::cin >> id;
                std::cout << (library.borrowBook(id) ? "Borrowed.\n" : "Could not borrow (check id).\n");
                break;
            }
            case 5: {
                int id;
                std::cout << "Book id to return: ";
                std::cin >> id;
                std::cout << (library.returnBook(id) ? "Returned.\n" : "Could not return (check id).\n");
                break;
            }
            case 6: {
                int id;
                std::cout << "Book id to remove: ";
                std::cin >> id;
                std::cout << (library.removeBook(id) ? "Removed.\n" : "Could not remove (check id).\n");
                break;
            }
            case 7: {
                std::string filename;
                std::cout << "Filename to save to (e.g. library_data.txt): ";
                std::getline(std::cin, filename);
                library.saveToFile(filename);
                break;
            }
            case 8: {
                std::string filename;
                std::cout << "Filename to load from: ";
                std::getline(std::cin, filename);
                library.loadFromFile(filename);
                break;
            }
            case 0:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Unknown option, try again.\n";
        }
    }

    return 0;
}
