//
// Created by bkg2k on 15/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include <input/InputMapper.h>
#include <input/InputManager.h>

InputMapper::~InputMapper()
{
  SaveConfiguration();
}

void InputMapper::Build()
{
  LoadConfiguration();
  // Get connected pads
  mConnected = AvailablePads();
  // Copy to pad active list
  PadList activePads(mConnected);

  // Remove device already fetched from configuration
  // Also add non-connected pads to the phantom list
  for(Pad& device : mPads)
    if (device.IsValid())
    {
      bool found = false;
      for(int i = (int)activePads.size(); --i >= 0; )
        if (const Pad& connectedDevice = activePads[i]; connectedDevice.Same(device))
        {
          found = true;
          device.Copy(connectedDevice);
          activePads.erase(activePads.begin() + i);
          break;
        }
      // Device not found, move to unconnected list
      if (!found)
      {
        mUnconnected.push_back(device);
        { LOG(LogDebug) << "[PadMapping] Move to unconnected: " << device.AsString(); }
      }
    }

  // Add remaining pads in unused slots
  bool assignNew = false;
  for(const Pad& connectedDevice : activePads)
    for (Pad& device : mPads)
      if (!device.IsValid())
      {
        { LOG(LogDebug) << "[PadMapping] Add connected to the list: " << connectedDevice.AsString(); }
        device.Set(connectedDevice.mName, connectedDevice.mUUID, connectedDevice.mPath, connectedDevice.mIndex);
        assignNew = true;
        break;
      }

  // Assigned? If not, take the slot from the first non connected pad
  if (!activePads.empty() && !assignNew)
    for(const Pad& connectedDevice : activePads)
      for (Pad& device : mPads)
        if (!device.IsConnected())
        {
          { LOG(LogDebug) << "[PadMapping] Add connected to the list (took unconnected slot): " << connectedDevice.AsString(); }
          device.Set(connectedDevice.mName, connectedDevice.mUUID, connectedDevice.mPath, connectedDevice.mIndex);
          assignNew = true;
          break;
        }

  // Push active pads first
  { LOG(LogDebug) << "[PadMapping] Sort active first"; }
  SortActiveFirst(mPads);

  int index = 0;
  for(const Pad& pad : mPads)
  { LOG(LogDebug) << "[PadMapping] Pad @" << ++index << " = " << pad.AsString(); }

  // Save config
  if (assignNew)
    SaveConfiguration();
}

void InputMapper::LoadConfiguration()
{
  // Fill from configuration
  String uuid;
  String name;
  for(int i = Input::sMaxInputDevices; --i >= 0; )
    mPads[i].Reset();
  for(int i = Input::sMaxInputDevices; --i >= 0; )
    if (RecalboxConf::Instance().GetPad(i).Extract(':', uuid, name, true))
    {
      mPads[i].Set(name, uuid, Path::Empty, -1);
      { LOG(LogDebug) << "[PadMapping] Load pad @" << i << " = " << mPads[i].AsString(); }
    }
}

void InputMapper::SaveConfiguration()
{
  for(int i = Input::sMaxInputDevices; --i >= 0;)
    if (mPads[i].IsValid())
    {
      RecalboxConf::Instance().SetPad(i, String(mPads[i].mUUID).Append(':').Append(mPads[i].mName));
      { LOG(LogDebug) << "[PadMapping] Save pad @" << i << " = " << mPads[i].AsString(); }
    }
  RecalboxConf::Instance().Save();
}

InputMapper::PadList InputMapper::AvailablePads()
{
  PadList result;
  int count = InputManager::Instance().DeviceCount();
  for(int i = 0; i < count; ++i)
  {
    const InputDevice& device = InputManager::Instance().GetDeviceConfigurationFromIndex(i);
    if (device.ButtonCount() == 0) continue; // Not a true pad
    result.push_back(Pad(device.Name().ToTrim(), device.GUID(), device.UDevPath(), device.Index()));
    { LOG(LogDebug) << "[PadMapping] Available pad @" << i << " = " << result.back().AsString(); }
  }
  return result;
}

void InputMapper::SortActiveFirst(PadArray& padArray)
{
  // Seek highest non empty/phantom devices
  int higher = Input::sMaxInputDevices;
  while(--higher >= 0)
    if (padArray[higher].mIndex >= 0)
      break;

  // No devices or no empty slot?
  if ((unsigned int)higher >= Input::sMaxInputDevices) return;

  // Stack the active ones
  for(int i = 0; i < higher; ++i)
    if (padArray[i].mIndex < 0)
    {
      Pad tmp = padArray[i];
      for(int j = i; j < higher; ++j) padArray[j] = padArray[j+1];
      padArray[higher] = tmp;
      --higher;
      --i; // restart @ same position
    }
}

String InputMapper::GetDecoratedName(int index)
{
  int count = 0;
  const Pad& pad = mPads[index];
  if (pad.mIndex >= 0)
    for(const Pad& current : mPads)
      if (current.Same(pad))
        if (current.mIndex < pad.mIndex && current.mIndex >= 0)
          count++;

  String result(pad.mName);
  if (count > 0) result.Append(LEGACY_STRING(" #")).Append(count + 1);
  if (String batteryIcon = pad.LookupPowerLevel(); !batteryIcon.empty())
    result.Append(' ').Append(batteryIcon);
  result.Insert(0, pad.mIndex < 0 ? "\u26aa" : "\u26ab")
        .Append(pad.mIndex < 0 ? "\u26aa" : "\u26ab");
  return pad.IsConnected() ? result : String::Empty;
}

void InputMapper::Swap(int index1, int index2)
{
  // Clamp indexes
  index1 = Math::clampi(index1, 0, Input::sMaxInputDevices - 1);
  index2 = Math::clampi(index2, 0, Input::sMaxInputDevices - 1);

  // Really swap?
  if ((index1 == index2) || (index1 < 0) || (index2 < 0)) return;

  // Swap
  Pad tmp       = mPads[index1];
  mPads[index1] = mPads[index2];
  mPads[index2] = tmp;
}

void InputMapper::PadsAddedOrRemoved(bool removed)
{
  (void)removed;
  // Rebuild all when a pad has been added or removed
  Build();
}

int InputMapper::PadIndexFromDeviceIdentifier(SDL_JoystickID identifier)
{
  int index = InputManager::Instance().GetDeviceIndexFromId(identifier);
  if (index >= 0)
    for(int i = Input::sMaxInputDevices; --i >= 0; )
      if (const Pad& pad = mPads[i]; pad.IsConnected())
        if (pad.mIndex == index)
          return i;
  return -1;
}

int InputMapper::ConnectedPadCount() const
{
  int result = 0;
  for(int i = Input::sMaxInputDevices; --i >= 0; )
    if (const Pad& pad = mPads[i]; pad.IsConnected())
      result++;
  return result;
}

String InputMapper::Pad::LookupPowerLevel() const
{
  if (mIndex >= 0) return String((String::Unicode )InputManager::Instance().GetDeviceConfigurationFromIndex(mIndex).BatteryLevelIcon(), 1);
  return "";
}
