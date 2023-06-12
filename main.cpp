/*

# Project Title: Library Management System

## Objective:
The objective of this project is to create a simple Library Management System
(LMS) using C++. The LMS will be a console application with a menu that allows
the user to navigate through different options.

## Requirements:

1. **Book Class**: Create a `Book` class that has private attributes for the
title, author, and ISBN number. Include appropriate accessor (get) and mutator
(set) methods.

2. **Library Class**: Create a `Library` class that stores a collection of
`Book` objects. It should have the following functionalities:
    - Add a book to the library.
    - Remove a book from the library.
    - Search for a book by title.
    - Search for a book by author.
    - Display all books in the library.

3. **User Interface**: Create a user interface that allows the user to select
from the following options:
    - Add a book.
    - Remove a book.
    - Search for a book.
    - Display all books.
    - Exit the program.

## Instructions:

1. Start by designing your `Book` and `Library` classes. Think about what
methods and data members they will need.

2. Implement the `Book` class. Make sure you include methods to get and set each
data member.

3. Implement the `Library` class. This class should use a data structure (like a
vector or list) to store `Book` objects.

4. Create the user interface. This can be a simple text menu that is displayed
in the console.

5. Test your program thoroughly. Make sure all options work and handle potential
errors gracefully.

## Submission:

Please submit your source code files along with a brief document outlining your
approach, challenges faced, and how you overcame them. Also, include
instructions on how to run your program.

## Due Date:

The project is due two weeks from today. (22JUN)

Good luck! I'm looking forward to seeing your Library Management System.
*/

#include "book.hpp"
#include "library.hpp"
#include <_ctype.h>
#include <algorithm>
#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

typedef std::vector<Book> vBook;

int lastColForThis(WINDOW *, std::string);
int lastRowInWin(WINDOW *);
int maxSizeInVector(std::vector<std::string> sv);
void addBookToLibrary(WINDOW *, WINDOW *, Library &);
void buildAuthor(WINDOW *, Book &, char buff[256]);
void buildBook(WINDOW *, Book &);
void buildISBN(WINDOW *, Book &, char buff[256]);
void buildTitle(WINDOW *, Book &, char buff[256]);
void clrRow(WINDOW *aWindow, unsigned);
void displayBook(WINDOW *aWin, Book &aBook, int r = 5, int tPos = 1,
                 int aPos = 23, int iPos = 45);
void displyBookVector(WINDOW *, vBook &);
void displayCatalog(WINDOW *, Library &);
void displayHelp(WINDOW *);
void displayMenu(WINDOW *);
void displayTextAtCenter(WINDOW *, std::string &, int);
void displayWelcome(WINDOW *);
void prompt(WINDOW *, const std::string, std::string &);
void removeBookFromLibrary(WINDOW *, WINDOW *, Library &);
void searchUsingAuthor(WINDOW *, WINDOW *, Library &);
void searchUsingTitle(WINDOW *, WINDOW *, Library &);
void tag(WINDOW *, std::string);
void tuiLoop(WINDOW *, WINDOW *, Library &);

int main(int argc, char *argv[]) {
  initscr();
  refresh();
  cbreak();

  int screenWidth, screenHeight;
  getmaxyx(stdscr, screenHeight, screenWidth);

  // mvprintw(1, 5, "w: %d  h: %d", screenWidth, screenHeight);
  // refresh();

  const int IWIN_HEIGHT(4);
  WINDOW *iWin(newwin(IWIN_HEIGHT, screenWidth, screenHeight - IWIN_HEIGHT, 0));
  wrefresh(iWin);

  WINDOW *oWin(newwin(screenHeight - IWIN_HEIGHT, screenWidth, 0, 0));
  displayWelcome(oWin);
  wrefresh(oWin);

  Library dLibrary;
  dLibrary.deserialize();

  tuiLoop(iWin, oWin, dLibrary);

  dLibrary.serialize();

  endwin();

  std::system("clear");
  return 0;
}

void tuiLoop(WINDOW *iWin, WINDOW *oWin, Library &dLibrary) {
  bool firstTime(true);
  char ch(' ');
  do {
    if (firstTime) {
      firstTime = false;
      displayWelcome(oWin);
    } else {
      cbreak();
      werase(iWin);
      // mvwprintw(iWin, 1, 3, ": ");
      wrefresh(iWin);
      wrefresh(oWin);
      curs_set(1);
      ch = getch();
      curs_set(0);
      wrefresh(iWin);
      if (ch != ERR) {
        if (ch == 'a') {
          addBookToLibrary(iWin, oWin, dLibrary);
        } else if (ch == 'g') {
          displayWelcome(oWin);
        } else if (ch == 'c') {
          displayCatalog(oWin, dLibrary);
        } else if (ch == 'r') {
          removeBookFromLibrary(iWin, oWin, dLibrary);
        } else if (ch == 't') {
          searchUsingTitle(iWin, oWin, dLibrary);
        } else if (ch == 's') {
          searchUsingAuthor(iWin, oWin, dLibrary);
        } else if (ch == 'x') {
          break;
        } else if (ch == 'h') {
          displayHelp(oWin);
        } else {
          displayHelp(oWin);
        }
      }
    }
  } while (true);
}

