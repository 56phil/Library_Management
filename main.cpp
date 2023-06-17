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
class Book;    // forward declaration
class Library; // forward declaration

#include "lms_project.hpp"

// prototypes
int lastColForThis(WINDOW *, std::string);
int lastRowInWin(WINDOW *);
int maxSizeInVector(vString);
int midColInWin(WINDOW *);
int midRowInWin(WINDOW *);
void addBookToLibrary(Library &);
void buildBook(Book &);
void displayBook(WINDOW *, Book &, int r = 5, int t = 1, int a = 23,
                 int i = 45);
void displayCatalog(Library &);
void displayCurrentPage(vBook, int, int, int, int, int = 3);
void displayHeader(WINDOW *, int = 3, int = 1, int = 23, int = 45, int = -1);
void displayHelp();
void displayMenu();
void displayPaginationMessage(int, int, int);
void displayTextAtCenter(WINDOW *, std::string, int);
void displyBookVector(vBook &);
void getAuthor(Book &, char buff[512]);
void getISBN(Book &, char buff[512]);
void getMinColSizes(vBook &, int &, int &, int &);
void getTitle(Book &, char buff[512]);
void handleResize(int signal);
void paginate(const vBook &, vvBook &, int);
void removeBookFromLibrary(Library &);
void resetMWin();
void resetOWin();
void resetScreen();
void resetIWin();
void searchUsingAuthor(Library &);
void searchUsingISBN(Library &);
void searchUsingTitle(Library &);
void sortCatalogAuthor(vBook &books, vvBook &pages, int &cpn, int maxLn);
void sortCatalogISBN(vBook &books, vvBook &pages, int &cpn, int maxLn);
void sortCatalogTitle(vBook &books, vvBook &pages, int &cpn, int maxLn);
void tag(WINDOW *, std::string, int r = 1);
void tuiLoop(Library &);

// global variables
WINDOW *oWin;
WINDOW *mWin;
WINDOW *iWin;

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

  resizeterm(screenHeight, screenWidth);

  int iShare(screenHeight >> 3);
  iShare = iShare < 5 ? 5 : iShare | 1;
  int oShare(screenHeight - iShare - iShare);

  oWin = newwin(oShare, screenWidth, 0, 0);
  mWin = newwin(iShare, screenWidth, oShare, 0);
  iWin = newwin(iShare, screenWidth, oShare + iShare, 0);

  if (has_colors()) {
    // Enable color functionality
    start_color();

    // Define color pairs
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);

    wbkgd(iWin, COLOR_PAIR(1));
    wbkgd(oWin, COLOR_PAIR(1));
    wbkgd(mWin, COLOR_PAIR(3));

    // Apply color pair to text
    // attron(COLOR_PAIR(1));
    // printw("Hello, ncurses with color!");
    // attroff(COLOR_PAIR(1));

    resetOWin();
    resetMWin();
    resetIWin();
    refresh();
  }

  Library dLibrary;
  dLibrary.deserialize();

  tuiLoop(dLibrary);

  dLibrary.serialize();

  endwin();

  std::system("clear");
  return 0;
}

void handleResize(int signal) {
  if (signal == KEY_RESIZE) {

    int screenWidth, screenHeight;
    getmaxyx(stdscr, screenHeight, screenWidth);

    resizeterm(screenHeight, screenWidth);

    int iShare(screenHeight >> 3);
    iShare =
        iShare < 5 ? 5 : iShare | 1; // 5 or more lines, must be an odd number

    int oShare(screenHeight - iShare - iShare);
    if (oShare <= iShare) {
      displayTextAtCenter(oWin, "HELP!", midRowInWin(oWin));
      refresh();
      int ch = getch();
    } else {
      wresize(oWin, oShare, screenWidth);
      wresize(mWin, iShare, screenWidth);
      wresize(iWin, iShare, screenWidth);

      mvwin(oWin, 0, 0);
      mvwin(mWin, oShare, 0);
      mvwin(iWin, oShare + iShare, 0);

      wclear(iWin);
      wclear(mWin);
      wclear(oWin);

      resetIWin();
      resetMWin();
      resetOWin();

      refresh();
    }
  }
}

