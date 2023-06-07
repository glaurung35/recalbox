//
// Created by bkg2k on 07/06/23.
//
#pragma once

#include <vector>
#include <utils/String.h>
#include <systems/ArcadeDatabase.h>

class IArcadeGamelistInterface
{
  public:
    //! Destructor
    virtual ~IArcadeGamelistInterface() {}

    /*!
     * @brief Check if the Arcade game list has a valid database for the current emulator/core
     * @return
     */
    [[nodiscard]] virtual bool HasValidDatabase() const = 0;

    /*!
     * @brief Get driver list (arcade view only)
     * @return Driver list
     */
    [[nodiscard]] virtual std::vector<ArcadeDatabase::Driver> GetDriverList() const = 0;

    /*!
     * @brief Get the current emulator name for the current folder
     * @return Emulator name
     */
    [[nodiscard]] virtual String GetCurrentEmulatorName() const = 0;

    /*!
     * @brief Get the current core name for the current folder
     * @return Core name
     */
    [[nodiscard]] virtual String GetCurrentCoreName() const = 0;

    /*!
     * @brief Get the number of games attached to the given driver, in the current gamalist
     * @param driverIndex Driver index
     * @return Game count
     */
    [[nodiscard]] virtual int GetGameCountForDriver(int driverIndex) const = 0;
};