#ifndef Mesh_tempworkarounds_hh
#define Mesh_tempworkarounds_hh

//Temporary workaround for incomplete C++11 support. This header is
//destined for removal one day.

#include "Core/_tempdetectbadcxx11.hh"//sets NEED_CXX11_WORKAROUNDS if needed

#ifdef NEED_CXX11_WORKAROUNDS
#include "MiniZLib/zlib.h"
#include <stdexcept>
namespace Mesh {

  struct Mesh_FakeLambda {
    //workaround for incomplete c++11 support
    Mesh_FakeLambda(gzFile f) : file(f) {}
    ~Mesh_FakeLambda() {}
    gzFile file;
    unsigned doRead(unsigned char* buf, unsigned buflen) 
    {
      int nb = gzread(file, buf, buflen);
      if (nb<1||(unsigned)nb>buflen)
	throw std::runtime_error("Read error");
      return (unsigned)nb;
    }
    void doWrite(unsigned char* buf, unsigned buflen) 
    {
      while (buflen) {
        int nb = gzwrite(file,buf,buflen);
        if (nb<1||(unsigned)nb>buflen)
          throw std::runtime_error("Write error");
        buf += nb;
        buflen -= nb;
      }
    }
  };
}
#endif
#endif
