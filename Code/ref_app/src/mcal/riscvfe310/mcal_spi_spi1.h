///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Some OF THE FE310 SPI1 codes are Originally From:

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

  #include <FE310.h>
  #include <mcal_irq.h>

  #include <util/utility/util_communication.h>
  #include <util/utility/util_noncopyable.h>

  namespace mcal { namespace spi {

  class spi1 : private ::util::noncopyable
  {
  public:
    spi1() = delete;

    ~spi1() = delete;

    static auto backend_send(const std::uint8_t byte_to_send, std::uint8_t* p_byte_to_receive) -> bool
    {
      // Fill the Tx FIFO.
      QSPI1->txdata.bit.data = byte_to_send;

      // Wait for the Rx FIFO to be full.
      while(QSPI1->ip.bit.rxwm == static_cast<std::uint32_t>(UINT8_C(0))) { ; }

      // Read the (single byte from the) RX FIFO.
      *p_byte_to_receive = static_cast<std::uint8_t>(QSPI1->rxdata.bit.data);

      return true;
    }

    static auto backend_select() -> void
    {
      mcal::irq::disable_all();

      // Set CS control mode (HOLD).
      QSPI1->csmode.bit.mode = static_cast<std::uint32_t>(UINT8_C(2));
    }

    static auto backend_deselect() -> void
    {
      // Set CS control mode (OFF).
      QSPI1->csmode.bit.mode = static_cast<std::uint32_t>(UINT8_C(3));

      mcal::irq::enable_all();
    }

    static void backend_init()
    {
      /* Configure the programmed IO pins */
      GPIO0->iof_sel.bit.pin2 = static_cast<std::uint32_t>(UINT8_C(0)); // SPI1_SS0
      GPIO0->iof_sel.bit.pin3 = static_cast<std::uint32_t>(UINT8_C(0)); // SPI1_MOSI
      GPIO0->iof_sel.bit.pin4 = static_cast<std::uint32_t>(UINT8_C(0)); // SPI1_MISO
      GPIO0->iof_sel.bit.pin5 = static_cast<std::uint32_t>(UINT8_C(0)); // SPI1_SCK

      /* Enable the programmed IO pins */
      GPIO0->iof_en.bit.pin2 =  static_cast<std::uint32_t>(UINT8_C(1)); // SPI1_SS0
      GPIO0->iof_en.bit.pin3 =  static_cast<std::uint32_t>(UINT8_C(1)); // SPI1_MOSI
      GPIO0->iof_en.bit.pin4 =  static_cast<std::uint32_t>(UINT8_C(1)); // SPI1_MISO
      GPIO0->iof_en.bit.pin5 =  static_cast<std::uint32_t>(UINT8_C(1)); // SPI1_SCK

      /* Configure the SPI controller */
      QSPI1->sckdiv.bit.div  =  static_cast<std::uint32_t>(UINT8_C(11)); // 8MHz, where [(96 - 1) -> 1MHz]
      QSPI1->sckmode.bit.pha =  static_cast<std::uint32_t>(UINT8_C(0));  // Clock phase = 0 ==> data sampled on rising edge and shifted out on the falling edge
      QSPI1->sckmode.bit.pol =  static_cast<std::uint32_t>(UINT8_C(0));  // Clock polarity = 0 ==> idle state of the clock is low
      QSPI1->csid            =  static_cast<std::uint32_t>(UINT8_C(0));  // SS0 is selected
      QSPI1->csmode.bit.mode =  static_cast<std::uint32_t>(UINT8_C(2));  // CS mode is HOLD
      QSPI1->fmt.bit.proto   =  static_cast<std::uint32_t>(UINT8_C(0));  // Mode is SPI
      QSPI1->fmt.bit.endian  =  static_cast<std::uint32_t>(UINT8_C(0));  // Transmit most-significant bit (MSB) first
      QSPI1->fmt.bit.dir     =  static_cast<std::uint32_t>(UINT8_C(0));  // FIFO is used
      QSPI1->fmt.bit.len     =  static_cast<std::uint32_t>(UINT8_C(8));  // 8 bits per frame

      // Set the Rx watermark to (1 - 1 = 0), since we will restrict
      // the driver to sending/receiving 1 byte only per transfer.
      QSPI1->rxmark.bit.rxmark = static_cast<std::uint32_t>(UINT8_C(0));
    }
  };

  } } // namespace mcal::spi

#endif // MCAL_SPI_SPI1_2023_06_20_H
