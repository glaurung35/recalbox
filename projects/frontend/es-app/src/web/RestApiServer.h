//
// Created by bkg2k on 13/07/23.
//
#pragma once

#include "utils/os/system/Thread.h"
#include "web/server/Server.h"
#include "web/server/handlers/RequestHandler.h"

class RestApiServer : public Thread
{
  public:
    // Constructor
    RestApiServer();

    //! Destructor
    ~RestApiServer() override { Stop(); }

  private:
    Parameters mParam;
    RequestHandler mRequestHandler;
    Server mServer;

    void Break() override;

    void Run() override;
};
