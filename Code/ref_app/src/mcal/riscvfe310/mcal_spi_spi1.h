///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Some of the FE310 SPI1 codes are Originally From:

/******************************************************************************************
  Filename    : SPI1.c
  
  Core        : RV32IMAC
  
  MCU         : FE310-G002 (SiFive)
  
  Author      : Chalandi Amine
  
  Owner       : Chalandi Amine
  
  Date        : 12.06.2023
  
  Description : SPI1 driver implementation
  
******************************************************************************************/

#ifndef MCAL_SPI_SPI1_2023_06_20_H
  #define MCAL_SPI_SPI1_2023_06_20_H

  #include <cstdint>

  #include "FE310.h"

  #include <mcal_irq.h>

  #include <util/utility/util_communication.h>

  extern "C"
  {
    void FE310_SPI1_Init(void);
  }

  namespace mcal { namespace spi {

  class spi1 : public ::util::communication_buffer_depth_one_byte
  {
  private:
    using base_class_type = util::communication_buffer_depth_one_byte;

  public:
    spi1()
    {
      ::FE310_SPI1_Init();

      // Set the Rx watermark to (1 - 1 = 0), since we will restrict
      // the driver to sending/receiving 1 byte only per transfer.
      QSPI1->rxmark.bit.rxmark = static_cast<std::uint32_t>(UINT8_C(0));
    }

    ~spi1() override = default;

    auto send(const std::uint8_t byte_to_send) -> bool override
    {
      // Fill the Tx FIFO.
      QSPI1->txdata.bit.data = byte_to_send;

      // Wait for the Rx FIFO to be full.
      while(!QSPI1->ip.bit.rxwm);

      // Read the (single byte from the) RX FIFO.
      base_class_type::recv_buffer = QSPI1->rxdata.bit.data;

      return true;
    }

    auto select() -> void override
    {
      mcal::irq::disable_all();

      // Set CS control mode (HOLD).
      QSPI1->csmode.bit.mode = static_cast<std::uint32_t>(UINT8_C(2));
    }

    auto deselect() -> void override
    {
      // Set CS control mode (OFF).
      QSPI1->csmode.bit.mode = static_cast<std::uint32_t>(UINT8_C(3));

      mcal::irq::enable_all();
    }
  };

  } } // namespace mcal::spi

#endif // MCAL_SPI_SPI1_2023_06_20_H
