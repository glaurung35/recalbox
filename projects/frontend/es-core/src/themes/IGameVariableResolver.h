//
// Created by bkg2k on 16/01/24.
//
#pragma once

#include <utils/String.h>

class IGameVariableResolver
{
  public:
    //! Default destructor
    virtual ~IGameVariableResolver() = default;

    /*!
     * @brief Resolve variables in the given string
     * @param string String in which to resolve variables
     */
    virtual void ResolveVariableIn(String& string) = 0;
