#include <RecalboxConf.h>
#include "guis/menus/MenuNetPlayEditPasswords.h"

MenuNetPlayEditPasswords::MenuNetPlayEditPasswords(WindowManager& window)
  : Menu(window, _("PREDEFINED PASSWORDS"))
{
  for(int i = 0; i < DefaultPasswords::sPasswordCount; i++)
  {
    String password = RecalboxConf::Instance().GetNetplayPasswords(i);
    if (password.empty()) password = DefaultPasswords::sDefaultPassword[i];
    AddEditable(_("PASSWORD #%i").Replace("%i", String(i)), password, i, this, false);
  }

  AddAction(_("VALIDATE"), "", 0, false, this);
}

void MenuNetPlayEditPasswords::MenuEditableChanged(int id, const String& newText)
{
  if (!newText.empty())
    RecalboxConf::Instance().SetNetplayPasswords(id, newText).Save();
}

