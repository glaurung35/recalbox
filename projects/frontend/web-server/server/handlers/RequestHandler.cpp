//
// Created by bkg2k on 30/03/2020.
//

#include <utils/Files.h>
#include <utils/Log.h>
#include <utils/json/JSONBuilder.h>
#include "RequestHandler.h"
#include "Mime.h"
#include "RequestHandlerTools.h"
#include <utils/network/Url.h>

using namespace Pistache;

void RequestHandler::FileServer(const Rest::Request& request, Http::ResponseWriter response)
{
  RequestHandlerTools::LogRoute(request, "FileServer");

  Path path = mWWWRoot / (request.resource() != "/" ? request.resource() : mDefaultFile);
  std::string ext = path.Extension();

  bool knownMime = Mime::ExtToMIME.contains(ext);
  const Pistache::Http::Mime::MediaType& mimeType = knownMime ? Mime::ExtToMIME[ext] : Mime::BinaryFile;
  if (!knownMime)
    LOG(LogWarning) << "Unknown MIME Type for file extension: " << ext;
  RequestHandlerTools::SendResource(path, response, mimeType);
}

void RequestHandler::SystemReboot(const Rest::Request& request, Http::ResponseWriter response)
{
  RequestHandlerTools::LogRoute(request, "SystemReboot");

  auto res = RequestHandlerTools::Send(response, Http::Code::Ok);
  res.then([](ssize_t){ RequestHandlerTools::OutputOf("reboot"); },Async::NoExcept);
}

void RequestHandler::SystemShutdown(const Rest::Request& request, Http::ResponseWriter response)
{
  RequestHandlerTools::LogRoute(request, "SystemShutdown");

  auto res = RequestHandlerTools::Send(response, Http::Code::Ok);
  res.then([](ssize_t){ RequestHandlerTools::OutputOf("shutdown -h now"); }, Async::NoExcept);
}

void RequestHandler::SystemEsStart(const Rest::Request& request, Http::ResponseWriter response)
{
  RequestHandlerTools::LogRoute(request, "SystemEsStart");

  auto res = RequestHandlerTools::Send(response, Http::Code::Ok);
  res.then([](ssize_t){ RequestHandlerTools::OutputOf("/etc/init.d/S31emulationstation start"); },Async::NoExcept);
}

void RequestHandler::SystemEsStop(const Rest::Request& request, Http::ResponseWriter response)
{
  RequestHandlerTools::LogRoute(request, "SystemEsStop");

  auto res = RequestHandlerTools::Send(response, Http::Code::Ok);
  res.then([](ssize_t){ RequestHandlerTools::OutputOf("/etc/init.d/S31emulationstation stop"); }, Async::NoExcept);
}

void RequestHandler::SystemEsRestart(const Rest::Request& request, Http::ResponseWriter response)
{
  RequestHandlerTools::LogRoute(request, "SystemEsRestart");

  auto res = RequestHandlerTools::Send(response, Http::Code::Ok);
  res.then([](ssize_t){ RequestHandlerTools::OutputOf("/etc/init.d/S31emulationstation restart"); },Async::NoExcept);
}

void RequestHandler::SystemGenerateSupportArchive(const Rest::Request& request, Http::ResponseWriter response)
{
  RequestHandlerTools::LogRoute(request, "SystemGenerateSupportArchive");

  std::string archivePath = RequestHandlerTools::OutputOf("bash /recalbox/scripts/recalbox-support.sh");

  archivePath.erase(archivePath.size() - 1);

  const auto pos = archivePath.find_last_of('/');
  const std::string fileName = archivePath.substr(pos);

  JSONBuilder json;
  json.Open()
          .Field("supportArchive", "/system/supportarchive/download" + fileName)
          .Close();

  RequestHandlerTools::Send(response, Http::Code::Ok, json, Mime::Json);
}

void RequestHandler::SystemDownloadSupportArchive(const Rest::Request& request, Http::ResponseWriter response)
{
  RequestHandlerTools::LogRoute(request, "SystemDownloadSupportArchive");

  Path first;
  RequestHandlerTools::GetSystemResourcePath(first, Url::URLDecode(request.splatAt(0).name()));
  RequestHandlerTools::SendResource(first, response, Mime::TarGz);
}
