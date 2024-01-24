//
// Created by bkg2k on 16/12/23.
//

#include "ThemeSupport.h"
#include "PropertyNames.h"

ThemePropertyType ThemeSupport::sArray[(int)ThemePropertyName::Count__];

void ThemeSupport::InitializeStatics()
{
  static bool sInitialized = false;
  if (sInitialized) return;
  sInitialized = true;

  // Intialise property name => type
  for(int i = (int)ThemePropertyName::Count__; --i >= 0;)
    switch((ThemePropertyName)i)
    {
      case ThemePropertyName::Alignment:
      case ThemePropertyName::Animations: sArray[i] = ThemePropertyType::String; break;
      case ThemePropertyName::BackgroundColor:
      case ThemePropertyName::Color:
      case ThemePropertyName::ColorTop:
      case ThemePropertyName::ColorBottom:
      case ThemePropertyName::ColorLeft:
      case ThemePropertyName::ColorRight:
      case ThemePropertyName::ColorTopLeft:
      case ThemePropertyName::ColorTopRight:
      case ThemePropertyName::ColorBottomLeft:
      case ThemePropertyName::ColorBottomRight: sArray[i] = ThemePropertyType::Color; break;
      case ThemePropertyName::DefaultTransition: sArray[i] = ThemePropertyType::String; break;
      case ThemePropertyName::Delay: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::Disabled: sArray[i] = ThemePropertyType::Boolean; break;
      case ThemePropertyName::Display: sArray[i] = ThemePropertyType::String; break;
      case ThemePropertyName::FadePath:
      case ThemePropertyName::FilledPath:
      case ThemePropertyName::FontPath: sArray[i] = ThemePropertyType::Path; break;
      case ThemePropertyName::FontSize: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::FontColor: sArray[i] = ThemePropertyType::Color; break;
      case ThemePropertyName::ForceUppercase: sArray[i] = ThemePropertyType::Boolean; break;
      case ThemePropertyName::Height:
      case ThemePropertyName::HorizontalMargin: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::IconA:
      case ThemePropertyName::IconAdvanced:
      case ThemePropertyName::IconArcade:
      case ThemePropertyName::IconB: sArray[i] = ThemePropertyType::Path; break;
      case ThemePropertyName::IconColor: sArray[i] = ThemePropertyType::Color; break;
      case ThemePropertyName::IconControllers:
      case ThemePropertyName::IconDownload:
      case ThemePropertyName::IconFastShutdown:
      case ThemePropertyName::IconGames:
      case ThemePropertyName::IconKodi:
      case ThemePropertyName::IconL:
      case ThemePropertyName::IconLeftRight:
      case ThemePropertyName::IconLicense:
      case ThemePropertyName::IconNetwork:
      case ThemePropertyName::IconQuit:
      case ThemePropertyName::IconR:
      case ThemePropertyName::IconRecalboxRGBDual:
      case ThemePropertyName::IconRestart:
      case ThemePropertyName::IconScraper:
      case ThemePropertyName::IconSelect:
      case ThemePropertyName::IconShutdown:
      case ThemePropertyName::IconSound:
      case ThemePropertyName::IconStart:
      case ThemePropertyName::IconSystem:
      case ThemePropertyName::IconTate:
      case ThemePropertyName::IconUI:
      case ThemePropertyName::IconUpDown:
      case ThemePropertyName::IconUpDownLeftRight:
      case ThemePropertyName::IconUpdates:
      case ThemePropertyName::IconX:
      case ThemePropertyName::IconY: sArray[i] = ThemePropertyType::Path; break;
      case ThemePropertyName::LineSpacing: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::Link:
      case ThemePropertyName::LogoAlignment: sArray[i] = ThemePropertyType::String; break;
      case ThemePropertyName::LogoRotation: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::LogoRotationOrigin: sArray[i] = ThemePropertyType::NormalizedPair; break;
      case ThemePropertyName::LogoScale: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::LogoSize: sArray[i] = ThemePropertyType::NormalizedPair; break;
      case ThemePropertyName::Loops:
      case ThemePropertyName::MaxLogoCount: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::MaxSize:
      case ThemePropertyName::Origin: sArray[i] = ThemePropertyType::NormalizedPair; break;
      case ThemePropertyName::Path:
      case ThemePropertyName::PathOff:
      case ThemePropertyName::PathOn: sArray[i] = ThemePropertyType::Path; break;
      case ThemePropertyName::Pos: sArray[i] = ThemePropertyType::NormalizedPair; break;
      case ThemePropertyName::PrimaryColor: sArray[i] = ThemePropertyType::Color; break;
      case ThemePropertyName::Reflection: sArray[i] = ThemePropertyType::NormalizedPair; break;
      case ThemePropertyName::Rotation: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::RotationOrigin: sArray[i] = ThemePropertyType::NormalizedPair; break;
      case ThemePropertyName::ScrollSound: sArray[i] = ThemePropertyType::Path; break;
      case ThemePropertyName::SecondaryColor:
      case ThemePropertyName::SelectedColor:
      case ThemePropertyName::SelectorColor: sArray[i] = ThemePropertyType::Color; break;
      case ThemePropertyName::SelectorHeight: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::SelectorImagePath: sArray[i] = ThemePropertyType::Path; break;
      case ThemePropertyName::SelectorImageTile: sArray[i] = ThemePropertyType::Boolean; break;
      case ThemePropertyName::SelectorOffsetY: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::SeparatorColor: sArray[i] = ThemePropertyType::Color; break;
      case ThemePropertyName::Size: sArray[i] = ThemePropertyType::NormalizedPair; break;
      case ThemePropertyName::Text: sArray[i] = ThemePropertyType::String; break;
      case ThemePropertyName::TextColor: sArray[i] = ThemePropertyType::Color; break;
      case ThemePropertyName::Tile: sArray[i] = ThemePropertyType::Boolean; break;
      case ThemePropertyName::Type: sArray[i] = ThemePropertyType::String; break;
      case ThemePropertyName::UnfilledPath: sArray[i] = ThemePropertyType::Path; break;
      case ThemePropertyName::Value: sArray[i] = ThemePropertyType::String; break;
      case ThemePropertyName::ZIndex: sArray[i] = ThemePropertyType::Float; break;
      case ThemePropertyName::Count__:
      default: abort();
    }
}

