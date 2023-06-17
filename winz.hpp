#ifndef WINZ_HPP
#define WINZ_HPP
#include "lms_project.hpp"
class Winz {
public:
  Winz();
  Winz(Winz &&) = default;
  Winz(const Winz &) = default;
  Winz &operator=(Winz &&) = default;
  Winz &operator=(const Winz &) = default;
  ~Winz();

private:
  WINDOW *aWin;

};

#endif // !DEBUG

