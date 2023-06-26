///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2012 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_SPI_2012_05_24_H_
  #define MCAL_SPI_2012_05_24_H_

  #include <mcal_spi_spi1.h>

  namespace mcal { namespace spi {

  using config_type = void;

  inline auto init(const config_type*) -> void { }

  using communication_type = ::util::communication<::mcal::spi::spi1>;

  auto spi_channels() -> communication_type&;

  } }

#endif // MCAL_SPI_2012_05_24_H_
