#include "Core/Python.hh"
#include "Utils/NumpyUtils.hh"

namespace UtilsTests {
  py::object gensquares() {
    double * buf;
    std::size_t n = 100000;
    py::object o = NumpyUtils::create_numpyarray(n,buf);
    for (std::size_t i = 0;i<n;++i)
      buf[i] = i*i;
    return o;
  }
}


//PYTHON_MODULE
BOOST_PYTHON_MODULE( usenp )
{
  py::def("gensquares",UtilsTests::gensquares);
}
