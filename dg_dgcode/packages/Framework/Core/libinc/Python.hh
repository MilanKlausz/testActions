#ifndef Core_Python_hh
#define Core_Python_hh

//---------------------------------------------------------------------------

//This header file provides boost/python.hpp functionality in a py:: namespace
//along with a PYTHON_MODULE macro to be used in the pycpp_XXX compiled python
//modules of the packages. It includes the necessary hacks and workarounds to
//work on the various platforms (don't put any hacks/workarounds elsewhere!).

//---------------------------------------------------------------------------

//We need the same macros here as in Types.hh since boost/python.hpp will
//include the same files and thus spoil Types.hh:
#define __STDC_LIMIT_MACROS
#define __STDC_FORMAT_MACROS

#include <dgboost/python.hpp>
namespace py = dgboost::python;
namespace dgboost {
  namespace python {
    typedef return_value_policy<reference_existing_object> return_ptr;

    //little utility function for turning any stl collection into a pylist (NB: all members will be copied one by one)
    template <class T>
    py::list stdcol2pylist(const T&t) {
      py::list l;
      typename T::const_iterator it(t.begin()),itE(t.end());
      for (;it!=itE;++it)
        l.append(*it);
      return l;
    }

    //initialise Python interpreter, setting also sys.argv array:
    void pyInit(const char * argv0 = 0);//only provide sys.argv[0] (defaulting to "dummyargv0")
    void pyInit(int argc, char** argv);//Transfer C++ cmdline to sys.argv
    bool isPyInit();//whether or not initialisation has been done (same as Py_IsInitialized())
    void ensurePyInit();//call pyInit only if !isPyInit (will initialise with dummy sys.argv[0])

    //convenient print + exit on py exception (call within catch statement):
    inline void print_and_handle_exception() {
      PyErr_Print();
      handle_exception();
    }
  }
}



//------------------PYTHON_MODULE definition---------------------------------
// In boost 1.41 the BOOST_PYTHON_MODULE needs a workaround to swallow a
// definition like PYMODNAME, whereas in boost 1.50 it works. So we cut
// arbitrarily and say that boost <1.45 will deploy a workaround.
// A bit of info about the issue at:
// http://boost.2283326.n4.nabble.com/boost-python-macro-hell-td2580415.html
#if BOOST_VERSION / 100000 >= 1 && BOOST_VERSION / 100 % 1000 >= 45
#  define PYTHON_MODULE BOOST_PYTHON_MODULE( PYMODNAME )
#else
#  define dgpyINITMOD BOOST_JOIN(init_module_,PYMODNAME)
#  define dgpyINIT BOOST_JOIN(init,PYMODNAME)
#  define dgpyPYMODNAMEstr BOOST_STRINGIZE(PYMODNAME)
#  define dgpyBOOSTPYMODINIT void dgpyINIT() {py::detail::init_module(dgpyPYMODNAMEstr,&dgpyINITMOD);} void dgpyINITMOD()
#  if BOOST_PYTHON_USE_GCC_SYMBOL_VISIBILITY
#    define PYTHON_MODULE void dgpyINITMOD(); extern "C" __attribute__ ((visibility("default"))) dgpyBOOSTPYMODINIT
#  else
#    define PYTHON_MODULE void dgpyINITMOD(); extern "C" dgpyBOOSTPYMODINIT
#  endif
#endif

#define PYTHON_CREATE_PYOBJECT(T,t) py::object(py::detail::new_reference(typename py::reference_existing_object::apply<T *>::type()(t)));

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Mac osx fix from https://code.google.com/r/przemyslawlinkowski-lightpack/source/browse/Software/PythonQT/src/PythonQtPythonInclude.h?spec=svn46b369b868758cf450d94bf511a0026db6778572&r=46b369b868758cf450d94bf511a0026db6778572:

//
// The following undefs for C standard library macros prevent
// build errors of the following type on mac ox 10.7.4 and XCode 4.3.3
//
// /usr/include/c++/4.2.1/bits/localefwd.h:57:21: error: too many arguments provided to function-like macro invocation
//    isspace(_CharT, const locale&);
//                    ^
// /usr/include/c++/4.2.1/bits/localefwd.h:56:5: error: 'inline' can only appear on functions
//     inline bool
//     ^
// /usr/include/c++/4.2.1/bits/localefwd.h:57:5: error: variable 'isspace' declared as a template
//     isspace(_CharT, const locale&);
//     ^
//
#undef isspace
#undef isupper
#undef islower
#undef isalpha
#undef isalnum
#undef toupper
#undef tolower
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
