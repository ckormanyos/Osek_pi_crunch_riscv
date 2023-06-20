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

#include <array>
#include <cstddef>

#include <mcal_spi.h>
#include <mcal_spi_spi1.h>

extern "C"
{
  void FE310_SPI1_Init(void);

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
    QSPI1->sckdiv.bit.div  = 31u; // 3MHz
    QSPI1->sckmode.bit.pha = 0u;  // Clock phase = 0 ==> data sampled on rising edge and shifted out on the falling edge
    QSPI1->sckmode.bit.pol = 0u;  // Clock polarity = 0 ==> idle state of the clock is low
    QSPI1->csid            = 0u;  // SS0 is selected
    QSPI1->csmode.bit.mode = 2u;  // CS mode is HOLD
    QSPI1->fmt.bit.proto   = 0u;  // Mode is SPI
    QSPI1->fmt.bit.endian  = 0u;  // Transmit most-significant bit (MSB) first
    QSPI1->fmt.bit.dir     = 0u;  // FIFO is used
    QSPI1->fmt.bit.len     = 8u;  // 8 bits per frame
  }
}

namespace local
{
  using mcal_spi_type = mcal::spi::spi1;

  mcal_spi_type com0;

  constexpr auto mcal_spi_channel_count = static_cast<std::size_t>(UINT8_C(1));

  using communication_base_array_type =
    std::array<::util::communication_base*, mcal_spi_channel_count>;

  communication_base_array_type com_channel_pointers
  {
    &com0
  };
}

util::communication_base& mcal::spi::spi_channels()
{
  using local_com_multichannel_type =
    util::communication_multi_channel<std::tuple_size<local::communication_base_array_type>::value>;

  static local_com_multichannel_type com_channels { local::com_channel_pointers.data() };

  return com_channels;
}