HashSet<String>& ThemeSupport::SupportedViews()
{
  static HashSet<String> sSupportedViews;
  if (sSupportedViews.empty())
    for(const String& value : String("system,basic,detailed,menu,gameclip").Split(','))
      sSupportedViews.insert(value);
  return sSupportedViews;
}

HashSet<String>& ThemeSupport::SupportedFeatures()
{
  static HashSet<String> sSupportedFeatures;
  if (sSupportedFeatures.empty())
    for(const String& value : String("carousel,z-index").Split(','))
      sSupportedFeatures.insert(value);
  return sSupportedFeatures;
}

HashMap<ThemeElementType, ThemePropertyNameBits>& ThemeSupport::ElementMap()
{
  static HashMap<ThemeElementType, ThemePropertyNameBits> sElementMap =
    {
      {
        ThemeElementType::Image,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::Size,
          ThemePropertyName::MaxSize,
          ThemePropertyName::Origin,
          ThemePropertyName::Rotation,
          ThemePropertyName::RotationOrigin,
          ThemePropertyName::Path,
          ThemePropertyName::Tile,
          ThemePropertyName::Color,
          ThemePropertyName::ZIndex,
          ThemePropertyName::Disabled,
          ThemePropertyName::Reflection,
        },
      },
      {
        ThemeElementType::Box,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::Size,
          ThemePropertyName::Origin,
          ThemePropertyName::Rotation,
          ThemePropertyName::RotationOrigin,
          ThemePropertyName::Color,
          ThemePropertyName::ColorTop,
          ThemePropertyName::ColorBottom,
          ThemePropertyName::ColorLeft,
          ThemePropertyName::ColorRight,
          ThemePropertyName::ColorTopLeft,
          ThemePropertyName::ColorTopRight,
          ThemePropertyName::ColorBottomLeft,
          ThemePropertyName::ColorBottomRight,
          ThemePropertyName::ZIndex,
          ThemePropertyName::Disabled,
        },
      },
      {
        ThemeElementType::Video,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::Size,
          ThemePropertyName::MaxSize,
          ThemePropertyName::Origin,
          ThemePropertyName::Rotation,
          ThemePropertyName::RotationOrigin,
          ThemePropertyName::Path,
          ThemePropertyName::ZIndex,
          ThemePropertyName::Disabled,
          ThemePropertyName::Animations,
          ThemePropertyName::Loops,
          ThemePropertyName::Delay,
          ThemePropertyName::Reflection,
          ThemePropertyName::Link,
        },
      },
      {
        ThemeElementType::Text,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::Size,
          ThemePropertyName::Origin,
          ThemePropertyName::Rotation,
          ThemePropertyName::RotationOrigin,
          ThemePropertyName::Text,
          ThemePropertyName::Path,
          ThemePropertyName::BackgroundColor,
          ThemePropertyName::FontPath,
          ThemePropertyName::FontSize,
          ThemePropertyName::Color,
          ThemePropertyName::Alignment,
          ThemePropertyName::ForceUppercase,
          ThemePropertyName::LineSpacing,
          ThemePropertyName::Value,
          ThemePropertyName::ZIndex,
          ThemePropertyName::Disabled,
        },
      },
      {
        ThemeElementType::ScrollText,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::Size,
          ThemePropertyName::Origin,
          ThemePropertyName::Rotation,
          ThemePropertyName::RotationOrigin,
          ThemePropertyName::Text,
          ThemePropertyName::Path,
          ThemePropertyName::BackgroundColor,
          ThemePropertyName::FontPath,
          ThemePropertyName::FontSize,
          ThemePropertyName::Color,
          ThemePropertyName::Alignment,
          ThemePropertyName::ForceUppercase,
          ThemePropertyName::Value,
          ThemePropertyName::ZIndex,
          ThemePropertyName::Disabled,
        },
      },
      {
        ThemeElementType::TextList,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::Size,
          ThemePropertyName::Origin,
          ThemePropertyName::SelectorHeight,
          ThemePropertyName::SelectorOffsetY,
          ThemePropertyName::SelectorColor,
          ThemePropertyName::SelectorImagePath,
          ThemePropertyName::SelectorImageTile,
          ThemePropertyName::SelectedColor,
          ThemePropertyName::PrimaryColor,
          ThemePropertyName::SecondaryColor,
          ThemePropertyName::FontPath,
          ThemePropertyName::FontSize,
          ThemePropertyName::ScrollSound,
          ThemePropertyName::Alignment,
          ThemePropertyName::HorizontalMargin,
          ThemePropertyName::ForceUppercase,
          ThemePropertyName::LineSpacing,
          ThemePropertyName::ZIndex,
        },
      },
      {
        ThemeElementType::Container,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::Size,
          ThemePropertyName::Origin,
          ThemePropertyName::ZIndex,
          ThemePropertyName::Disabled,
        },
      },
      {
        ThemeElementType::NinePatch,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::Size,
          ThemePropertyName::Path,
          ThemePropertyName::ZIndex,
          ThemePropertyName::Disabled,
        },
      },
      {
        ThemeElementType::DateTime,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::Size,
          ThemePropertyName::Origin,
          ThemePropertyName::Color,
          ThemePropertyName::FontPath,
          ThemePropertyName::FontSize,
          ThemePropertyName::Alignment,
          ThemePropertyName::ForceUppercase,
          ThemePropertyName::ZIndex,
          ThemePropertyName::Disabled,
          ThemePropertyName::Display,
        },
      },
      {
        ThemeElementType::Rating,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::Size,
          ThemePropertyName::Origin,
          ThemePropertyName::Rotation,
          ThemePropertyName::RotationOrigin,
          ThemePropertyName::FilledPath,
          ThemePropertyName::UnfilledPath,
          ThemePropertyName::ZIndex,
          ThemePropertyName::Disabled,
        },
      },
      {
        ThemeElementType::Sound,
        {
          ThemePropertyName::Path,
        },
      },
      {
        ThemeElementType::HelpSystem,
        {
          ThemePropertyName::Pos,
          ThemePropertyName::TextColor,
          ThemePropertyName::IconColor,
          ThemePropertyName::FontPath,
          ThemePropertyName::FontSize,
          ThemePropertyName::IconUpDown,
          ThemePropertyName::IconLeftRight,
          ThemePropertyName::IconUpDownLeftRight,
          ThemePropertyName::IconA,
          ThemePropertyName::IconB,
          ThemePropertyName::IconX,
          ThemePropertyName::IconY,
          ThemePropertyName::IconL,
          ThemePropertyName::IconR,
          ThemePropertyName::IconStart,
          ThemePropertyName::IconSelect,
        },
      },
      {
        ThemeElementType::Carousel,
        {
          ThemePropertyName::Type,
          ThemePropertyName::Size,
          ThemePropertyName::Pos,
          ThemePropertyName::Origin,
          ThemePropertyName::Color,
          ThemePropertyName::FontPath,
          ThemePropertyName::LogoScale,
          ThemePropertyName::LogoRotation,
          ThemePropertyName::LogoRotationOrigin,
          ThemePropertyName::LogoSize,
          ThemePropertyName::LogoAlignment,
          ThemePropertyName::MaxLogoCount,
          ThemePropertyName::DefaultTransition,
          ThemePropertyName::ZIndex,
        },
      },
      {
        ThemeElementType::MenuBackground,
        {
          ThemePropertyName::Color,
          ThemePropertyName::Path,
          ThemePropertyName::FadePath,
        },
      },
      {
        ThemeElementType::MenuIcons,
        {
          ThemePropertyName::IconKodi,
          ThemePropertyName::IconSystem,
          ThemePropertyName::IconSystem,
          ThemePropertyName::IconUpdates,
          ThemePropertyName::IconControllers,
          ThemePropertyName::IconGames,
          ThemePropertyName::IconUI,
          ThemePropertyName::IconSound,
          ThemePropertyName::IconNetwork,
          ThemePropertyName::IconScraper,
          ThemePropertyName::IconAdvanced,
          ThemePropertyName::IconQuit,
          ThemePropertyName::IconRestart,
          ThemePropertyName::IconShutdown,
          ThemePropertyName::IconFastShutdown,
          ThemePropertyName::IconLicense,
          ThemePropertyName::IconRecalboxRGBDual,
          ThemePropertyName::IconTate,
          ThemePropertyName::IconArcade,
          ThemePropertyName::IconDownload,
        },
      },
      {
        ThemeElementType::MenuSwitch,
        {
          ThemePropertyName::PathOn,
          ThemePropertyName::PathOff,
        },
      },
      {
        ThemeElementType::MenuSlider,
        {
          ThemePropertyName::Path,
        },
      },
      {
        ThemeElementType::MenuButton,
        {
          ThemePropertyName::Path,
          ThemePropertyName::FilledPath,
        },
      },
      {
        ThemeElementType::MenuText,
        {
          ThemePropertyName::FontPath,
          ThemePropertyName::FontSize,
          ThemePropertyName::Color,
          ThemePropertyName::SeparatorColor,
          ThemePropertyName::SelectedColor,
          ThemePropertyName::SelectorColor,
        },
      },
      {
        ThemeElementType::MenuTextSmall,
        {
          ThemePropertyName::FontPath,
          ThemePropertyName::FontSize,
          ThemePropertyName::Color,
          ThemePropertyName::SelectedColor,
          ThemePropertyName::SelectorColor,
        },
      },
      {
        ThemeElementType::MenuSize,
        {
          ThemePropertyName::Height,
        },
      }
    };
  return sElementMap;
}

