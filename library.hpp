
// Library.hpp
#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include "book.hpp"
#include <string>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, Book> umb;
typedef unsigned long ul;

class Library {
public:
  bool empty();
  ul size();
  umb getAllBooks() { return books; }
  void addBook(Book &);
  void displayAllBooks();
  bool removeBook(Book &aBook);
  void searchByAuthor(std::string &author, std::vector<Book> &results);
  void searchByTitle(std::string &title, std::vector<Book> &results);
  void searchByISPN(std::string &ispn, std::vector<Book> &results);
  void serialize();
  void deserialize();

  // Serialization function
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &books;
  }

private:
  umb books;
};

#endif // LIBRARY_HPP
