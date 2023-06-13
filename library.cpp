// library.cpp
#include "library.hpp"

bool Library::empty() { return this->books.empty(); }

ul Library::size() { return this->books.size(); }

void Library::addBook(Book &aBook) { books[aBook.getISBN()] = aBook; }

bool Library::removeBook(Book &aBook) {
  auto it =
      books.find(aBook.getISBN()); // Find the element in the unordered_map

  if (it != books.end()) {
    books.erase(it); // Erase the element using the iterator
    return true;     // Return true indicating successful erasure
  }
  return false; // Return false if element not found
}

void Library::displayAllBooks() {
  for (const auto &aPair : books) {
    Book tmp;
    tmp = aPair.second;
    std::cout << tmp.formatBook();
  }
}

void Library::searchByAuthor(std::string &author, std::vector<Book> &results) {
  results.clear();
  for (auto aPair : books) {
    if (aPair.second.getAuthor() == author) {
      results.emplace_back(aPair.second);
    }
  }
}
void Library::searchByISPN(std::string &ispn, std::vector<Book> &results) {
  results.clear();
  for (auto aPair : books) {
    if (aPair.second.getISBN() == ispn) {
      results.emplace_back(aPair.second);
      break;
    }
  }
}

void Library::searchByTitle(std::string &title, std::vector<Book> &results) {
  results.clear();
  for (auto aPair : books) {
    if (aPair.second.getTitle() == title) {
      results.emplace_back(aPair.second);
      break;
    }
  }
}

void Library::serialize() {
  std::ofstream ofs("library_data.txt");
  boost::archive::text_oarchive oa(ofs);
  oa << *this;
}

void Library::deserialize() {
  std::ifstream ifs("library_data.txt");
  if (ifs.is_open()) {
    boost::archive::text_iarchive ia(ifs);
    ia >> *this;
  }
}
