#pragma once

#include <map>
#include <vector>
#include <string>
#include <SDL.h>
#include <sstream>
#include "pugixml/pugixml.hpp"

#define DEVICE_KEYBOARD -1
#define MAX_PLAYERS 5

enum class InputType
{
  Axis,
  Button,
  Hat,
  Key,
  Count,
};

struct Input
{
public:
  int device;
  InputType type;
  int id;
  int value;
  int code;
  bool configured;

  Input()
    : device(DEVICE_KEYBOARD),
      type(InputType::Count),
      id(-1),
      value(-999),
      code(-1),
      configured(false)
  {
  }

  Input(int dev, InputType t, int i, int val, bool conf)
    : device(dev),
      type(t),
      id(i),
      value(val),
      code(-1),
      configured(conf)
  {
  }

  static const char* getHatDir(int val)
  {
    if ((val & SDL_HAT_UP) != 0)    return "up";
    if ((val & SDL_HAT_DOWN) != 0)  return "down";
    if ((val & SDL_HAT_LEFT) != 0)  return "left";
    if ((val & SDL_HAT_RIGHT) != 0) return "right";
    return "neutral?";
  }

  std::string string()
  {
    std::stringstream stream;
    switch (type)
    {
      case InputType::Button:  stream << "Button " << id; break;
      case InputType::Axis:    stream << "Axis " << id << (value > 0 ? "+" : "-"); break;
      case InputType::Hat:     stream << "Hat " << id << " " << getHatDir(value);  break;
      case InputType::Key:     stream << "Key " << SDL_GetKeyName((SDL_Keycode) id); break;
      case InputType::Count:
      default:                 stream << "Input to string error"; break;
    }
    return stream.str();
  }

  void computeCode()
  {
    if (device == DEVICE_KEYBOARD)
    {
      return;
    }
    switch (type)
    {
      case InputType::Axis:
        #ifdef SDL_JOYSTICK_IS_OVERRIDEN_BY_RECALBOX
        code = SDL_JoystickAxisEventCodeById(device, id);
        #endif
        break;
      case InputType::Button:
        #ifdef SDL_JOYSTICK_IS_OVERRIDEN_BY_RECALBOX
        code = SDL_JoystickButtonEventCodeById(device, id);
        #endif
        break;
      case InputType::Hat:
        #ifdef SDL_JOYSTICK_IS_OVERRIDEN_BY_RECALBOX
        code = SDL_JoystickHatEventCodeById(device, id);
        #endif
        break;
      default:
        break;
      case InputType::Key:
        break;
      case InputType::Count:
        break;
    }
  }
};

class InputConfig
{
  private:
    const int mDeviceId;
    const int mDeviceIndex;
    const std::string mDeviceName;
    const std::string mDeviceGUID;
    const int mDeviceNbAxes; // number of axes of the device
    const int mDeviceNbHats;
    const int mDeviceNbButtons;
    std::map<std::string, Input> mNameMap;

  public:
    explicit InputConfig(const InputConfig* source);

    InputConfig(int deviceId, int deviceIndex, const std::string& deviceName, const std::string& deviceGUID,
                int deviceNbAxes, int deviceNbHats, int deviceNbButtons);

    void clear();

    void loadFrom(const InputConfig* source);

    void mapInput(const std::string& name, Input input);

    void unmapInput(const std::string& name); // unmap all Inputs mapped to this name

    inline int getDeviceId() const { return mDeviceId; };

    inline int getDeviceIndex() const { return mDeviceIndex; };

    inline const std::string& getDeviceName() const { return mDeviceName; }

    inline const std::string& getDeviceGUIDString() const { return mDeviceGUID; }

    inline int getDeviceNbAxes() const { return mDeviceNbAxes; };

    inline int getDeviceNbHats() const { return mDeviceNbHats; };

    inline int getDeviceNbButtons() const { return mDeviceNbButtons; };

    inline const std::map<std::string, Input>& getNameMap() const { return mNameMap; };

    std::string getSDLPowerLevel();

    std::string getSysPowerLevel();

    //Returns true if Input is mapped to this name, false otherwise.
    bool isMappedTo(const std::string& name, Input input);

    bool isMapped(const std::string& name);

    //Returns a list of names this input is mapped to.
    std::vector<std::string> getMappedTo(Input input);

    void loadFromXML(pugi::xml_node root);

    void writeToXML(pugi::xml_node parent);

    bool isConfigured() { return !mNameMap.empty(); }

    // Returns true if there is an Input mapped to this name, false otherwise.
    // Writes Input mapped to this name to result if true.
    bool getInputByName(const std::string& name, Input* result);
};