/* tuiLoop

 */

void tuiLoop(Library &dLibrary) {
  int ch(' ');
  displayMenu();
  do {
    resetScreen();
    curs_set(0);
    move(0, 0);
    ch = getch();
    wbkgd(mWin, COLOR_PAIR(3));
    werase(mWin);
    werase(iWin);
    if (ch == ERR) {
    } else if (ch == KEY_RESIZE) {
      handleResize(ch);
    } else if (ch == 'x') {
      break;
    } else if (ch == 'c') {
      werase(oWin);
      displayCatalog(dLibrary);
    } else if (ch == 'a') {
      addBookToLibrary(dLibrary);
    } else if (ch == 't') {
      searchUsingTitle(dLibrary);
    } else if (ch == 's') {
      searchUsingAuthor(dLibrary);
    } else if (ch == 'r') {
      removeBookFromLibrary(dLibrary);
    } else if (ch == 'h') {
      displayHelp();
    } else if (ch == 'i') {
      searchUsingISBN(dLibrary);
    } else {
      displayHelp();
    }
  } while (true);
}

void resetScreen() {
  wbkgd(iWin, COLOR_PAIR(1));
  wbkgd(mWin, COLOR_PAIR(3));
  wbkgd(oWin, COLOR_PAIR(1));
  resetIWin();
  resetMWin();
  resetOWin();
}

void addBookToLibrary(Library &aLibrary) {
  werase(mWin);
  resetMWin();
  Book aBook;
  buildBook(aBook);
  vBook results;
  std::string tmp(aBook.getISBN());
  aLibrary.searchByISBN(tmp, results);
  char buff[128];
  if (results.empty()) {
    aLibrary.addBook(aBook);
    werase(iWin);
    displayHeader(mWin, 1);
    displayBook(mWin, aBook, midRowInWin(mWin));
    snprintf(buff, 127, "Is now part of you collection of %lu books.",
             aLibrary.size());
    tag(mWin, buff, midRowInWin(mWin));
    werase(iWin);
    resetIWin();
  } else {
    snprintf(buff, 127,
             "A book with that ISBN already is in your collection. The old "
             "copy of %s will be replaced.",
             aBook.getISBN().c_str());
    displayTextAtCenter(mWin, buff, midRowInWin(mWin));
  }
  resetMWin();
  displayMenu();
}

void removeBookFromLibrary(Library &aLibrary) {
  Book aBook;
  char buff[512];
  displayHeader(iWin, 1);
  getISBN(aBook, buff);
  werase(oWin);
  werase(iWin);
  std::string tmp = aBook.getISBN();
  vBook results;
  aLibrary.searchByISBN(tmp, results);
  if (results.empty()) {
    wbkgd(mWin, COLOR_PAIR(2));
    mvwprintw(mWin, midRowInWin(mWin), 2,
              "Book with ISBN of %s is not in catalog.",
              aBook.getISBN().c_str());
  } else if (aLibrary.removeBook(aBook)) {
    displayHeader(mWin, 1);
    displayBook(mWin, results.front(), midRowInWin(mWin));
    char buff[128];
    snprintf(buff, 127, "The collection is down to %lu books.",
             aLibrary.size());
    tag(mWin, buff, midRowInWin(mWin));
  }
  wrefresh(mWin);
  displayMenu();
}

void searchUsingISBN(Library &aLibrary) {
  Book aBook;
  char buff[512];
  getISBN(aBook, buff);
  std::string tmp = aBook.getISBN();
  werase(oWin);
  werase(iWin);
  displayHeader(iWin);
  resetScreen();
  vBook results;
  aLibrary.searchByISBN(tmp, results);
  if (results.empty()) {
    displayBook(mWin, aBook, lastRowInWin(mWin));
    mvwprintw(mWin, midRowInWin(mWin), 2,
              "Book with ISBN of %s is not in catalog.",
              aBook.getISBN().c_str());
    displayMenu();
  } else {
    displyBookVector(results);
  }
}

