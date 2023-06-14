// main.cpp
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
*/

#include "book.hpp"
#include "library.hpp"
class Book;     //forward declaration 
class Library;  //forward declaration

#include "lms_project.hpp"

int lastColForThis(WINDOW *, std::string);
int lastRowInWin(WINDOW *);
int maxSizeInVector(vString);
void addBookToLibrary(WINDOW *, WINDOW *, Library &);
void buildAuthor(WINDOW *, Book &, char buff[512]);
void buildBook(WINDOW *, Book &);
void buildISBN(WINDOW *, Book &, char buff[512]);
void buildTitle(WINDOW *, Book &, char buff[512]);
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
void searchUsingISBN(WINDOW *, WINDOW *, Library &);
void searchUsingTitle(WINDOW *, WINDOW *, Library &);
void tag(WINDOW *, std::string);
void tuiLoop(WINDOW *, WINDOW *, Library &);

/* main
   gets: usual cli parameters which are ignored
   returns: int 0
   set curses with two windows, loads library data, enters tui looop, stores
   library, terminates curses, and clears the screen.
 */
int main(int argc, char *argv[]) {
  initscr();
  refresh();
  cbreak();
  noecho();

  int screenWidth, screenHeight;
  getmaxyx(stdscr, screenHeight, screenWidth);

  const int IWIN_HEIGHT(screenHeight / 9); // input window height must be > 1 
  const int IWIN_WIDTH(screenWidth - screenWidth / 5);
  WINDOW *iWin(newwin(IWIN_HEIGHT, IWIN_WIDTH, screenHeight - IWIN_HEIGHT, screenWidth / 10));

  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  attron(COLOR_PAIR(1)); // Enable the color pair
  wbkgd(iWin, COLOR_PAIR(1));

  WINDOW *oWin(newwin(screenHeight - IWIN_HEIGHT, screenWidth, 0, 0));

  box(iWin, 0, 0);

  refresh();
  wrefresh(iWin);
  wrefresh(oWin);

  Library dLibrary;
  dLibrary.deserialize();

  tuiLoop(iWin, oWin, dLibrary);

  dLibrary.serialize();

  endwin();

  std::system("clear");
  return 0;
}

/* tuiLoop

 */
