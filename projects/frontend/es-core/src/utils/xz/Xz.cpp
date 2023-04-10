//
// Xz.cpp based on 02_decompress.c from Lasse Collin
//        modified by davidb2111 on 14/04/2023
//        for Recalbox 
//

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <lzma.h>
#include <string.h>
#include <cstring>
#include "Xz.h"

Xz::Xz() :
  m_strm LZMA_STREAM_INIT
{
}

Xz::~Xz() {
  // Free the memory allocated for the decoder.
  lzma_end(&m_strm);
}

lzma_ret Xz::InitDecoder() {
  lzma_ret ret = lzma_stream_decoder(&m_strm, UINT64_MAX, LZMA_CONCATENATED);
  return ret;
}

bool Xz::IsStillDecompressing() {
  return m_strm.avail_in ? true : false;
}

void Xz::InjectBuffer(uint8_t *inbuf, size_t inbuf_size, lzma_action action) {
  m_strm.next_in = inbuf;
  m_strm.avail_in = inbuf_size;
  m_action = action;
}

size_t Xz::Decompress(uint8_t *outbuf, size_t outbuf_size)
{
  // When LZMA_CONCATENATED flag was used when initializing the decoder,
  // we need to tell lzma_code() when there will be no more input.
  // This is done by setting action to LZMA_FINISH instead of LZMA_RUN
  // in the same way as it is done when encoding.
  //
  // When LZMA_CONCATENATED isn't used, there is no need to use
  // LZMA_FINISH to tell when all the input has been read, but it
  // is still OK to use it if you want. When LZMA_CONCATENATED isn't
  // used, the decoder will stop after the first .xz stream. In that
  // case some unused data may be left in strm->next_in.

  m_strm.next_out = outbuf;
  m_strm.avail_out = outbuf_size;
  m_status = lzma_code(&m_strm, m_action);

  size_t decompressed = outbuf_size - m_strm.avail_out;


  if (m_status != LZMA_OK) {
    // Once everything has been decoded successfully, the
    // return value of lzma_code() will be LZMA_STREAM_END.
    //
    // It is important to check for LZMA_STREAM_END. Do not
    // assume that getting ret != LZMA_OK would mean that
    // everything has gone well or that when you aren't
    // getting more output it must have successfully
    // decoded everything.
    if (m_status == LZMA_STREAM_END)
      return decompressed;

    // It's not LZMA_OK nor LZMA_STREAM_END,
    // so it must be an error code. See lzma/base.h
    // (src/liblzma/api/lzma/base.h in the source package
    // or e.g. /usr/include/lzma/base.h depending on the
    // install prefix) for the list and documentation of
    // possible values. Many values listen in lzma_ret
    // enumeration aren't possible in this example, but
    // can be made possible by enabling memory usage limit
    // or adding flags to the decoder initialization.
    switch (m_status) {
      case LZMA_MEM_ERROR:
        m_message = std::string("Memory allocation failed");
        break;

      case LZMA_FORMAT_ERROR:
        // .xz magic bytes weren't found.
        m_message = std::string("The input is not in the .xz format");
        break;

      case LZMA_OPTIONS_ERROR:
        // For example, the headers specify a filter
        // that isn't supported by this liblzma
        // version (or it hasn't been enabled when
        // building liblzma, but no-one sane does
        // that unless building liblzma for an
        // embedded system). Upgrading to a newer
        // liblzma might help.
        //
        // Note that it is unlikely that the file has
        // accidentally became corrupt if you get this
        // error. The integrity of the .xz headers is
        // always verified with a CRC32, so
        // unintentionally corrupt files can be
        // distinguished from unsupported files.
        m_message = std::string("Unsupported compression options");
        break;

      case LZMA_DATA_ERROR:
        m_message = std::string("Compressed file is corrupt");
        break;

      case LZMA_BUF_ERROR:
        // Typically this error means that a valid
        // file has got truncated, but it might also
        // be a damaged part in the file that makes
        // the decoder think the file is truncated.
        // If you prefer, you can use the same error
        // message for this as for LZMA_DATA_ERROR.
        m_message = std::string("Compressed file is truncated or "
          "otherwise corrupt");
        break;

      default:
        // This is most likely LZMA_PROG_ERROR.
        m_message = std::string("Unknown error, possibly a bug");
        break;
    }

    return 0;
  }
  return decompressed;
}

lzma_ret Xz::Error() {
  return m_status;
}
std::string Xz::ErrorMessage() {
  return m_message;
}
