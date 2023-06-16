# LMS Project Notes
## Approach
## Challenges
1. CURSES. 
2. Time management. 
3. Debugging. 
## Instructions
- building from source:
1. Place source code in an empty directory.
2. To compile & link: 
```bash
g++ main.cpp book.cpp library.cpp -o lms -lboost_serialization -lncurses -std=c++20
```
3. To run:
```bash
./lms
```
(If you want to resize the terminal, do so **before** executing this command!)
- Running lms
1. The display has three windows. They are named: 
  1. Output Area
  2. Message Area
  3. Work Area

When the program starts, the output window has a menu, the message and work area windows are empty.