void tuiLoop(WINDOW *iWin, WINDOW *oWin, Library &dLibrary) {
  displayWelcome(oWin);
  wprintw(iWin, "Helllo!!!");
  wrefresh(iWin);
  char ch(' ');
  do {
    werase(iWin);
    wrefresh(oWin);
    curs_set(0);
    move(0, 0);
    ch = wgetch(iWin);
    // curs_set(0);
    // wrefresh(iWin);
    if (ch != ERR) {
      if (ch == 'a') {
        addBookToLibrary(iWin, oWin, dLibrary);
      } else if (ch == 'c') {
        displayCatalog(oWin, dLibrary);
      } else if (ch == 'i') {
        searchUsingISBN(iWin, oWin, dLibrary);
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
  } while (true);
}

void addBookToLibrary(WINDOW *iWin, WINDOW *oWin, Library &aLibrary) {
  Book aBook;
  buildBook(iWin, aBook);
  aLibrary.addBook(aBook);
  werase(oWin);
  displayBook(oWin, aBook, lastRowInWin(oWin));
  tag(oWin, "ADDED");
}

void removeBookFromLibrary(WINDOW *iWin, WINDOW *oWin, Library &aLibrary) {
  Book aBook;
  char buff[512];
  buildISBN(iWin, aBook, buff);
  werase(oWin);
  displayBook(oWin, aBook, lastRowInWin(oWin));
  if (aLibrary.removeBook(aBook)) {
    tag(oWin, "REMOVED");
  } else {
    tag(oWin, "NOT FOUND");
  }
}

void searchUsingISBN(WINDOW *iWin, WINDOW *oWin, Library &aLibrary) {
  Book aBook;
  char buff[512];
  buildISBN(iWin, aBook, buff);
  std::string tmp = aBook.getISBN();
  vBook results;
  aLibrary.searchByISBN(tmp, results);
  werase(oWin);
  if (results.empty()) {
    displayBook(oWin, aBook, lastRowInWin(oWin));
    tag(oWin, "NOT FOUND");
  } else {
    displayBook(oWin, results.front(), lastRowInWin(oWin));
  }
}

void searchUsingTitle(WINDOW *iWin, WINDOW *oWin, Library &aLibrary) {
  Book aBook;
  char buff[512];
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
  char buff[512];
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
}

void buildTitle(WINDOW *iWin, Book &aBook, char buff[512]) {
  werase(iWin);
  mvwprintw(iWin, 1, 3, "title: ");
  // wrefresh(iWin);
  echo();
  curs_set(1);
  wgetstr(iWin, buff);
  noecho();
  curs_set(0);
  aBook.setTitle(buff);
  werase(iWin);
  displayBook(iWin, aBook, 2);
  wrefresh(iWin);
}

void buildAuthor(WINDOW *iWin, Book &aBook, char buff[512]) {
  werase(iWin);
  mvwprintw(iWin, 1, 2, "author: ");
  // wrefresh(iWin);
  echo();
  curs_set(1);
  wgetstr(iWin, buff);
  noecho();
  curs_set(0);
  aBook.setAuthor(buff);
  werase(iWin);
  displayBook(iWin, aBook, 2);
  wrefresh(iWin);
}

void buildISBN(WINDOW *iWin, Book &aBook, char buff[512]) {
  werase(iWin);
  mvwprintw(iWin, 1, 4, "ISBN: ");
  // wrefresh(iWin);
  echo();
  curs_set(1);
  wgetstr(iWin, buff);
  noecho();
  curs_set(0);
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
  std::string h0("Help Screen");
  std::string h1("Choose one of the availible options from the above menu. ");
  std::string h2("e.g. 'c' will display the catalog, 'a' to add a book, etc.");
  werase(oWin);
  displayTextAtCenter(oWin, h0, 1);
  displayMenu(oWin);
  displayTextAtCenter(oWin, h1, lastRowInWin(oWin) - 1);
  displayTextAtCenter(oWin, h2, lastRowInWin(oWin));
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
    vString lines;
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

/* displayMenu
   gets: WINDOW pointer
   returns: nothing
   objective: display options - used by displayHelp & displayWelcome
 */
void displayMenu(WINDOW *aWin) {
  std::string m0("MENU");
  std::string m1("h: Help");
  std::string m2("c: Catalog");
  std::string m3("i: Search by Isbn");
  std::string m4("t: search by Title");
  std::string m5("s: Search by author");
  std::string m6("a: Add a book");
  std::string m7("r: Remove a book");
  std::string m8("x: eXit Library");
  vString menuDetail({m1, m2, m3, m4, m5, m6, m7, m8});
  int maxRows, maxCols;
  getmaxyx(aWin, maxRows, maxCols);

  displayTextAtCenter(aWin, m0, 3);
  unsigned r(5), c((maxCols - maxSizeInVector(menuDetail)) >> 1);
  for (auto s : menuDetail) {
    mvwprintw(aWin, r++, c, s.c_str());
  }
}

/* lastRowInWin
   gets: WINDOW pointer
   returns: int
   objective: return the number of last useable row
 */
int lastRowInWin(WINDOW *aWin) {
  int maxRows, maxCols;
  getmaxyx(aWin, maxRows, maxCols);
  return maxRows - 1;
}

/* maxSizeInVector
   gets: vector of strings
   returns: int
   objective: return the length of longest string in vector
 */
int maxSizeInVector(vString sv) {
  int sz(0);
  for (auto s : sv) {
    sz = sz < s.size() ? s.size() : sz;
  }
  return sz;
}

/* lastColForThis
   gets: WINDOW pointer, string
   returns: int
   objective: return the starting position of given string so that right
   justified
 */
int lastColForThis(WINDOW *aWin, std::string str) {
  int maxRows, maxCols;
  getmaxyx(aWin, maxRows, maxCols);
  return maxCols - str.size();
}

/* tag
   gets: WINDOW pointer, string
   returns: nothing
   objective: put the string on the far right side of the bottom row.
 */
void tag(WINDOW *aWin, std::string str) {
  mvwprintw(aWin, lastRowInWin(aWin), lastColForThis(aWin, str), str.c_str());
}

/* displyBookVector
   gets: WINDOW pointer
   vector of Book objects
   returns: nothing
   objective: put the contents of books on the screen
   method: locates the center, displays the header, traverses the vector
   displaying title, author, and ISBN using the displayBook function.
*/
void displyBookVector(WINDOW *aWin, vBook &books) {
  int maxRows, maxCols, r(3);
  Book aBook;
  getmaxyx(aWin, maxRows, maxCols);
  int cen((maxCols >> 1) - 6);
  int lef(cen - 33), rig(cen + 49);
  lef = lef < 0 ? 0 : lef;
  rig = rig + 11 > maxCols ? maxCols - 11 : rig;
  aBook.clear();
  aBook.setAuthor("Author");
  aBook.setTitle("Title");
  aBook.setISBN("ISBN");
  displayBook(aWin, aBook, r++, lef, cen, rig);
  for (auto aBook : books) {
    displayBook(aWin, aBook, r++, lef, cen, rig);
  }
}
