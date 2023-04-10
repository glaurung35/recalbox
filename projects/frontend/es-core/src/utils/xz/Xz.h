//
// Xz.cpp based on 02_decompress.c from Lasse Collin
//        modified by davidb2111 on 14/04/2023
//        for Recalbox 
//
#pragma once

#include <cstring>
#include <lzma.h>
#include <string>

class Xz
{
  public:
    Xz();
    ~Xz();
    lzma_ret InitDecoder();
    lzma_ret Error();
    std::string ErrorMessage();
    void InjectBuffer(uint8_t *inbuf, size_t inbuf_size, lzma_action action);
    size_t Decompress(uint8_t *outbuf, size_t outbuf_size);
    bool IsStillDecompressing();

  private:
    std::string m_message;
    lzma_action m_action;
    lzma_ret m_status;
    lzma_stream m_strm;
};
