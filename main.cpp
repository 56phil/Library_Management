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
#include <cstdio>
#include <ncurses.h>
#include <string>
#include <vector>
class Book;    // forward declaration
class Library; // forward declaration

#include "lms_project.hpp"

int lastColForThis(WINDOW *, std::string);
int lastRowInWin(WINDOW *);
int maxSizeInVector(vString);
int midColInWin(WINDOW *);
int midRowInWin(WINDOW *);
void addBookToLibrary(WINDOW *, WINDOW *, WINDOW *, Library &);
void buildAuthor(WINDOW *, Book &, char buff[512]);
void buildBook(WINDOW *, Book &);
void buildISBN(WINDOW *, Book &, char buff[512]);
void buildTitle(WINDOW *, Book &, char buff[512]);
void clrRow(WINDOW *aWindow, unsigned);
void displayBook(WINDOW *aWin, Book &aBook, int r = 5, int tPos = 1,
                 int aPos = 23, int iPos = 45);
void displyBookVector(WINDOW *, WINDOW *, vBook &);
void displayCatalog(WINDOW *, WINDOW *, Library &);
void displayCurrentPage(WINDOW *, WINDOW *, vBook, int, int, int, int);
void displayHelp(WINDOW *, WINDOW *);
void displayMenu(WINDOW *);
void displayPaginationMessage(WINDOW *, int, int, int);
void displayTextAtCenter(WINDOW *, std::string &, int);
void displayWelcome(WINDOW *);
void paginate(const vBook &, vvBook &, int);
void prompt(WINDOW *, const std::string, std::string &);
void removeBookFromLibrary(WINDOW *, WINDOW *, WINDOW *, Library &);
void resetMessageArea(WINDOW *);
void resetWorkArea(WINDOW *);
void searchUsingAuthor(WINDOW *, WINDOW *, WINDOW *, Library &);
void searchUsingISBN(WINDOW *, WINDOW *, WINDOW *, Library &);
void searchUsingTitle(WINDOW *, WINDOW *, WINDOW *, Library &);
void tag(WINDOW *, std::string, int r = 1);
void tuiLoop(WINDOW *, WINDOW *, WINDOW *, Library &);

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
  erase();

  int screenWidth, screenHeight;
  getmaxyx(stdscr, screenHeight, screenWidth);

  const int IWIN_HEIGHT(screenHeight / 9 - 1);
  const int IWIN_WIDTH(screenWidth - screenWidth / 5);

  const int MWIN_HEIGHT(5);
  const int MWIN_WIDTH(IWIN_WIDTH);

  const int OWIN_HEIGHT(screenHeight - MWIN_HEIGHT - IWIN_HEIGHT);
  const int OWIN_WIDTH(screenWidth - screenWidth / 11);

  WINDOW *iWin(newwin(IWIN_HEIGHT, IWIN_WIDTH, screenHeight - IWIN_HEIGHT,
                      screenWidth / 10)); // input window
  WINDOW *mWin(newwin(MWIN_HEIGHT, MWIN_WIDTH,
                      screenHeight - IWIN_HEIGHT - IWIN_HEIGHT,
                      screenWidth / 10)); // message window
  WINDOW *oWin(
      newwin(OWIN_HEIGHT, OWIN_WIDTH, 0, screenWidth / 22)); // output window

  if (has_colors()) {
    // Enable color functionality
    start_color();

    // Define color pairs
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    wbkgd(iWin, COLOR_PAIR(1));
    wbkgd(oWin, COLOR_PAIR(1));
    wbkgd(mWin, COLOR_PAIR(3));

    // Apply color pair to text
    // attron(COLOR_PAIR(1));
    // printw("Hello, ncurses with color!");
    // attroff(COLOR_PAIR(1));

    refresh();
    // getch();
  }

  wclear(iWin);
  wclear(oWin);
  wclear(mWin);

  refresh();
  wrefresh(iWin);
  wrefresh(oWin);
  wrefresh(mWin);

  Library dLibrary;
  dLibrary.deserialize();

  tuiLoop(iWin, oWin, mWin, dLibrary);

  dLibrary.serialize();

  endwin();

  std::system("clear");
  return 0;
}

/* tuiLoop

 */
