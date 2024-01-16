//
// Created by bkg2k on 16/01/24.
//
#pragma once

#include <utils/String.h>

class IGlobalVariableResolver
{
  public:
    //! Default destructor
    virtual ~IGlobalVariableResolver() = default;

    /*!
     * @brief Resolve variables in the given string
     * @param string String in which to resolve variables
     */
    virtual void ResolveVariableIn(String& string) = 0;

    //! Has a CRT adapter active?
    [[nodiscard]] virtual bool HasCrt() const = 0;

    //! Has an RGB Jamma adapter active?
    [[nodiscard]] virtual bool HasJamma() const = 0;

    //! Is in TATE mode?
    [[nodiscard]] virtual bool IsTate() const = 0;

    //! Is resolution QVGA or less?
    [[nodiscard]] virtual bool IsQVGA() const = 0;

    //! Is Resolution between QVGA (excluded) & VGA (included)?
    [[nodiscard]] virtual bool IsVGA() const = 0;

    //! Is Resolution between VGA (excluded) & HD (included)?
    [[nodiscard]] virtual bool IsHD() const = 0;

    //! Is Resolution higher than HD?
    [[nodiscard]] virtual bool IsFHD() const = 0;
};
