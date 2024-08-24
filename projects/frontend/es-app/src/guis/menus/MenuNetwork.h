//
// Created by bkg2k on 26/10/2020.
//
#pragma once

#include <guis/menus/base/Menu.h>
#include <guis/GuiWaitLongExecution.h>
#include <guis/IGuiArcadeVirtualKeyboardInterface.h>

enum class NetworkOperation
{
  StartWIFI,
  StopWIFI,
  NewConnection,
  ScanSSID,
  WPS,
};

class MenuNetwork : public Menu
                     , private ILongExecution<NetworkOperation, bool>
                     , private IGuiArcadeVirtualKeyboardInterface
                     , private ISingleSelectorChanged<String>
                     , private IEditableChanged
                     , private ISwitchChanged
                     , private IActionTriggered
{
  public:
    /*!
     * @brief Default constructor
     * @param window Global window
     */
    explicit MenuNetwork(WindowManager& window);

    //! Default dstructor
    ~MenuNetwork() override;

  private:
    enum class Components
    {
      WifiOnOff,
      WifiSSID,
      WifiKey,
      Hostname,
      WPS,
    };

    //! SSID list
    ItemSelector<String>* mSSIDList;
    //! Hostname
    ItemEditable* mHostname;
    //! WIFI On/Off
    ItemSwitch* mWifiOnOff;
    //! WIFI Key
    ItemEditable* mWifiKey;
    //! Status
    ItemText* mStatus;
    //! IP
    ItemText* mIP;

    //! Edited text Backup
    String mBackupedText;

    //! Captured WPS SSID
    String mWpsSSID;
    //! Captured WPS PSK
    String mWpsPSK;

    //! Last SSID Scan
    String::List mScannedSSIDList;

    //! Anti-renentry flag
    bool mFillingList;

    //! Need backup into /boot/recalbox-backup.conf
    bool mNeedBackup;

    /*!
     * @brief Try WPS connection
     */
    void TryWPS();

    /*!
     * @brief Connect using WPS
     * @param from Current Operation
     * @return True if the WPS connection is successful
     */
    bool ConnectWps(GuiWaitLongExecution<NetworkOperation, bool>& from);

    /*!
     * @brief Set WIFI ssid
     * @param ssid new wifi ssid
     * @param save True to save configuration immediately
     */
    static void SetWifiSSID(const String& ssid, bool save)
    {
      RecalboxConf::Instance().SetWifiSSID(ssid);
      if (save) RecalboxConf::Instance().Save();
    }

    /*
     * ILongExecution implementation
     */

    /*!
     * @brief Execture network operation
     * @param parameter Network operation required to execute
     * @return True if the operation was successful
     */
    bool Execute(GuiWaitLongExecution<NetworkOperation, bool>& from, const NetworkOperation& parameter) final;

    /*!
     * @brief Receive the status of network operations
     * @param parameter original input parameter
     * @param result Result state
     */
    void Completed(const NetworkOperation& parameter, const bool& result) final;

    /*
     * IOptionListComponent implementation
     */

    void MenuSingleChanged(int id, int index, const String& value) override;

    /*
     * IEditableComponent implementation
     */

    void MenuEditableChanged(int id, const String& text) override;

    /*
     * ISwitchComponent implementation
     */

    void MenuSwitchChanged(int id, bool& status) override;

    /*
     * IGuiArcadeVirtualKeyboardInterface implementation
     */

    void ArcadeVirtualKeyboardTextChange(GuiArcadeVirtualKeyboard&, const String&) override {}

    void ArcadeVirtualKeyboardValidated(GuiArcadeVirtualKeyboard& vk, const String& text) override;

    void ArcadeVirtualKeyboardCanceled(GuiArcadeVirtualKeyboard&) override {}

    /*
     * IActionTriggered implementation
     */

    void MenuActionTriggered(int id);
};