void addBookToLibrary(WINDOW *iWin, WINDOW *oWin, Library &aLibrary) {
  Book aBook;
  buildBook(iWin, aBook);
  aLibrary.addBook(aBook);
  werase(oWin);
  displayBook(oWin, aBook, lastRowInWin(oWin));
  tag(oWin, "ADDED");
  wrefresh(oWin);
  curs_set(0);
  move(0, 0);
}

void removeBookFromLibrary(WINDOW *iWin, WINDOW *oWin, Library &aLibrary) {
  Book aBook;
  char buff[256];
  buildISBN(iWin, aBook, buff);
  werase(oWin);
  displayBook(oWin, aBook, lastRowInWin(oWin));
  if (aLibrary.removeBook(aBook)) {
    tag(oWin, "REMOVED");
  } else {
    tag(oWin, "NOT FOUND");
  }
  wrefresh(oWin);
  curs_set(0);
  move(0, 0);
}

void searchUsingTitle(WINDOW *iWin, WINDOW *oWin, Library &aLibrary) {
  Book aBook;
  char buff[256];
  buildTitle(iWin, aBook, buff);
  std::string tmp = aBook.getTitle();
  vBook results;
  aLibrary.searchByTitle(tmp, results);
  werase(oWin);
  if (results.empty()) {
    displayBook(oWin, aBook, lastRowInWin(oWin));
    tag(oWin, "NOT FOUND");
  } else {
    displayBook(oWin, results.front(), lastRowInWin(oWin));
  }
}

void searchUsingAuthor(WINDOW *iWin, WINDOW *oWin, Library &aLibrary) {
  char buff[256];
  Book aBook;
  buildAuthor(iWin, aBook, buff);
  std::string tmp = aBook.getAuthor();
  vBook results;
  aLibrary.searchByAuthor(tmp, results);
  if (results.empty()) {
    mvwprintw(oWin, lastRowInWin(oWin), 5, "No books written by %s found.",
              aBook.getAuthor().c_str());
  } else {
    std::sort(results.begin(), results.end(),
              [](const Book &book1, const Book &book2) {
                return book1.getTitle() < book2.getTitle();
              });
    werase(oWin);
    std::string s0("Books by " + results.front().getAuthor() + ":");
    displayTextAtCenter(oWin, s0, 1);
    displyBookVector(oWin, results);
  }
  wrefresh(oWin);
  curs_set(0);
  move(0, 0);
}

void buildTitle(WINDOW *iWin, Book &aBook, char buff[256]) {
  werase(iWin);
  mvwprintw(iWin, 1, 3, "title: ");
  wrefresh(iWin);
  wgetstr(iWin, buff);
  aBook.setTitle(buff);
  werase(iWin);
  displayBook(iWin, aBook, 2);
  wrefresh(iWin);
}

void buildAuthor(WINDOW *iWin, Book &aBook, char buff[256]) {
  werase(iWin);
  mvwprintw(iWin, 1, 2, "author: ");
  wrefresh(iWin);
  wgetstr(iWin, buff);
  aBook.setAuthor(buff);
  werase(iWin);
  displayBook(iWin, aBook, 2);
  wrefresh(iWin);
}

void buildISBN(WINDOW *iWin, Book &aBook, char buff[256]) {
  werase(iWin);
  mvwprintw(iWin, 1, 4, "ISBN: ");
  wrefresh(iWin);
  wgetstr(iWin, buff);
  aBook.setISBN(buff);
  werase(iWin);
  displayBook(iWin, aBook, 2);
  wrefresh(iWin);
}

void buildBook(WINDOW *iWin, Book &aBook) {
  char buff[512];
  buildTitle(iWin, aBook, buff);
  buildAuthor(iWin, aBook, buff);
  buildISBN(iWin, aBook, buff);
}

void displayHelp(WINDOW *oWin) {
  werase(oWin);
  std::string h1("Help Screen");
  displayTextAtCenter(oWin, h1, 1);
  displayMenu(oWin);
  std::string h0("Choose one of the availible options from the above list. "
                 "e.g. 'c' will display the catalog of books.");
}

