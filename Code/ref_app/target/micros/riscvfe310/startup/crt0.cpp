///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

namespace crt
{
  auto init_ram  (void) -> void;
  auto init_ctors(void) -> void;
}

extern "C"
{
  void Startup_Init(void) __attribute__((naked, no_reorder, optimize(0), used, noinline));
  void Startup_RunApplication(void);
  void Startup_InitMcuSystem(void);
}

void Startup_Init(void)
{
  ::Startup_InitMcuSystem();

  crt::init_ram();
  crt::init_ctors();

  ::Startup_RunApplication();
}
