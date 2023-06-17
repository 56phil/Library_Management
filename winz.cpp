#include "lms_project.hpp"
#include "winz.hpp"
 
Winz::Winz() {

}

void handleResize(int signal) {
  // Get the new terminal window dimensions
  int newRows, newCols;
  getmaxyx(stdscr, newRows, newCols);


  // Resize and adjust the application's windows
  // resizeAppWindows(&oWin, newRows / 10 * 8, newCols);

  // Redraw the screen
  refresh();
}

Winz::~Winz() {}