void searchUsingTitle(Library &aLibrary) {
  Book aBook;
  char buff[512];
  getTitle(aBook, buff);
  std::string tmp = aBook.getTitle();
  werase(oWin);
  werase(iWin);
  displayHeader(iWin);
  resetScreen();
  vBook results;
  aLibrary.searchByTitle(tmp, results);
  if (results.empty()) {
    mvwprintw(mWin, midRowInWin(mWin), 2,
              "Book with title of %s is not in catalog.",
              aBook.getTitle().c_str());
  } else {
    displyBookVector(results);
  }
}

void searchUsingAuthor(Library &aLibrary) {
  char buff[512];
  Book aBook;
  getAuthor(aBook, buff);
  werase(iWin);
  werase(mWin);
  displayHeader(iWin);
  resetScreen();
  std::string tmp(aBook.getAuthor());
  vBook results;
  aLibrary.searchByAuthor(tmp, results);
  if (results.empty()) {
    mvwprintw(mWin, midRowInWin(mWin), 5, "Nothing written by %s was found.",
              aBook.getAuthor().c_str());
    displayMenu();
  } else {
    std::string s0("Books by " + aBook.getAuthor() + ":");
    displayTextAtCenter(oWin, s0, 1);
    std::sort(results.begin(), results.end(),
              [](const Book &book1, const Book &book2) {
                return book1.getTitle() < book2.getTitle();
              });
    displyBookVector(results);
  }
}

void getAuthor(Book &aBook, char buff[512]) {
  resetIWin();
  echo();
  curs_set(1);
  wmove(iWin, midRowInWin(iWin), 23);
  wgetstr(iWin, buff);
  noecho();
  curs_set(0);
  aBook.setAuthor(buff);
  displayBook(iWin, aBook, midRowInWin(iWin));
  resetIWin();
}

void getISBN(Book &aBook, char buff[512]) {
  resetIWin();
  echo();
  curs_set(1);
  wmove(iWin, midRowInWin(iWin), 45);
  wgetstr(iWin, buff);
  noecho();
  curs_set(0);
  aBook.setISBN(buff);
  displayBook(iWin, aBook, midRowInWin(iWin));
  resetIWin();
}

void getTitle(Book &aBook, char buff[512]) {
  resetIWin();
  echo();
  curs_set(1);
  wmove(iWin, midRowInWin(iWin), 1);
  wgetstr(iWin, buff);
  noecho();
  curs_set(0);
  aBook.setTitle(buff);
  displayBook(iWin, aBook, midRowInWin(iWin));
  resetIWin();
}

void buildBook(Book &aBook) {
  char buff[512];
  wclear(iWin);
  resetIWin();
  getTitle(aBook, buff);
  getAuthor(aBook, buff);
  getISBN(aBook, buff);
}

void displayHelp() {
  std::string h0("Help Screen");
  std::string h1("Choose one of the availible options from the above menu. ");
  std::string h2("e.g. 'c' will display the catalog, 'a' to add a book, etc.");
  std::string h3("To quit the program, press the 'x' key.");
  werase(oWin);
  werase(mWin);
  displayMenu();
  displayTextAtCenter(mWin, h0, 1);
  displayTextAtCenter(mWin, h1, 2);
  displayTextAtCenter(mWin, h2, 3);
  displayTextAtCenter(mWin, h3, 4);
}

/*
gets: string, int
returns: nonthing
objective: put a string at the center on the current line
method: calculate & print number of spaces needed for padding.
print the string. When the string is too large, the string is spit
into two strings of roughly equal size.
*/
void displayTextAtCenter(WINDOW *aWin, std::string str, int onRow) {

  int maxRows, maxCols;
  getmaxyx(aWin, maxRows, maxCols);
  onRow = onRow > maxRows ? maxRows : onRow;

  if (str.size() > maxCols) {
    vString lines;
    lines.clear();
    int sPos(str.substr(0, str.size() / 2).find_last_of(' '));
    if (sPos == std::string::npos) {
      sPos = str.size() >> 1;
    }
    std::string tmpStr(str.substr(0, sPos));
    displayTextAtCenter(aWin, tmpStr, onRow);
    str = str.substr(sPos + 1, onRow);
    displayTextAtCenter(aWin, tmpStr, ++onRow);
  } else {
    int diff(maxCols - str.size());
    int xPos(diff >> 1);
    if (diff & 1) {
      xPos++;
    }
    mvwprintw(aWin, onRow, xPos, str.c_str());
  }
  wrefresh(aWin);
}

