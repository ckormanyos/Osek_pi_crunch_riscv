///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2012 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_spi.h>
#include <mcal_spi_spi1.h>

auto mcal::spi::spi_channels() -> communication_type&
{
  static communication_type com_spi1;

  return com_spi1;
}
