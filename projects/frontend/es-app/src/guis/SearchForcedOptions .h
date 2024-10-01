//
// Created by gugue_u
//


#pragma once

#include <string>

class SearchForcedOptions
{
  public:
    explicit SearchForcedOptions(std::string& searchText, FolderData::FastSearchContext context, bool fullMatch)
      : mSearchText(searchText),
        mContext(context),
        mFullMatch(fullMatch)
    {}

    std::string mSearchText;
    FolderData::FastSearchContext mContext;
    bool mFullMatch;
};
