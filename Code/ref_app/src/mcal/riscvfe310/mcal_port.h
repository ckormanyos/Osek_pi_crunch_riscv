///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2022 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_PORT_2022_12_16_H_
  #define MCAL_PORT_2022_12_16_H_

  #include <cstdint>

  #include <mcal_irq.h>
  #include <mcal_reg.h>

  namespace mcal
  {
    namespace port
    {
      using config_type = void;
      auto init(const config_type*) -> void;

      template<typename addr_type,
               typename reg_type,
               const addr_type port,
               const reg_type bpos>
      class port_pin
      {
      private:
        static constexpr auto input_val  = static_cast<addr_type>(port + static_cast<addr_type>(UINT32_C(0x00000000)));
        static constexpr auto input_en   = static_cast<addr_type>(port + static_cast<addr_type>(UINT32_C(0x00000004)));
        static constexpr auto output_en  = static_cast<addr_type>(port + static_cast<addr_type>(UINT32_C(0x00000008)));
        static constexpr auto output_val = static_cast<addr_type>(port + static_cast<addr_type>(UINT32_C(0x0000000C)));

      public:
        static void set_direction_output()
        {
          mcal::irq::disable_all();
          csr_clr_bits_mip(static_cast<std::uint32_t>(UINT32_C(0xFFFFFFFF)));
          mcal::reg::reg_access_static<addr_type, reg_type, output_en, bpos>::bit_set();
          mcal::irq::enable_all();
        }

        static void set_direction_input() { }

        static void set_pin_high()
        {
          mcal::irq::disable_all();
          csr_clr_bits_mip(static_cast<std::uint32_t>(UINT32_C(0xFFFFFFFF)));
          mcal::reg::reg_access_static<addr_type, reg_type, output_val, bpos>::bit_set();
          mcal::irq::enable_all();
        }

        static void set_pin_low()
        {
          mcal::irq::disable_all();
          csr_clr_bits_mip(static_cast<std::uint32_t>(UINT32_C(0xFFFFFFFF)));
          mcal::reg::reg_access_static<addr_type, reg_type, output_val, bpos>::bit_clr();
          mcal::irq::enable_all();
        }

        static bool read_input_value()
        {
          return false;
        }

        static void toggle_pin()
        {
          mcal::irq::disable_all();
          csr_clr_bits_mip(static_cast<std::uint32_t>(UINT32_C(0xFFFFFFFF)));
          mcal::reg::reg_access_static<addr_type, reg_type, output_val, bpos>::bit_not();
          mcal::irq::enable_all();
        }
      };
    }
  }

#endif // MCAL_PORT_2022_12_16_H_
