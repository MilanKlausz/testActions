#include "Core/Python.hh"

int main(int,char**) {

  //Test that python can be initialised from compiled code.

  py::ensurePyInit();

  return 0;
}
