#include "ZLibUtils/Compress.hh"
#include "Core/Types.hh"
#include <cassert>
#include <cstdio>
#include "MiniZLib/zlib.h"

void ZLibUtils::compressToBuffer(const char* indata, unsigned indataLength, std::vector<char>& output,unsigned& outdataLength)
{
  outdataLength = 0;
  output.clear();
  assert(indataLength<UINT32_MAX);
  output.reserve(indataLength==0?sizeof(uint32_t):compressBound(indataLength) + 128 + sizeof(uint32_t));
  *(reinterpret_cast<uint32_t*>(&output[0])) = uint32_t(indataLength);//embed old original size at first 4 bytes
  if (indataLength==0) {
    //empty buffer, nothing to compress
    outdataLength = sizeof(uint32_t);
    return;
  }
  unsigned long outlength = output.capacity()-1 - sizeof(uint32_t);
  int res = compress(reinterpret_cast<unsigned char*>(&(output[sizeof(uint32_t)])),&outlength,
                     reinterpret_cast<const unsigned char*>(indata),indataLength);
  if (res==Z_OK) {
    assert(outlength<UINT_MAX-sizeof(uint32_t));
    outdataLength = static_cast<unsigned>(outlength) + sizeof(uint32_t);
    return;
  }
  //something went wrong:
  if (res==Z_MEM_ERROR) {
    printf("ZLibUtils::compressToBuffer ERROR: Z_MEM_ERROR during compression. Exiting.\n");
  } else if (res==Z_BUF_ERROR) {
    printf("ZLibUtils::compressToBuffer ERROR: Z_BUF_ERROR during compression. Exiting.\n");
  } else {
    printf("ZLibUtils::compressToBuffer ERROR: Unknown problem during compression. Exiting.\n");
  }
  exit(1);
}

void ZLibUtils::decompressToBuffer(const char* indata, unsigned indataLength, std::vector<char>& output,unsigned& outdataLength)
{
  output.clear();
  assert(indataLength>=sizeof(uint32_t));
  uint32_t outdataLength_orig = *(reinterpret_cast<const uint32_t*>(indata));
  outdataLength = outdataLength_orig;
  if (outdataLength_orig==0&&indataLength==sizeof(uint32_t))
    {
      return;//original buffer was empty
    }
  output.clear();
  output.reserve(outdataLength);
  unsigned long outlength = outdataLength;
  int res = uncompress(reinterpret_cast<unsigned char*>(&(output[0])),&outlength,
                       reinterpret_cast<const unsigned char*>(indata)+sizeof(uint32_t),indataLength);
  if (res==Z_OK) {
    assert(outlength<UINT_MAX-sizeof(uint32_t));
    outdataLength = static_cast<unsigned>(outlength);
    if (outdataLength!=outdataLength_orig) {
      printf("ZLibUtils::decompressToBuffer ERROR: Weird error during decompression. Exiting.\n");
      exit(1);
    }
    return;
  }
  //something went wrong:
  if (res==Z_MEM_ERROR) {
    printf("ZLibUtils::decompressToBuffer ERROR: Z_MEM_ERROR during decompression. Exiting.\n");
  } else if (res==Z_BUF_ERROR) {
    printf("ZLibUtils::decompressToBuffer ERROR: Z_BUF_ERROR during decompression. Exiting.\n");
  } else if (res==Z_BUF_ERROR) {
    printf("ZLibUtils::decompressToBuffer ERROR: Z_DATA_ERROR during decompression. Data might be incomplete or corrupted. Exiting.\n");
  } else {
    printf("ZLibUtils::decompressToBuffer ERROR: Unknown problem during decompression. Exiting.\n");
  }
  exit(1);
}

//ZEXTERN int ZEXPORT compress OF((Bytef *dest,   uLongf *destLen, const Bytef *source, uLong sourceLen));
/*
         Compresses the source buffer into the destination buffer.  sourceLen is
     the byte length of the source buffer.  Upon entry, destLen is the total size
     of the destination buffer, which must be at least the value returned by
     compressBound(sourceLen).  Upon exit, destLen is the actual size of the
     compressed buffer.

         compress returns Z_OK if success, Z_MEM_ERROR if there was not
     enough memory, Z_BUF_ERROR if there was not enough room in the output
     buffer.
*/

//ZEXTERN int ZEXPORT uncompress OF((Bytef *dest,   uLongf *destLen, const Bytef *source, uLong sourceLen));
/*
         Decompresses the source buffer into the destination buffer.  sourceLen is
     the byte length of the source buffer.  Upon entry, destLen is the total size
     of the destination buffer, which must be large enough to hold the entire
     uncompressed data.  (The size of the uncompressed data must have been saved
     previously by the compressor and transmitted to the decompressor by some
     mechanism outside the scope of this compression library.) Upon exit, destLen
     is the actual size of the uncompressed buffer.

         uncompress returns Z_OK if success, Z_MEM_ERROR if there was not
     enough memory, Z_BUF_ERROR if there was not enough room in the output
     buffer, or Z_DATA_ERROR if the input data was corrupted or incomplete.  In
     the case where there is not enough room, uncompress() will fill the output
     buffer with the uncompressed data up to that point.
*/
