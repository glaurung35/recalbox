#include "InputConfig.h"
#include <string>
#include <SDL.h>
#include <iostream>
#include "Log.h"
#include "utils/StringUtil.h"

//some util functions
std::string inputTypeToString(InputType type)
{
  switch(type)
  {
  case InputType::Axis:		return "axis";
  case InputType::Button:	return "button";
  case InputType::Hat:		return "hat";
  case InputType::Key:		return "key";
  case InputType::Count:
  default:                return "error";
  }
}

InputType stringToInputType(const std::string& type)
{
  if (type == "axis")	  return InputType::Axis;
  if (type == "button")	return InputType::Button;
  if (type == "hat")    return InputType::Hat;
  if (type == "key")    return InputType::Key;
  return InputType::Count;
}

InputConfig::InputConfig(const InputConfig* source) :
  mDeviceId(source->getDeviceId()),
  mDeviceIndex(source->getDeviceIndex()),
  mDeviceName(source->getDeviceName()),
  mDeviceGUID(source->getDeviceGUIDString()),
  mDeviceNbAxes(source->getDeviceNbAxes()),
  mDeviceNbHats(source->getDeviceNbHats()),
  mDeviceNbButtons(source->getDeviceNbButtons())
{
  mNameMap = source->getNameMap();
}

InputConfig::InputConfig(int deviceId, int deviceIndex, const std::string& deviceName, const std::string& deviceGUID, int deviceNbAxes, int deviceNbHats, int deviceNbButtons)
  : mDeviceId(deviceId),
    mDeviceIndex(deviceIndex),
    mDeviceName(deviceName),
    mDeviceGUID(deviceGUID),
    mDeviceNbAxes(deviceNbAxes),
    mDeviceNbHats(deviceNbHats),
    mDeviceNbButtons(deviceNbButtons)
{
  (void)deviceName;
  (void)deviceGUID;
}

void InputConfig::clear()
{
  mNameMap.clear();
}

void InputConfig::loadFrom(const InputConfig* source) 
{
  mNameMap = source->getNameMap();
}

std::string InputConfig::getSDLPowerLevel()
{
  SDL_Joystick* joy;
  //joy = InputManager::getInstance()->getJoystickByJoystickID(getDeviceId());
  joy = SDL_JoystickOpen(getDeviceId());
  SDL_JoystickPowerLevel power = SDL_JoystickCurrentPowerLevel(joy);
  switch(power)
  {
    case SDL_JOYSTICK_POWER_EMPTY:   return "\uF1b6";
    case SDL_JOYSTICK_POWER_FULL:    return "\uF1b7";
    case SDL_JOYSTICK_POWER_LOW:     return "\uF1b1";
    case SDL_JOYSTICK_POWER_MAX:     return "\uF1ba";
    case SDL_JOYSTICK_POWER_MEDIUM:  return "\uF1b8";
    case SDL_JOYSTICK_POWER_UNKNOWN: return "\uF1b9";
    case SDL_JOYSTICK_POWER_WIRED:   return "\uF1b4";
    default: break;
  }

  return "";
}

std::string InputConfig::getSysPowerLevel()
{
  SDL_Joystick* joy;
  //joy = InputManager::getInstance()->getJoystickByJoystickID(getDeviceId());
  joy = SDL_JoystickOpen(getDeviceId());
  (void)joy; // TODO: Check usefulness
  return "\uF1be";
}

void InputConfig::mapInput(const std::string& name, Input input)
{
  mNameMap[StringUtil::toLower(name)] = input;
}

void InputConfig::unmapInput(const std::string& name)
{
  auto it = mNameMap.find(StringUtil::toLower(name));
  if(it != mNameMap.end())
    mNameMap.erase(it);
}

bool InputConfig::getInputByName(const std::string& name, Input* result)
{
  auto it = mNameMap.find(StringUtil::toLower(name));
  if(it != mNameMap.end())
  {
    *result = it->second;
    return true;
  }

  return false;
}

bool InputConfig::isMapped(const std::string& name)
{
  auto it = mNameMap.find(StringUtil::toLower(name));
  return it != mNameMap.end();
}

bool InputConfig::isMappedTo(const std::string& name, Input input)
{
  Input comp;
  if(!getInputByName(name, &comp))
    return false;

  if(comp.configured && comp.type == input.type && comp.id == input.id)
  {
    if(comp.type == InputType::Hat)
    {
      return (input.value == 0 || input.value & comp.value);
    }

    if(comp.type == InputType::Axis)
    {
      return input.value == 0 || comp.value == input.value;
    }else{
      return true;
    }
  }
  return false;
}

std::vector<std::string> InputConfig::getMappedTo(Input input)
{
  std::vector<std::string> maps;

  for (auto& iterator : mNameMap)
  {
    Input chk = iterator.second;

    if(!chk.configured)
      continue;

    if(chk.device == input.device && chk.type == input.type && chk.id == input.id)
    {
      if(chk.type == InputType::Hat)
      {
        if(input.value == 0 || input.value & chk.value)
        {
          maps.push_back(iterator.first);
        }
        continue;
      }

      if(input.type == InputType::Axis)
      {
        if(input.value == 0 || chk.value == input.value)
          maps.push_back(iterator.first);
      }else{
        maps.push_back(iterator.first);
      }
    }
  }

  return maps;
}

void InputConfig::loadFromXML(pugi::xml_node node)
{
  clear();

  for (pugi::xml_node input = node.child("input"); input; input = input.next_sibling("input"))
  {
    std::string name = input.attribute("name").as_string();
    std::string type = input.attribute("type").as_string();
    InputType typeEnum = stringToInputType(type);

    if(typeEnum == InputType::Count)
    {
      LOG(LogError) << "InputConfig load error - input of type \"" << type << "\" is invalid! Skipping input \"" << name << "\".\n";
      continue;
    }

    int id = input.attribute("id").as_int();
    int value = input.attribute("value").as_int();

    if(value == 0)
      LOG(LogWarning) << "WARNING: InputConfig value is 0 for " << type << " " << id << "!\n";

    mNameMap[StringUtil::toLower(name)] = Input(mDeviceId, typeEnum, id, value, true);
  }
}

void InputConfig::writeToXML(pugi::xml_node parent)
{
  pugi::xml_node cfg = parent.append_child("inputConfig");

  if(mDeviceId == DEVICE_KEYBOARD)
  {
    cfg.append_attribute("type") = "keyboard";
    cfg.append_attribute("deviceName") = "Keyboard";
  }else{
    cfg.append_attribute("type") = "joystick";
    cfg.append_attribute("deviceName") = mDeviceName.c_str();
  }

  cfg.append_attribute("deviceGUID") = mDeviceGUID.c_str();
    cfg.append_attribute("deviceNbAxes") = mDeviceNbAxes;
    cfg.append_attribute("deviceNbHats") = mDeviceNbHats;
    cfg.append_attribute("deviceNbButtons") = mDeviceNbButtons;

  for (auto& iterator : mNameMap)
  {
    if(!iterator.second.configured)
      continue;

    pugi::xml_node input = cfg.append_child("input");
    input.append_attribute("name") = iterator.first.c_str();
    input.append_attribute("type") = inputTypeToString(iterator.second.type).c_str();
    input.append_attribute("id").set_value(iterator.second.id);
    input.append_attribute("value").set_value(iterator.second.value);
    if(iterator.second.code != -1) {
      input.append_attribute("code").set_value(iterator.second.code);
    }
  }
}
