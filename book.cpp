// Book.cpp

#include "book.hpp"
#include "lms_project.hpp"

Book::Book(const std::string &title, const std::string &author,
           const std::string &isbn)
    : title(title), author(author), isbn(isbn) {}

Book::Book() { Book::clear(); }

Book::~Book() { Book::clear(); }

void Book::clear() {
  title.clear();
  author.clear();
  isbn.clear();
}

std::string Book::getTitle() const { return title; }

std::string Book::getAuthor() const { return author; }

std::string Book::getISBN() const { return isbn; }

void Book::setTitle(const std::string &newTitle) { title = newTitle; }

void Book::setAuthor(const std::string &newAuthor) { author = newAuthor; }

void Book::setISBN(const std::string &newISBN) { isbn = newISBN; }

std::string Book::formatBook(int l, int c, int r) {
  std::stringstream sst;
  sst << std::right << std::setw(l) << Book::title << std::right << std::setw(c)
      << Book::author << std::right << std::setw(r) << Book::isbn << std::endl;
  return sst.str();
}
