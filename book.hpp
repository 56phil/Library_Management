// Book.hpp
#ifndef BOOK_HPP
#define BOOK_HPP

#include "lms_project.hpp"

class Book {
public:
  Book(const std::string &title, const std::string &author,
       const std::string &isbn);
  Book();
  ~Book();
  std::string formatBook(int = 22, int = 22, int = 22);
  std::string getAuthor() const;
  std::string getISBN() const;
  std::string getTitle() const;
  void clear();
  void setAuthor(const std::string &author);
  void setISBN(const std::string &isbn);
  void setTitle(const std::string &title);

  bool operator<(const Book &other) const {
    if (author == other.author) {
      if (title == other.title) {
        return isbn < other.isbn;
      }
      return title < other.title;
    }
    return author < other.author;
  }

  friend class boost::serialization::access;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &title;
    ar &author;
    ar &isbn;
  }

private:
  std::string title;
  std::string author;
  std::string isbn;
};
typedef std::vector<Book> vBook;

namespace std {
template <> struct hash<Book> {
  size_t operator()(const Book &book) const {
    return ((hash<string>()(book.getTitle()) ^
             (hash<string>()(book.getAuthor()) << 1) ^
             (hash<string>()(book.getISBN()) << 2)) >>
            2);
  }
};
} // namespace std

#endif // BOOK_HPP
