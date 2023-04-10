//
// Created by davidb2111 on 14/04/2023
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#include <utils/network/HttpUnxzUntar.h>
#include "utils/Log.h"

HttpUnxzUntar::HttpUnxzUntar(Path outputPath)
{
  mOutputPath = &outputPath;
  if (xz.InitDecoder() != LZMA_OK) {
    // Decoder initialization failed. There's no point
    // to retry it so we need to exit.
    { LOG(LogError) << "[HttpUnxzUntar] Error initializing XZ decoder.\n"; }
    return;
  }
  { LOG(LogDebug) << "[HttpUnxzUntar] Xz initialized"; }

  tar.Untar(mOutputPath->ToString());
  { LOG(LogDebug) << "[HttpUnxzUn] Tar initialized"; }

}

void HttpUnxzUntar::DataReceived(const char* data, int length)
{
  
  { LOG(LogDebug) << "[HttpUnxzUntar] Data received from http server size=" << length << " bytes"; }
  if (!ProcessBuffer(data, length))
    Cancel();
  return;

}

size_t HttpUnxzUntar::ProcessBuffer(const char* ptr, size_t available) {
  size_t unarchived = 0; 
  size_t availableout; 

  xz.InjectBuffer((uint8_t*)ptr, available, LZMA_RUN);
  { LOG(LogDebug) << "[HttpUnxzUntar] Buffer injected in xz decompressor"; }
 
  while (xz.IsStillDecompressing()) {
    { LOG(LogDebug) << "[HttpUnxzUntar] Xz reports some data to decompress"; }
    availableout = xz.Decompress(mOutbuf, BUFSIZ);
    if (!availableout) {
      return 0;
    }
    { LOG(LogDebug) << "[HttpUnxzUntar] uncompressed " << availableout << " bytes"; }
   //     std::cout << "Read compressed " << read << " bytes and decompressed " << available << " bytes - m_status: " << xz->m_status << "\n";
   //     std::cout << "  avail_in: " << xz->m_strm.avail_in << "   avail_out: " << xz->m_strm.avail_out << "\n";
   //     fwrite(outbuf, 1, available, fhout);
    tar.InjectBuffer(mOutbuf, availableout);
    unarchived+=availableout;
  
  }
  { LOG(LogDebug) << "[HttpUnxzUntar] Processing done for " << unarchived << " bytes"; }

  return unarchived;
}
void HttpUnxzUntar::DataStart()
{
}

void HttpUnxzUntar::DataEnd()
{
}

