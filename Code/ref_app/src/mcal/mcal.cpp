///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal/mcal.h>

extern "C"
auto mcal_init(void) -> void;

auto mcal_init(void) -> void
{
  ::mcal::init();
}

extern "C"
void FE310_SPI1_Init(void);

void mcal::init()
{
  // Initialize the microcontroller abstraction layer.

  mcal::port::init(nullptr);

  ::FE310_SPI1_Init();
}