HashMap<String, ThemeElementType>& ThemeSupport::ElementType()
{
  static HashMap<String, ThemeElementType> sTypes
  {
    { "image", ThemeElementType::Image },
    { "box", ThemeElementType::Box },
    { "video", ThemeElementType::Video },
    { "text", ThemeElementType::Text },
    { "scrolltext", ThemeElementType::ScrollText },
    { "textlist", ThemeElementType::TextList },
    { "container", ThemeElementType::Container },
    { "ninepatch", ThemeElementType::NinePatch },
    { "datetime", ThemeElementType::DateTime },
    { "rating", ThemeElementType::Rating },
    { "sound", ThemeElementType::Sound },
    { "helpsystem", ThemeElementType::HelpSystem },
    { "carousel", ThemeElementType::Carousel },
    { "menuBackground", ThemeElementType::MenuBackground },
    { "menuIcons", ThemeElementType::MenuIcons },
    { "menuSwitch", ThemeElementType::MenuSwitch },
    { "menuSlider", ThemeElementType::MenuSlider },
    { "menuButton", ThemeElementType::MenuButton },
    { "menuText", ThemeElementType::MenuText },
    { "menuTextSmall", ThemeElementType::MenuTextSmall },
    { "menuSize", ThemeElementType::MenuSize },
  };

  return sTypes;
}

