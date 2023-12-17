//
// Created by bkg2k on 09/11/2019.
//
#pragma once

#include <map>
#include <utils/String.h>
#include <utils/os/fs/Path.h>
#include <utils/math/Vector2f.h>
#include "ElementTypes.h"
#include "PropertyNames.h"
#include "utils/storage/HashMap.h"

class ThemeElement
{
  private:
    //! Property type
    enum class Type: char
    {
      Vector,
      String,
      Integer,
      Float,
      Bool,
    };
    //! Bag of data
    class PropertyBag
    {
      private:
        String mString;
        union // Save some space by union-ing cardinal exclusive values
        {
          int mInteger;
          float mFloat;
          bool mBool;
        };
        float mSecondFloat;
        Type mType;

      public:
        PropertyBag()                              : mInteger(0), mSecondFloat(0.0f), mType(Type::String) {}
        explicit PropertyBag(const Vector2f& v)    : mFloat(v.x()), mSecondFloat(v.y()), mType(Type::Vector) {}
        explicit PropertyBag(float v1, float v2)   : mFloat(v1), mSecondFloat(v2), mType(Type::Vector) {}
        explicit PropertyBag(const String& s) : mString(s), mInteger(0), mSecondFloat(0.0f), mType(Type::String) {}
        explicit PropertyBag(int v)                : mInteger(v), mSecondFloat(0.0f), mType(Type::Integer) {}
        explicit PropertyBag(float v)              : mFloat(v), mSecondFloat(0.0f), mType(Type::Float) {}
        explicit PropertyBag(bool v)               : mBool(v), mSecondFloat(0.0f), mType(Type::Bool) {}

        [[nodiscard]] String AsString() const
        {
          switch(mType)
          {
            case Type::Vector: return String(mFloat, 4).Append(' ').Append(mSecondFloat, 4);
            case Type::String: return mString;
            case Type::Integer: return String(mInteger);
            case Type::Float: return String(mFloat, 4);
            case Type::Bool: return String(mBool ? '1' : '0');
          }
          return String();
        }
        [[nodiscard]] Path AsPath() const
        {
          switch(mType)
          {
            case Type::Vector: return Path(String(mFloat, 4).Append(' ').Append(mSecondFloat, 4));
            case Type::String: return Path(mString);
            case Type::Integer: return Path(String(mInteger));
            case Type::Float: return Path(String(mFloat, 4));
            case Type::Bool: return Path(String(mBool ? '1' : '0'));
          }
          return Path();
        }
        [[nodiscard]] int AsInt() const
        {
          switch(mType)
          {
            case Type::String: { int result = 0; return mString.TryAsInt(result) ? result : 0; }
            case Type::Integer: return mInteger;
            case Type::Vector:
            case Type::Float: return (int)mFloat;
            case Type::Bool: return (int)mBool;
          }
          return 0;
        }
        [[nodiscard]] float AsFloat() const
        {
          switch(mType)
          {
            case Type::String: { float result = 0; return mString.TryAsFloat(result) ? result : 0.0f; }
            case Type::Integer: return (float)mInteger;
            case Type::Vector:
            case Type::Float: return mFloat;
            case Type::Bool: return (float)mBool;
          }
          return 0.0f;
        }
        [[nodiscard]] bool AsBool() const
        {
          switch(mType)
          {
            case Type::String: { bool result = false; if (mString.TryAsBool(result)) return result; return false; }
            case Type::Integer: return (bool)mInteger;
            case Type::Vector:
            case Type::Float: return (bool)mFloat;
            case Type::Bool: return mBool;
          }
          return false;
        }
        [[nodiscard]] Vector2f AsVector() const
        {
          switch(mType)
          {
            case Type::Vector: return { mFloat, mSecondFloat };
            case Type::String:
            {
              float x = 0;
              float y = 0;
              if (mString.TryAsFloat(0, ' ', x))
              {
                int pos = mString.Find(' ');
                if (pos >= 0)
                  if (mString.TryAsFloat((int) pos + 1, 0, y))
                    return { x, y };
              }
              break;
            }
            case Type::Integer: return { (float)mInteger, (float)mInteger };
            case Type::Float: return { mFloat, mFloat };
            case Type::Bool: return {(float)mBool, (float)mBool};
          }
          return { 0.0f, 0.0f };
        }
    };

    //! Properties
    HashMap<ThemePropertyName, PropertyBag> mProperties;
    //! Property map
    ThemePropertyNameBits mAvailableProperties;
    //! Element name
    String mName;
    //! Element type
    ThemeElementType mType;
    //! Is extra element
    bool mExtra;

  public:
    /*!
     * @brief Constructor
     * @param name Element name
     * @param type Element type
     * @param extra Element is an extra
     */
    ThemeElement(const String& name, ThemeElementType type, bool extra)
    {
      mName = name;
      mType = type;
      mExtra = extra;
    }

    [[nodiscard]] String AsString(ThemePropertyName name) const
    {
      PropertyBag* bag = mProperties.try_get(name);
      return bag != nullptr ? bag->AsString() : String::Empty;
    }

    [[nodiscard]] Path AsPath(ThemePropertyName name) const
    {
      PropertyBag* bag = mProperties.try_get(name);
      return bag != nullptr ? bag->AsPath() : Path::Empty;
    }

    [[nodiscard]] int AsInt(ThemePropertyName name) const
    {
      PropertyBag* bag = mProperties.try_get(name);
      return bag != nullptr ? bag->AsInt() : 0;
    }

    [[nodiscard]] float AsFloat(ThemePropertyName name) const
    {
      PropertyBag* bag = mProperties.try_get(name);
      return bag != nullptr ? bag->AsFloat() : 0.f;
    }

    [[nodiscard]] bool AsBool(ThemePropertyName name) const
    {
      PropertyBag* bag = mProperties.try_get(name);
      return bag != nullptr && bag->AsBool();
    }

    [[nodiscard]] Vector2f AsVector(ThemePropertyName name) const
    {
      PropertyBag* bag = mProperties.try_get(name);
      return bag != nullptr ? bag->AsVector() : Vector2f(0.0f, 0.0f);
    }

    [[nodiscard]] bool HasProperty(ThemePropertyName property) const { return mAvailableProperties.IsSet(property); }

    [[nodiscard]] bool HasProperties() const { return !mProperties.empty(); }

    [[nodiscard]] const String& Name() const { return mName; }

    [[nodiscard]] ThemeElementType Type() const { return mType; }

    [[nodiscard]] bool Extra() const { return mExtra; }

    void AddVectorProperty(ThemePropertyName name, float x, float y) { mProperties[name] = PropertyBag(x, y); mAvailableProperties.Set(name); }
    void AddVectorProperty(ThemePropertyName name, const Vector2f& v) { mProperties[name] = PropertyBag(v); mAvailableProperties.Set(name); }
    void AddStringProperty(ThemePropertyName name, const String& s) { mProperties[name] = PropertyBag(s); mAvailableProperties.Set(name); }
    void AddIntProperty(ThemePropertyName name, int v) { mProperties[name] = PropertyBag(v); mAvailableProperties.Set(name); }
    void AddFloatProperty(ThemePropertyName name, float v) { mProperties[name] = PropertyBag(v); mAvailableProperties.Set(name); }
    void AddBoolProperty(ThemePropertyName name, bool v) { mProperties[name] = PropertyBag(v); mAvailableProperties.Set(name); }
};