void tuiLoop(WINDOW *iWin, WINDOW *oWin, WINDOW *mWin, Library &dLibrary) {
  char ch(' ');
  displayWelcome(oWin);
  do {
    resetMessageArea(mWin);
    resetWorkArea(iWin);
    wrefresh(oWin);
    curs_set(0);
    move(0, 0);
    ch = wgetch(iWin);
    if (ch != ERR) {
      werase(mWin);
      werase(iWin);
      if (ch == 'a') {
        addBookToLibrary(iWin, oWin, mWin, dLibrary);
      } else if (ch == 'c') {
        displayCatalog(oWin, mWin, dLibrary);
      } else if (ch == 'i') {
        searchUsingISBN(iWin, oWin, mWin, dLibrary);
      } else if (ch == 'r') {
        removeBookFromLibrary(iWin, oWin, mWin, dLibrary);
      } else if (ch == 't') {
        searchUsingTitle(iWin, oWin, mWin, dLibrary);
      } else if (ch == 's') {
        searchUsingAuthor(iWin, oWin, mWin, dLibrary);
      } else if (ch == 'x') {
        break;
      } else if (ch == 'h') {
        displayHelp(oWin, mWin);
      } else {
        displayHelp(oWin, mWin);
      }
    }
  } while (true);
}

void addBookToLibrary(WINDOW *iWin, WINDOW *oWin, WINDOW *mWin,
                      Library &aLibrary) {
  Book aBook;
  buildBook(iWin, aBook);
  aLibrary.addBook(aBook);
  werase(iWin);
  displayBook(mWin, aBook, midRowInWin(mWin), 3);
  tag(mWin, "ADDED", midRowInWin(mWin));
  werase(iWin);
  resetWorkArea(iWin);
  resetMessageArea(mWin);
}

void removeBookFromLibrary(WINDOW *iWin, WINDOW *oWin, WINDOW *mWin,
                           Library &aLibrary) {
  Book aBook;
  char buff[512];
  buildISBN(iWin, aBook, buff);
  werase(oWin);
  werase(iWin);
  std::string tmp = aBook.getISBN();
  vBook results;
  aLibrary.searchByISBN(tmp, results);
  if (results.empty()) {
    mvwprintw(mWin, midRowInWin(mWin), 2,
              "Book with ISBN of %s not in catalog.", aBook.getISBN().c_str());
  } else if (aLibrary.removeBook(aBook)) {
    displayBook(mWin, results.front(), midRowInWin(mWin));
    tag(mWin, "REMOVED", midRowInWin(mWin));
    // } else {
    //   displayBook(mWin, aBook, midRowInWin(mWin));
    //   attroff(COLOR_PAIR(1));
    //   attron(COLOR_PAIR(2)); // Enable the color pair
    //   // tag(mWin, "NOT FOUND", midRowInWin(mWin));
    //   attroff(COLOR_PAIR(2)); // disable the color pair
  }
  wrefresh(mWin);
}

void searchUsingISBN(WINDOW *iWin, WINDOW *oWin, WINDOW *mWin,
                     Library &aLibrary) {
  Book aBook;
  char buff[512];
  buildISBN(iWin, aBook, buff);
  std::string tmp = aBook.getISBN();
  vBook results;
  aLibrary.searchByISBN(tmp, results);
  werase(oWin);
  if (results.empty()) {
    displayBook(oWin, aBook, lastRowInWin(oWin));
    tag(mWin, "NOT FOUND", midRowInWin(mWin));
  } else {
    displyBookVector(oWin, mWin, results);
  }
}

void searchUsingTitle(WINDOW *iWin, WINDOW *oWin, WINDOW *mWin,
                      Library &aLibrary) {
  Book aBook;
  char buff[512];
  buildTitle(iWin, aBook, buff);
  std::string tmp = aBook.getTitle();
  vBook results;
  aLibrary.searchByTitle(tmp, results);
  werase(oWin);
  if (results.empty()) {
    displayBook(oWin, aBook, lastRowInWin(oWin));
    tag(mWin, "NOT FOUND", midRowInWin(mWin));
  } else {
    displayBook(oWin, results.front(), lastRowInWin(oWin));
  }
}