HashMap<String, ThemePropertyName>& ThemeSupport::PropertyName()
{
  static HashMap<String, ThemePropertyName> sNames
  {
    { "alignment", ThemePropertyName::Alignment },
    { "animations", ThemePropertyName::Animations },
    { "backgroundColor", ThemePropertyName::BackgroundColor },
    { "color", ThemePropertyName::Color },
    { "colorTop", ThemePropertyName::ColorTop },
    { "colorBottom", ThemePropertyName::ColorBottom },
    { "colorLeft", ThemePropertyName::ColorLeft },
    { "colorRight", ThemePropertyName::ColorRight },
    { "colorTopLeft", ThemePropertyName::ColorTopLeft },
    { "colorTopRight", ThemePropertyName::ColorTopRight },
    { "colorBottomLeft", ThemePropertyName::ColorBottomLeft },
    { "colorBottomRight", ThemePropertyName::ColorBottomRight },
    { "defaultTransition", ThemePropertyName::DefaultTransition },
    { "delay", ThemePropertyName::Delay },
    { "disabled", ThemePropertyName::Disabled },
    { "display", ThemePropertyName::Display },
    { "fadePath", ThemePropertyName::FadePath },
    { "filledPath", ThemePropertyName::FilledPath },
    { "fontPath", ThemePropertyName::FontPath },
    { "fontSize", ThemePropertyName::FontSize },
    { "fontColor", ThemePropertyName::FontColor },
    { "forceUppercase", ThemePropertyName::ForceUppercase },
    { "height", ThemePropertyName::Height },
    { "horizontalMargin", ThemePropertyName::HorizontalMargin },
    { "iconA", ThemePropertyName::IconA },
    { "iconAdvanced", ThemePropertyName::IconAdvanced },
    { "iconArcade", ThemePropertyName::IconArcade },
    { "iconB", ThemePropertyName::IconB },
    { "iconColor", ThemePropertyName::IconColor },
    { "iconControllers", ThemePropertyName::IconControllers },
    { "iconDownload", ThemePropertyName::IconDownload },
    { "iconFastShutdown", ThemePropertyName::IconFastShutdown },
    { "iconGames", ThemePropertyName::IconGames },
    { "iconKodi", ThemePropertyName::IconKodi },
    { "iconL", ThemePropertyName::IconL },
    { "iconLeftRight", ThemePropertyName::IconLeftRight },
    { "iconLicense", ThemePropertyName::IconLicense },
    { "iconNetwork", ThemePropertyName::IconNetwork },
    { "iconQuit", ThemePropertyName::IconQuit },
    { "iconR", ThemePropertyName::IconR },
    { "iconRecalboxRGBDual", ThemePropertyName::IconRecalboxRGBDual },
    { "iconRestart", ThemePropertyName::IconRestart },
    { "iconScraper", ThemePropertyName::IconScraper },
    { "iconSelect", ThemePropertyName::IconSelect },
    { "iconShutdown", ThemePropertyName::IconShutdown },
    { "iconSound", ThemePropertyName::IconSound },
    { "iconStart", ThemePropertyName::IconStart },
    { "iconSystem", ThemePropertyName::IconSystem },
    { "iconTate", ThemePropertyName::IconTate },
    { "iconUI", ThemePropertyName::IconUI },
    { "iconUpDown", ThemePropertyName::IconUpDown },
    { "iconUpDownLeftRight", ThemePropertyName::IconUpDownLeftRight },
    { "iconUpdates", ThemePropertyName::IconUpdates },
    { "iconX", ThemePropertyName::IconX },
    { "iconY", ThemePropertyName::IconY },
    { "lineSpacing", ThemePropertyName::LineSpacing },
    { "link", ThemePropertyName::Link },
    { "logoAlignment", ThemePropertyName::LogoAlignment },
    { "logoRotation", ThemePropertyName::LogoRotation },
    { "logoRotationOrigin", ThemePropertyName::LogoRotationOrigin },
    { "logoScale", ThemePropertyName::LogoScale },
    { "logoSize", ThemePropertyName::LogoSize },
    { "loops", ThemePropertyName::Loops },
    { "maxLogoCount", ThemePropertyName::MaxLogoCount },
    { "maxSize", ThemePropertyName::MaxSize },
    { "origin", ThemePropertyName::Origin },
    { "path", ThemePropertyName::Path },
    { "pathOff", ThemePropertyName::PathOff },
    { "pathOn", ThemePropertyName::PathOn },
    { "pos", ThemePropertyName::Pos },
    { "primaryColor", ThemePropertyName::PrimaryColor },
    { "reflection", ThemePropertyName::Reflection },
    { "rotation", ThemePropertyName::Rotation },
    { "rotationOrigin", ThemePropertyName::RotationOrigin },
    { "scrollSound", ThemePropertyName::ScrollSound },
    { "secondaryColor", ThemePropertyName::SecondaryColor },
    { "selectedColor", ThemePropertyName::SelectedColor },
    { "selectorColor", ThemePropertyName::SelectorColor },
    { "selectorHeight", ThemePropertyName::SelectorHeight },
    { "selectorImagePath", ThemePropertyName::SelectorImagePath },
    { "selectorImageTile", ThemePropertyName::SelectorImageTile },
    { "selectorOffsetY", ThemePropertyName::SelectorOffsetY },
    { "separatorColor", ThemePropertyName::SeparatorColor },
    { "size", ThemePropertyName::Size },
    { "text", ThemePropertyName::Text },
    { "textColor", ThemePropertyName::TextColor },
    { "tile", ThemePropertyName::Tile },
    { "type", ThemePropertyName::Type },
    { "unfilledPath", ThemePropertyName::UnfilledPath },
    { "value", ThemePropertyName::Value },
    { "zIndex", ThemePropertyName::ZIndex },
  };

  return sNames;
}

