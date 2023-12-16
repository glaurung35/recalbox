//
// Created by bkg2k on 14/12/23.
//
#pragma once

#include <themes/Properties.h>
#include <utils/String.h>

// Forward declaration
class Component;
class ThemeData;
class ThemeElement;

class Themable
{
  public:
    /*!
     * @brief Constructor
     * @param component Reference constructor
     */
    explicit Themable(Component* component)
      : mComponent(*component)
    {
    }

    /*!
     * @brief Called to apply a theme element on the current component
     * @param theme Theme
     * @param view View name
     * @param element Element name
     * @param properties Properties to update
     */
    void DoApplyThemeElement(const ThemeData& theme, const String& viewName, const String& elementName, ThemePropertiesType properties);

  protected:
    /*!
     * @brief Force implementations to return a theme element type
     * @return Element type
     */
    virtual String ThemeElementType() const = 0;

    /*!
     * @brief Called when a theme element is applyed on the current component.
     * @param element Element instance
     * @param properties Properties to update
     */
    virtual void OnApplyThemeElement(const ThemeElement& element, ThemePropertiesType properties) { (void)element; (void)properties; }

  private:
    /*!
     * @brief Called when a theme element is applyed on the current component.
     * Actually, this implementation only takes properties related to the Component
     * and is called from DoApplyThemeElement.
     * There is no need for overridden implementation to call the component implementation.
     * Also no need for overridden implementation to process pos/size/origin and all basic component properties
     * @param element Element instance
     * @param properties Properties to update
     */
    void OnApplyThemeElementBase(const ThemeElement& element, ThemePropertiesType properties);

    //! Component reference
    Component& mComponent;
};
