//
// Created by bkg2k on 08/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//
#pragma once

#include <guis/menus/base/Menu.h>
#include <guis/menus/base/ISelectorChanged.h>
#include "hardware/devices/storage/StorageDevices.h"

// Forward declaration
class SystemManager;

class MenuSystem : public Menu
                    , private ISingleSelectorChanged<String>
                    , private ISingleSelectorChanged<StorageDevices::Device>
                    , private ISubMenuSelected
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     * @param systemManager System manager
     */
    explicit MenuSystem(WindowManager& window, SystemManager& systemManager);

    //! Destructeur
    ~MenuSystem() override;

  private:
    enum class Components
    {
      DiskUsage,
      Storage,
      Culture,
      Keyboard,
    };

    //! Kayboard name
    struct Keyboard
    {
      String Displayable; //!< Kayboard displayable name
      String Identifier;  //!< Keyboard identifier
    };
    //! Keyboard list
    typedef std::vector<Keyboard> KeyboardList;

    //! Culture structure
    struct Culture
    {
      String Displayable; //!< Language name
      String Identifier;  //!< Culture identifier
    };
    //! Culture list
    typedef std::vector<Culture> CultureList;

    //! System manager reference
    SystemManager& mSystemManager;

    //! Storage devices
    StorageDevices mStorageDevices;

    //! Available storages
    ItemSelector<StorageDevices::Device>* mStorages;
    //! Culture
    ItemSelector<String>* mCulture;
    //! Keyboard
    ItemSelector<String>* mKeyboard;

    //! Original storage
    String mOriginalStorage;
    //! Original culture
    String mOriginalCulture;
    //! Original keyboard
    String mOriginalKeyboard;

    //! Get Storage List
    SelectorEntry<StorageDevices::Device>::List GetStorageEntries(int& currentIndex);
    //! Get Culture List
    SelectorEntry<String>::List GetCultureEntries();
    //! Get Keyboard List
    SelectorEntry<String>::List GetKeyboardEntries();

    /*
     * IOptionListComponent<String> implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;

    /*
     * IOptionListComponent<StorageDevices::Device> implementation
     */

    void MenuSingleChanged(int id, int index, const StorageDevices::Device& value) override;

    /*
     * ISubMenuSelected implementation
     */

    void SubMenuSelected(int id) override;
};



