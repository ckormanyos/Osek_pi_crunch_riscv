///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2012 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Some FE310 SPI1 codes are Originally From:

/******************************************************************************************
  Filename    : SPI1.c
  
  Core        : RV32IMAC
  
  MCU         : FE310-G002 (SiFive)
  
  Author      : Chalandi Amine
  
  Owner       : Chalandi Amine
  
  Date        : 12.06.2023
  
  Description : SPI1 driver implementation
  
******************************************************************************************/

#include <cstddef>
#include <cstdint>

#include "FE310.h"
#include "riscv-csr.h"

#include <mcal_irq.h>
#include <mcal_spi.h>

#include <util/utility/util_communication.h>

namespace local
{
  constexpr auto mcal_spi_channel_count = static_cast<std::size_t>(UINT8_C(1));

  void FE310_SPI1_Init(void)
  {
    /* Configure the programmed IO pins */
    GPIO0->iof_sel.bit.pin2 = 0u; // SPI1_SS0
    GPIO0->iof_sel.bit.pin3 = 0u; // SPI1_MOSI
    GPIO0->iof_sel.bit.pin4 = 0u; // SPI1_MISO
    GPIO0->iof_sel.bit.pin5 = 0u; // SPI1_SCK

    /* Enable the programmed IO pins */
    GPIO0->iof_en.bit.pin2 = 1u; // SPI1_SS0
    GPIO0->iof_en.bit.pin3 = 1u; // SPI1_MOSI
    GPIO0->iof_en.bit.pin4 = 1u; // SPI1_MISO
    GPIO0->iof_en.bit.pin5 = 1u; // SPI1_SCK

    /* Configure the SPI controller */
    QSPI1->sckdiv.bit.div  = 95u; // 1MHz
    QSPI1->sckmode.bit.pha = 0u;  // Clock phase = 0 ==> data sampled on rising edge and shifted out on the falling edge
    QSPI1->sckmode.bit.pol = 0u;  // Clock polarity = 0 ==> idle state of the clock is low
    QSPI1->csid            = 0u;  // SS0 is selected
    QSPI1->csmode.bit.mode = 2u;  // CS mode is HOLD
    QSPI1->fmt.bit.proto   = 0u;  // Mode is SPI
    QSPI1->fmt.bit.endian  = 0u;  // Transmit most-significant bit (MSB) first
    QSPI1->fmt.bit.dir     = 0u;  // FIFO is used
    QSPI1->fmt.bit.len     = 8u;  // 8 bits per frame
  }
} // namespace local;

std::uint32_t mcal_spi_saved_int_level;

class spi1 : public ::util::communication_buffer_depth_one_byte
{
private:
  using base_class_type = util::communication_buffer_depth_one_byte;

public:
  spi1()
  {
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
    mcal_spi_saved_int_level = csr_read_clr_bits_mstatus(MSTATUS_MIE_BIT_MASK);

    // Set CS control mode (HOLD).
    QSPI1->csmode.bit.mode = static_cast<std::uint32_t>(UINT8_C(2));
  }

  auto deselect() -> void override
  {
    // Set CS control mode (OFF).
    QSPI1->csmode.bit.mode = static_cast<std::uint32_t>(UINT8_C(3));

    csr_write_mstatus(mcal_spi_saved_int_level);
  }
};

void mcal::spi::init(const config_type*)
{
  local::FE310_SPI1_Init();
}

util::communication_base& mcal::spi::spi_channels()
{
  using mcal_spi0_type = spi1;

  static mcal_spi0_type com0;

  static util::communication_base* com_channel_pointers[local::mcal_spi_channel_count] =
  {
    &com0
  };

  static util::communication_multi_channel<local::mcal_spi_channel_count> com_channels(com_channel_pointers);

  return com_channels;
}
