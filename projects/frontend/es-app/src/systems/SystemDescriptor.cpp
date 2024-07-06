//
// Created by bkg2k on 19/06/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#include <systems/SystemDescriptor.h>

String SystemDescriptor::mDefaultCommand;  //!< Default command
String SystemDescriptor::IconPrefix() const
{
  return String().AssignUTF8(mIcon).Append(' ');
}

void SystemDescriptor::StoreExtensionTypes(const String& extensiontypes)
{
  if (extensiontypes.empty()) return;
  String key;
  String value;
  for(const String& group : extensiontypes.Split(','))
    if (group.Extract('=', key, value, true))
    {
      SupportType type = ConvertSupportType(value);
      for(const String& extension: key.Split('|'))
        mExtensionTypes.Add({ ExtensionTo8Bytes(extension), type });
    }
    else
    {
      LOG(LogError) << "[SystemDescriptor] Malformed extensiontypes string: " << extensiontypes;
    }
}

SystemDescriptor::SupportType SystemDescriptor::LookupSupportType(const String& extension)
{
  long long int ext = ExtensionTo8Bytes(extension.ToTrimLeft('.').LowerCase());
  // Regular extension
  for(const ExtToType& e : mExtensionTypes)
    if (e.mExtension == ext)
      return e.mType;
  // All extension jocker?
  for(const ExtToType& e : mExtensionTypes)
    if (e.mExtension == '*')
      return e.mType;
  // Unknown
  return SupportType::Unknown;
}