/*
gets: string, int
returns: nonthing
objective: put a string at the center on the current line
method: calculate & print number of spaces needed for padding.
print the string. When the string is too large, the string is spit
into two strings of roughly equal size.
*/
void displayTextAtCenter(WINDOW *aWin, std::string &str, int onRow) {

  // Get the maximum rows and columns of the window
  int maxRows, maxCols;
  getmaxyx(aWin, maxRows, maxCols);
  onRow = onRow > maxRows ? maxRows : onRow;

  if (str.size() > maxCols) {
    std::vector<std::string> lines;
    lines.clear();
    int sPos(str.substr(0, str.size() / 2).find_last_of(' '));
    if (sPos == std::string::npos) {
      sPos = str.size() / 2;
    }
    std::string tmpStr(str.substr(0, sPos));
    displayTextAtCenter(aWin, tmpStr, onRow);
    str = str.substr(sPos + 1, onRow);
    displayTextAtCenter(aWin, tmpStr, ++onRow);
  } else {
    int xPos((maxCols - str.size()) / 2);
    // clrRow(aWin, onRow);
    mvwprintw(aWin, onRow, xPos, str.c_str());
  }
  wrefresh(aWin);
}

void clrRow(WINDOW *aWin, unsigned row) {
  unsigned col(0);
  int maxRows, maxCols;
  getmaxyx(aWin, maxRows, maxCols);
  mvwprintw(aWin, row, col, "%*s", maxCols - 1, "");
}

void displayBook(WINDOW *aWin, Book &b, int r, int tPos, int aPos, int iPos) {
  mvwprintw(aWin, r, tPos, b.getTitle().c_str());
  mvwprintw(aWin, r, aPos, b.getAuthor().c_str());
  mvwprintw(aWin, r, iPos, b.getISBN().c_str());
}

void displayCatalog(WINDOW *aWin, Library &aLibrary) {
  int r(1);
  wclear(aWin);
  wrefresh(aWin);
  std::string t0("Your Catalog:");
  displayTextAtCenter(aWin, t0, r);
  r += 2;
  auto dcatalog(aLibrary.getAllBooks());
  vBook results;
  for (auto aPair : dcatalog) {
    results.emplace_back(aPair.second);
  }
  displyBookVector(aWin, results);
}

void displayWelcome(WINDOW *aWin) {
  werase(aWin);
  std::string l1("Welcome to Your Library");
  displayTextAtCenter(aWin, l1, 1);
  displayMenu(aWin);
}

void displayMenu(WINDOW *aWin) {
  std::string m0("MENU");
  std::string m1("H: Help");
  std::string m2("C: Catalog");
  std::string m3("T: Search by Title");
  std::string m4("S: Search by Author");
  std::string m5("A: Add a book");
  std::string m6("R: Remove a book");
  std::string m7("X: Exit Library");
  std::vector<std::string> menuDetail({m1, m2, m3, m4, m5, m6, m7});
  int maxRows, maxCols;
  getmaxyx(aWin, maxRows, maxCols);

  displayTextAtCenter(aWin, m0, 3);
  unsigned r(5), c((maxCols - maxSizeInVector(menuDetail)) >> 1);
  for (auto s : menuDetail) {
    mvwprintw(aWin, r++, c, s.c_str());
  }
  wrefresh(aWin);
  curs_set(0);
  move(0, 0);
}

int lastRowInWin(WINDOW *aWin) {
  int maxRows, maxCols;
  getmaxyx(aWin, maxRows, maxCols);
  return maxRows - 1;
}

int maxSizeInVector(std::vector<std::string> sv) {
  int sz(0);
  for (auto s : sv) {
    sz = sz < s.size() ? s.size() : sz;
  }
  return sz;
}

int lastColForThis(WINDOW *aWin, std::string str) {
  int maxRows, maxCols;
  getmaxyx(aWin, maxRows, maxCols);
  return maxCols - str.size();
}

void tag(WINDOW *aWin, std::string str) {
  mvwprintw(aWin, lastRowInWin(aWin), lastColForThis(aWin, str), str.c_str());
}

void displyBookVector(WINDOW *aWin, vBook &books) {
  int maxRows, maxCols, r(3);
  Book aBook;
  getmaxyx(aWin, maxRows, maxCols);
  int cen(maxCols >> 1);
  int lef(cen - 33), rig(cen + 49);
  aBook.clear();
  aBook.setAuthor("Author");
  aBook.setTitle("Title");
  aBook.setISBN("ISBN");
  displayBook(aWin, aBook, r++, lef, cen, rig);
  for (auto aBook : books) {
    displayBook(aWin, aBook, r++, lef, cen, rig);
  }
  wrefresh(aWin);
  curs_set(0);
  move(0, 0);
}