void searchUsingAuthor(WINDOW *iWin, WINDOW *oWin, WINDOW *mWin,
                       Library &aLibrary) {
  char buff[512];
  Book aBook;
  buildAuthor(iWin, aBook, buff);
  std::string tmp = aBook.getAuthor();
  vBook results;
  aLibrary.searchByAuthor(tmp, results);
  werase(iWin);
  werase(mWin);
  if (results.empty()) {
    mvwprintw(iWin, midRowInWin(iWin), 5, "No books written by %s found.",
              aBook.getAuthor().c_str());
  } else {
    std::sort(results.begin(), results.end(),
              [](const Book &book1, const Book &book2) {
                return book1.getTitle() < book2.getTitle();
              });
    werase(oWin);
    std::string s0("Books by " + results.front().getAuthor() + ":");
    displayTextAtCenter(oWin, s0, 1);
    displyBookVector(oWin, mWin, results);
  }
}

void buildTitle(WINDOW *iWin, Book &aBook, char buff[512]) {
  mvwprintw(iWin, 1, 3, "title: ");
  resetWorkArea(iWin);
  echo();
  curs_set(1);
  wmove(iWin, 1, 10);
  wgetstr(iWin, buff);
  noecho();
  curs_set(0);
  aBook.setTitle(buff);
  displayBook(iWin, aBook, midRowInWin(iWin), 3);
  resetWorkArea(iWin);
}

void buildAuthor(WINDOW *iWin, Book &aBook, char buff[512]) {
  mvwprintw(iWin, 1, 2, "author: ");
  wmove(iWin, 1, 10);
  wclrtoeol(iWin);
  resetWorkArea(iWin);
  echo();
  curs_set(1);
  wmove(iWin, 1, 10);
  wgetstr(iWin, buff);
  noecho();
  curs_set(0);
  aBook.setAuthor(buff);
  displayBook(iWin, aBook, midRowInWin(iWin), 3);
  resetWorkArea(iWin);
}

void buildISBN(WINDOW *iWin, Book &aBook, char buff[512]) {
  wmove(iWin, 1, 1);
  wclrtoeol(iWin);
  mvwprintw(iWin, 1, 4, "ISBN: ");
  resetWorkArea(iWin);
  echo();
  curs_set(1);
  wmove(iWin, 1, 10);
  wgetstr(iWin, buff);
  noecho();
  curs_set(0);
  aBook.setISBN(buff);
  displayBook(iWin, aBook, midRowInWin(iWin), 3);
  resetWorkArea(iWin);
}

void buildBook(WINDOW *iWin, Book &aBook) {
  char buff[512];
  wclear(iWin);
  resetWorkArea(iWin);
  buildTitle(iWin, aBook, buff);
  buildAuthor(iWin, aBook, buff);
  buildISBN(iWin, aBook, buff);
}

