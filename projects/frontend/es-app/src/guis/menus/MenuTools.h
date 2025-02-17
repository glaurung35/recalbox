//
// Created by bkg2k on 06/01/2021.
//
// As part of the RECALBOX Project
// http://www.recalbox.com
//

#pragma once

#include <utils/String.h>
#include <vector>

// Forward declarations
class SystemManager;
class SystemData;

class MenuTools
{
  public:
    struct Shader
    {
      Path ShaderPath;
      String Displayable;
    };

    //! Shader structure list
    typedef std::vector<Shader> ShaderList;

    //! Structure used to display Emulator & Core in menus
    struct EmulatorAndCore
    {
      String Identifier;  //!< Identifier
      String Displayable; //!< Displayable emulator or emulator/core
      bool Selected;           //!< True if this is the selected item
    };

    //! List of Emulator & core structure
    typedef std::vector<EmulatorAndCore> EmulatorAndCoreList;

    /*!
     * @brief Get a displayable list of emulator and core available for the given game
     * @param game Game to retrieve emulators & cores from
     * @param outDefaultEmulator Filled with default emeulator
     * @param outDefaultCore Filled from default core
     * @param currentEmulator Current emulator from caller (to select the right entry)
     * @param currentCore Current core from caller (to select the right entry)
     * @return Emulator & core list
     */
    static EmulatorAndCoreList ListGameEmulatorAndCore(FileData& game, String& outDefaultEmulator,
                                                       String& outDefaultCore, const String& currentEmulator,
                                                       const String& currentCore);

    /*!
     * @brief Get a displayable list of emulator and core available for the given system
     * @param system System to retrieve emulators & cores from
     * @param outDefaultEmulator Filled with default emeulator
     * @param outDefaultCore Filled from default core
     * @param currentEmulator Current emulator from caller (to select the right entry)
     * @param currentCore Current core from caller (to select the right entry)
     * @return Emulator & core list
     */
    static EmulatorAndCoreList ListEmulatorAndCore(SystemData& system,
                                                   String& outDefaultEmulator, String& outDefaultCore,
                                                   const String& currentEmulator, const String& currentCore);

    /*!
     * @brief List shaders available in the user shader path
     * @return Shader list
     */
    static ShaderList ListShaders();

    /*!
     * @brief Adjust resolution text used in menu:
     * - Empty resolution returns "UNSET"
     * - "default" returns the native resolution
     * - old RPi resolutions are returned as XxY
     * - Any other string is return "as is"
     * @param resolution
     * @return
     */
    static String GetResolutionText(const String& resolution);

  private:
    static const Path sShadersPath;

    /*!
     * @brief Read shaders in folder recursively
     * @param root Current folder to read .glsl from
     * @param glslp Output list
     */
    static void ReadShaderFolder(const Path& root, Path::PathList& glslp);

    /*!
     * @brief Collect all abailable shaders
     * @return Shader path collection
     */
    static Path::PathList GetShaderList();
};



