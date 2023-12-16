//
// Created by bkg2k on 16/12/23.
//
#pragma once

#include <utils/String.h>
#include <utils/storage/HashMap.h>
#include <utils/storage/Set.h>

class ThemeSupport
{
  public:
    //! Element's property type
    enum class ElementProperty
    {
      NormalizedPair,
      Path,
      String,
      Color,
      Float,
      Boolean
    };

    //! Authorized elements/sub-elements in the xml
    static HashMap<String, HashMap<String, ElementProperty>>& ElementMap();
    //! Supported features set
    static HashSet<String>& SupportedFeatures();
    //! Supported view set
    static HashSet<String>& SupportedViews();
};