void displayHelp(WINDOW *aWin, WINDOW *bWin) {
  std::string h0("Help Screen");
  std::string h1("Choose one of the availible options from the above menu. ");
  std::string h2("e.g. 'c' will display the catalog, 'a' to add a book, etc.");
  std::string h3("To quit the program, press the 'x' key.");
  werase(aWin);
  werase(bWin);
  displayTextAtCenter(aWin, h0, 1);
  displayMenu(aWin);
  displayTextAtCenter(bWin, h1, 1);
  displayTextAtCenter(bWin, h2, 2);
  displayTextAtCenter(bWin, h3, 3);
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

void displayCatalog(WINDOW *aWin, WINDOW *mWin, Library &aLibrary) {
  int r(1);
  werase(aWin);
  wrefresh(aWin);
  std::string t0("Your Catalog:");
  displayTextAtCenter(aWin, t0, r);
  r += 2;
  auto dcatalog(aLibrary.getAllBooks());
  vBook results;
  for (auto aPair : dcatalog) {
    results.emplace_back(aPair.second);
  }
  displyBookVector(aWin, mWin, results);
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
  return maxRows;
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
  return maxCols - str.size() - 1;
}

/* tag
   gets: WINDOW pointer, string
   returns: nothing
   objective: put the string on the far right side of the bottom row.
 */
void tag(WINDOW *aWin, std::string str, int r) {
  mvwprintw(aWin, r, lastColForThis(aWin, str), str.c_str());
}

/* displyBookVector
   gets: WINDOW pointer
   vector of Book objects
   returns: nothing
   objective: put the contents of books on the screen
   method: locates the center, displays the header, traverses the vector
   displaying title, author, and ISBN using the displayBook function.
*/
void displyBookVector(WINDOW *oWin, WINDOW *mWin, vBook &books) {
  int maxRows, maxCols;
  getmaxyx(oWin, maxRows, maxCols);
  int cen((maxCols >> 1) - 6);
  int lef(cen - 33);
  lef = lef < 0 ? 0 : lef;
  int rig(cen + 49);
  rig = rig + 11 > maxCols ? maxCols - 11 : rig;
  const int maxLn(maxRows - 5);
  vvBook pages;
  paginate(books, pages, maxLn);
  int cpn(0); // current page number - 1 (pages index)
  displayCurrentPage(oWin, mWin, pages[cpn], lef, cen, rig, cpn);
  displayPaginationMessage(mWin, cpn, pages.size(), books.size());
  if (pages.size() > 1) {
    char ch('j');
    while (true) {
      ch = getch();
      if (ch == 'x') {
        break;
      } else if (ch == 'h') { // first page
        cpn = 0;
        displayCurrentPage(oWin, mWin, pages[cpn], lef, cen, rig, cpn);
      } else if (ch == 'l') { // last page
        cpn = pages.size() - 1;
        displayCurrentPage(oWin, mWin, pages[cpn], lef, cen, rig, cpn);
      } else if (ch == 'k' && cpn > 0) { // prev page
        cpn--;
        displayCurrentPage(oWin, mWin, pages[cpn], lef, cen, rig, cpn);
      } else if (ch == 'j' && cpn < pages.size() - 1) { // next page
        cpn++;
        displayCurrentPage(oWin, mWin, pages[cpn], lef, cen, rig, cpn);
      }
      displayPaginationMessage(mWin, cpn, pages.size(), books.size());
    }
  }
  werase(oWin);
  werase(mWin);
  displayMenu(oWin);
}

void displayPaginationMessage(WINDOW *mWin, int cpn, int pc, int bc) {
  char tmp[255];
  snprintf(tmp, 200, "Displaying page %d of %d (%d books).", cpn + 1, pc, bc);
  std::string h0(tmp);
  std::string h1("Press 'j' for next page, 'k' for previous, 'h' for first, "
                 "'l' for last.");
  displayTextAtCenter(mWin, h0, 1);
  displayTextAtCenter(mWin, h1, 3);
  resetMessageArea(mWin);
}

void displayHeader(WINDOW *oWin, WINDOW *mWin, int lef, int cen, int rig,
                   int cpn) {
  Book aBook;
  aBook.clear();
  aBook.setAuthor("Author");
  aBook.setTitle("Title");
  aBook.setISBN("ISBN");
  displayBook(oWin, aBook, 3, lef, cen, rig);
  mvwprintw(oWin, 3, rig + 6, "Page %d", cpn + 1);
}

void paginate(const vBook &books, vvBook &pages, int maxLn) {
  pages.clear();
  vBook t0;
  pages.emplace_back(t0);
  for (auto b : books) {
    if (pages.back().size() > maxLn) {
      vBook tn;
      pages.emplace_back(tn);
    }
    pages.back().emplace_back(b);
  }
}

void displayCurrentPage(WINDOW *oWin, WINDOW *mWin, vBook books, int lef,
                        int cen, int rig, int cpn) {
  werase(oWin);
  displayHeader(oWin, mWin, lef, cen, rig, cpn);
  int r(5);
  for (auto aBook : books) {
    displayBook(oWin, aBook, r++, lef, cen, rig);
  }
  wrefresh(oWin);
}

void resetWorkArea(WINDOW *aWin) {
  box(aWin, 0, 0);
  mvwprintw(aWin, 0, midColInWin(aWin) - 4, "Work Area");
  wrefresh(aWin);
}

void resetMessageArea(WINDOW *aWin) {
  box(aWin, 0, 0);
  mvwprintw(aWin, 0, midColInWin(aWin) - 6, "Message Area");
  wrefresh(aWin);
}

int midRowInWin(WINDOW *aWin) { return getmaxy(aWin) >> 1; }

int midColInWin(WINDOW *aWin) { return getmaxx(aWin) >> 1; }
