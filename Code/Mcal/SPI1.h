/******************************************************************************************
  Filename    : SPI1.h
  
  Core        : RV32IMAC
  
  MCU         : FE310-G002 (SiFive)
    
  Author      : Chalandi Amine
 
  Owner       : Chalandi Amine
  
  Date        : 12.06.2023
  
  Description : SPI1 driver header file
  
******************************************************************************************/

#ifndef __SPI1_H__
#define __SPI1_H__

#include "Platform_Types.h"

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
  static uint32 SavedIntLevel;
};

#endif /* __SPI1_H__ */