void displayBook(WINDOW *aWin, Book &b, int r, int tPos, int aPos, int iPos) {
  mvwprintw(aWin, r, tPos, b.getTitle().c_str());
  mvwprintw(aWin, r, aPos, b.getAuthor().c_str());
  mvwprintw(aWin, r, iPos, b.getISBN().c_str());
}

void displayCatalog(Library &aLibrary) {
  int r(1);
  wrefresh(oWin);
  std::string t0("Your Catalog:");
  displayTextAtCenter(oWin, t0, r);
  r += 2;
  auto dcatalog(aLibrary.getAllBooks());
  vBook results;
  for (auto aPair : dcatalog) {
    results.emplace_back(aPair.second);
  }
  displyBookVector(results);
}

/* displayMenu
   gets: WINDOW pointer
   returns: nothing
   objective: display options - used by displayHelp & displayMenu
 */
void displayMenu() {
  std::string l1("Welcome to Your Library");
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
  getmaxyx(oWin, maxRows, maxCols);

  displayTextAtCenter(oWin, l1, 1);
  displayTextAtCenter(oWin, m0, 3);
  unsigned r(5), c((maxCols - maxSizeInVector(menuDetail)) >> 1);
  for (auto s : menuDetail) {
    mvwprintw(oWin, r++, c, s.c_str());
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
   objective: return the starting position of given string so that it is right
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

/*
 calculates good column positions for Title, Author, and ISBN
 Minimum column widths provided in l, c, r.
 */
void catalogSizing(int &l, int &c, int &r, int &lef, int &cen, int &rig) {
  int maxRows, maxCols;
  getmaxyx(oWin, maxRows, maxCols);
  int dt(c >> 1);
  cen = ((maxCols >> 1) - dt);
  lef = (cen - l - dt);
  rig = (cen + r + dt);
}

/* displyBookVector
   gets: WINDOW pointer
   vector of Book objects
   returns: nothing
   objective: put the contents of books on the screen
   method: locates the center, displays the header, traverses the vector
   displaying title, author, and ISBN using the displayBook function.
*/
void displyBookVector(vBook &books) {
  if (books.empty()) {
    return;
  }
  int l, c, r, lef, cen, rig;
  getMinColSizes(books, l, c, r);
  int maxRows, maxCols;
  getmaxyx(oWin, maxRows, maxCols);
  catalogSizing(l, c, r, lef, cen, rig);
  const int maxLn(maxRows - 5);
  vvBook pages;
  paginate(books, pages, maxLn);
  int cpn(0);  // current page number - 1 (pages index)
  int ch('h'); // page one, for starters
  while (true) {
    if (ch == 'x') {
      break;
    } else if (ch == KEY_RESIZE) {
      handleResize(ch);
      catalogSizing(l, c, r, lef, cen, rig);
      cpn = 0;
    } else if (ch == 'h') { // first page
      cpn = 0;
    } else if (ch == 'j' && cpn < pages.size() - 1) { // next page
      cpn++;
    } else if (ch == 'k' && cpn > 0) { // prev page
      cpn--;
    } else if (ch == 'l') { // last page
      cpn = pages.size() - 1;
    } else if (ch == 't') { // sort using title
      sortCatalogTitle(books, pages, cpn, maxLn);
    } else if (ch == 'a') { // sort using author
      sortCatalogAuthor(books, pages, cpn, maxLn);
    } else if (ch == 'i') { // sort by isbn
      sortCatalogISBN(books, pages, cpn, maxLn);
    } else {
      cpn = 0;
    }
    werase(oWin);
    displayCurrentPage(pages[cpn], lef, cen, rig, cpn);
    displayPaginationMessage(cpn, pages.size(), books.size());
    ch = getch();
  }
  werase(oWin);
  werase(mWin);
  displayMenu();
}

void getMinColSizes(vBook &books, int &l, int &c, int &r) {
  l = 5; // "Title".size();
  c = 6; // "Author".size();
  r = 4; // "ISBN".size();
  for (auto book : books) {
    l = book.getTitle().size() > l ? book.getTitle().size() : l;
    c = book.getAuthor().size() > c ? book.getAuthor().size() : c;
    r = book.getISBN().size() > r ? book.getISBN().size() : r;
  }
  l += 2;
  c += 2;
  r += 2;
}

void sortCatalogAuthor(vBook &books, vvBook &pages, int &cpn, int maxLn) {
  std::sort(books.begin(), books.end(),
            [](const Book &book1, const Book &book2) {
              return book1.getAuthor() < book2.getAuthor();
            });
  cpn = 0;
  paginate(books, pages, maxLn);
}

void sortCatalogTitle(vBook &books, vvBook &pages, int &cpn, int maxLn) {
  std::sort(books.begin(), books.end(),
            [](const Book &book1, const Book &book2) {
              return book1.getTitle() < book2.getTitle();
            });
  cpn = 0;
  paginate(books, pages, maxLn);
}

void sortCatalogISBN(vBook &books, vvBook &pages, int &cpn, int maxLn) {
  std::sort(books.begin(), books.end(),
            [](const Book &book1, const Book &book2) {
              return book1.getISBN() < book2.getISBN();
            });
  cpn = 0;
  paginate(books, pages, maxLn);
}

void displayPaginationMessage(int cpn, int pc, int bc) {
  std::string bp(bc == 1 ? "book" : "books");
  char tmp[255];
  snprintf(tmp, 200, "Displaying page %d of %d (%d %s).", cpn + 1, pc, bc,
           bp.c_str());
  std::string h0(tmp);
  std::string h1("Sort options: 't' Title, 'a' Author, and 'i' Isbn ");
  std::string h2("Press 'h' for first, 'j' for next page, 'k' for previous, "
                 "'l' for last, and 'x' to get back to the main screen.");
  displayTextAtCenter(mWin, h0, 1);
  if (bc > 1) {
    displayTextAtCenter(mWin, h1, 2);
  }
  displayTextAtCenter(mWin, h2, 3);
  resetMWin();
}

void displayHeader(WINDOW *aWin, int r, int lef, int cen, int rig, int cpn) {
  Book aBook;
  aBook.clear();
  aBook.setAuthor("Author");
  aBook.setTitle("Title");
  aBook.setISBN("ISBN");
  displayBook(aWin, aBook, r, lef, cen, rig);
  if (cpn >= 0) {
    mvwprintw(aWin, r, rig + 16, "Page %d", cpn + 1);
  }
}

/*
 segment a collection of books into a collection of pages. Each page is a
 collection of books of a number that won't overfill the window.
*/
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

void displayCurrentPage(vBook books, int lef, int cen, int rig, int cpn,
                        int flo) {
  displayHeader(oWin, flo, lef, cen, rig, cpn);
  int r(flo + 2); // leave room for top matter
  for (auto aBook : books) {
    displayBook(oWin, aBook, r++, lef, cen, rig);
  }
  resetOWin();
}

void resetOWin() {
  box(oWin, 0, 0);
  mvwprintw(oWin, 0, lastColForThis(oWin, "Output") - 2, "Output");
  wrefresh(oWin);
}

void resetMWin() {
  box(mWin, 0, 0);
  mvwprintw(mWin, 0, lastColForThis(mWin, "Information") - 2, "Information");
  wrefresh(mWin);
}

void resetIWin() {
  box(iWin, 0, 0);
  mvwprintw(iWin, 0, lastColForThis(iWin, "Input") - 2, "Input");
  wrefresh(iWin);
}

int midRowInWin(WINDOW *aWin) { return getmaxy(aWin) >> 1; }

int midColInWin(WINDOW *aWin) { return getmaxx(aWin) >> 1; }
