///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// ORIGINALLY FROM:

/******************************************************************************************
  Filename    : SPI1.h
  
  Core        : RV32IMAC
  
  MCU         : FE310-G002 (SiFive)

  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 12.06.2023
  
  Description : SPI1 driver header file
  
******************************************************************************************/

#ifndef MCAL_SPI_2023_06_18_H
#define MCAL_SPI_2023_06_18_H

#include <cstdint>

#include <util/utility/util_communication.h>

class spi1 : public ::util::communication_buffer_depth_one_byte
{
private:
  using base_class_type = util::communication_buffer_depth_one_byte;

public:
  spi1();

  ~spi1() override = default;

  auto send(const std::uint8_t byte_to_send) -> bool override;

  auto select() -> void override;

  auto deselect() -> void override;

private:
  static std::uint32_t SavedIntLevel;
};

#endif // MCAL_SPI_2023_06_18_H
