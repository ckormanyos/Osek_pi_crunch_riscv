///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_IRQ_2010_04_10_H
  #define MCAL_IRQ_2010_04_10_H

  #include <cstdint>

  #include "riscv-csr.h"

  namespace mcal
  {
    namespace irq
    {
      extern std::uint32_t saved_int_state;

      inline void enable_all () { ::csr_write_mstatus(saved_int_state); }
      inline void disable_all() { saved_int_state = ::csr_read_clr_bits_mstatus(MSTATUS_MIE_BIT_MASK); }
    }
  }

#endif // MCAL_IRQ_2010_04_10_H
