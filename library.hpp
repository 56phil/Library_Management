
// Library.hpp
#ifndef LIBRARY_HPP
#define LIBRARY_HPP

#include "book.hpp"
 

class Library {
public:
  bool empty();
  ul size();
  umB getAllBooks() { return books; }
  void addBook(Book &);
  void displayAllBooks();
  bool removeBook(Book &aBook);
  void searchByAuthor(std::string &author, std::vector<Book> &results);
  void searchByTitle(std::string &title, std::vector<Book> &results);
  void searchByISBN(std::string &ispn, std::vector<Book> &results);
  void serialize();
  void deserialize();

  // Serialization function 
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &books;
  }

private:
  umB books;
};

#endif // LIBRARY_HPP
