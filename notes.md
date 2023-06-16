# LMS Project Notes
## Approach
## Challenges
1. CURSES. 
2. Time management. 
3. Debugging. 
## Instructions
- building from source:
-   Place source code in an empty directory.
-   To compile & link: 
```bash
g++ main.cpp book.cpp library.cpp -o lms -lboost_serialization -lncurses -std=c++20
```
-To run:
```bash
./lms
```
(If you want to resize the terminal, do so **before** executing this command!)
- Running lms
1. The display has three windows. The names are placed at the upper right corner of each window. They are named:
     1. Output 
     2. Message
     3. Input

When the program starts, the output window has a menu, the message and work area windows are empty.

