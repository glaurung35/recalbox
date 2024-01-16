//
// Created by bkg2k on 16/01/24.
//
#pragma once

#include <utils/String.h>

class IExternalVariableResolver
{
  public:
    //! Default destructor
    virtual ~IExternalVariableResolver() = default;

    /*!
     * @brief Resolve variables in the given string
     * @param string String in which to resolve variables
     */
    virtual void ResolveVariableIn(String& string) = 0;
};