String ThemeSupport::ReversePropertyName(ThemePropertyName name)
{
  static HashMap<ThemePropertyName, String> sNames
  {
    { ThemePropertyName::Alignment, "alignment" },
    { ThemePropertyName::Animations, "animations" },
    { ThemePropertyName::BackgroundColor, "backgroundColor" },
    { ThemePropertyName::Color, "color" },
    { ThemePropertyName::ColorTop, "colorTop" },
    { ThemePropertyName::ColorBottom, "colorBottom" },
    { ThemePropertyName::ColorLeft, "colorLeft" },
    { ThemePropertyName::ColorRight, "colorRight" },
    { ThemePropertyName::ColorTopLeft, "colorTopLeft" },
    { ThemePropertyName::ColorTopRight, "colorTopRight" },
    { ThemePropertyName::ColorBottomLeft, "colorBottomLeft" },
    { ThemePropertyName::ColorBottomRight, "colorBottomRight" },
    { ThemePropertyName::DefaultTransition, "defaultTransition" },
    { ThemePropertyName::Delay, "delay" },
    { ThemePropertyName::Disabled, "disabled" },
    { ThemePropertyName::Display, "display" },
    { ThemePropertyName::FadePath, "fadePath" },
    { ThemePropertyName::FilledPath, "filledPath" },
    { ThemePropertyName::FontPath, "fontPath" },
    { ThemePropertyName::FontSize, "fontSize" },
    { ThemePropertyName::FontColor, "fontColor" },
    { ThemePropertyName::ForceUppercase, "forceUppercase" },
    { ThemePropertyName::Height, "height" },
    { ThemePropertyName::HorizontalMargin, "horizontalMargin" },
    { ThemePropertyName::IconA, "iconA" },
    { ThemePropertyName::IconAdvanced, "iconAdvanced" },
    { ThemePropertyName::IconArcade, "iconArcade" },
    { ThemePropertyName::IconB, "iconB" },
    { ThemePropertyName::IconColor, "iconColor" },
    { ThemePropertyName::IconControllers, "iconControllers" },
    { ThemePropertyName::IconDownload, "iconDownload" },
    { ThemePropertyName::IconFastShutdown, "iconFastShutdown" },
    { ThemePropertyName::IconGames, "iconGames" },
    { ThemePropertyName::IconKodi, "iconKodi" },
    { ThemePropertyName::IconL, "iconL" },
    { ThemePropertyName::IconLeftRight, "iconLeftRight" },
    { ThemePropertyName::IconLicense, "iconLicense" },
    { ThemePropertyName::IconNetwork, "iconNetwork" },
    { ThemePropertyName::IconQuit, "iconQuit" },
    { ThemePropertyName::IconR, "iconR" },
    { ThemePropertyName::IconRecalboxRGBDual, "iconRecalboxRGBDual" },
    { ThemePropertyName::IconRestart, "iconRestart" },
    { ThemePropertyName::IconScraper, "iconScraper" },
    { ThemePropertyName::IconSelect, "iconSelect" },
    { ThemePropertyName::IconShutdown, "iconShutdown" },
    { ThemePropertyName::IconSound, "iconSound" },
    { ThemePropertyName::IconStart, "iconStart" },
    { ThemePropertyName::IconSystem, "iconSystem" },
    { ThemePropertyName::IconTate, "iconTate" },
    { ThemePropertyName::IconUI, "iconUI" },
    { ThemePropertyName::IconUpDown, "iconUpDown" },
    { ThemePropertyName::IconUpDownLeftRight, "iconUpDownLeftRight" },
    { ThemePropertyName::IconUpdates, "iconUpdates" },
    { ThemePropertyName::IconX, "iconX" },
    { ThemePropertyName::IconY, "iconY" },
    { ThemePropertyName::LineSpacing, "lineSpacing" },
    { ThemePropertyName::Link, "link" },
    { ThemePropertyName::LogoAlignment, "logoAlignment" },
    { ThemePropertyName::LogoRotation, "logoRotation" },
    { ThemePropertyName::LogoRotationOrigin, "logoRotationOrigin" },
    { ThemePropertyName::LogoScale, "logoScale" },
    { ThemePropertyName::LogoSize, "logoSize" },
    { ThemePropertyName::Loops, "loops" },
    { ThemePropertyName::MaxLogoCount, "maxLogoCount" },
    { ThemePropertyName::MaxSize, "maxSize" },
    { ThemePropertyName::Origin, "origin" },
    { ThemePropertyName::Path, "path" },
    { ThemePropertyName::PathOff, "pathOff" },
    { ThemePropertyName::PathOn, "pathOn" },
    { ThemePropertyName::Pos, "pos" },
    { ThemePropertyName::PrimaryColor, "primaryColor" },
    { ThemePropertyName::Rotation, "rotation" },
    { ThemePropertyName::RotationOrigin, "rotationOrigin" },
    { ThemePropertyName::ScrollSound, "scrollSound" },
    { ThemePropertyName::SecondaryColor, "secondaryColor" },
    { ThemePropertyName::SelectedColor, "selectedColor" },
    { ThemePropertyName::SelectorColor, "selectorColor" },
    { ThemePropertyName::SelectorHeight, "selectorHeight" },
    { ThemePropertyName::SelectorImagePath, "selectorImagePath" },
    { ThemePropertyName::SelectorImageTile, "selectorImageTile" },
    { ThemePropertyName::SelectorOffsetY, "selectorOffsetY" },
    { ThemePropertyName::SeparatorColor, "separatorColor" },
    { ThemePropertyName::Size, "size" },
    { ThemePropertyName::Text, "text" },
    { ThemePropertyName::TextColor, "textColor" },
    { ThemePropertyName::Tile, "tile" },
    { ThemePropertyName::Type, "type" },
    { ThemePropertyName::UnfilledPath, "unfilledPath" },
    { ThemePropertyName::Value, "value" },
    { ThemePropertyName::ZIndex, "zIndex" },
  };

  String* sname = sNames.try_get(name);
  return sname != nullptr ? *sname : "Unknown";
}
