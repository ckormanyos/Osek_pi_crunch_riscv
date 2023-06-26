///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2012 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_SPI_2012_05_24_H
  #define MCAL_SPI_2012_05_24_H

  #include <mcal_spi_spi1.h>

  namespace mcal { namespace spi {

  using config_type = void;

  inline auto init(const config_type*) -> void { }

  template<typename CommunicationType>
  struct communication final
  {
    using communication_type = CommunicationType;

    static auto get() -> communication_type&
    {
      static communication_type my_spi;

      return my_spi;
    }
  };

  using spi_channels_type = communication<::util::communication<::mcal::spi::spi1>>;

  using communication_type = typename spi_channels_type::communication_type;

  inline auto spi_channels() -> communication_type& { return spi_channels_type::get(); }

  } }

#endif // MCAL_SPI_2012_05_24_H
