#ifndef LMS_PROJECT_HPP
#define LMS_PROJECT_HPP

class Book; // foward reference

#include <_ctype.h>
#include <algorithm>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <cstdio>
#include <cstdlib>
#include <curses.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ncurses.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, Book> umB;
typedef std::vector<Book> vBook;
typedef std::vector<std::string> vString;
typedef std::vector<vBook> vvBook;
typedef unsigned long ul;

#endif // !LMS_PROJECT_HPP
