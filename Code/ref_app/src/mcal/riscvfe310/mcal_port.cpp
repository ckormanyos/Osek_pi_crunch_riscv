///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mcal_benchmark.h>
#include <mcal_port.h>
#include <mcal_reg.h>

void mcal::port::init(const config_type*)
{
  benchmark::benchmark_port_type::set_direction_output();
}
