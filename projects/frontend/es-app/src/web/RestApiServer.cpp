//
// Created by bkg2k on 13/07/23.
//

#include "RestApiServer.h"
#include <csignal>
#include "Parameters.h"
#include "server/handlers/RequestHandler.h"
#include "server/Server.h"
#include <utils/Log.h>

RestApiServer::RestApiServer()
  : mRequestHandler(mParam.WWWRoot(), mParam.DefaultFile())
  , mServer(mParam, &mRequestHandler)
{
  Start("rest-api");
}

void RestApiServer::Run()
{
  LOG(LogInfo) << "Recalbox WebApi Server 1.0";

  // Run!
  mServer.Run();
}

void RestApiServer::Break()
{
  mServer.Cancel();
}
