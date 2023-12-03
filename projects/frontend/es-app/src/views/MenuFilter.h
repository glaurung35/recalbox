//
// Created by digi on 10/13/23.
//

#pragma once

class MenuFilter
{
  public:
    enum Menu {
      Main,
      Exit,
      Search,
      GamelistOptions
    };
    static bool ShouldDisplayMenu(const enum Menu menu) { (void)menu; return RecalboxConf::Instance().GetMenuType() !=RecalboxConf::Menu::None; }
};
