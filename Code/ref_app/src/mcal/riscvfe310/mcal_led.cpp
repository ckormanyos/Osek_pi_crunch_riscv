///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_benchmark.h>
#include <mcal_irq.h>
#include <mcal_led.h>
#include <mcal_led/mcal_led_port.h>
#include <mcal_port.h>

extern "C"
{
  void mcal_led_toggle(void);

  void mcal_led_toggle(void)
  {
    mcal::led::led0().toggle();
  }
}

mcal::led::led_base& mcal::led::led0()
{
  using led0_port_type = mcal::port::port_pin<std::uint32_t,
                                              std::uint32_t,
                                              mcal::reg::gpio0_base,
                                              static_cast<std::uint32_t>(UINT8_C(5))>;

  using led0_led_type = mcal::led::led_port<led0_port_type>;

  static led0_led_type local_led0;

  return local_led0;
}
