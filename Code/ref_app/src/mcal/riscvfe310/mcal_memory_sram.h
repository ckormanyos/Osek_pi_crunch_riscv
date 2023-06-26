///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MCAL_MEMORY_SRAM_2020_04_09_H
  #define MCAL_MEMORY_SRAM_2020_04_09_H

  #include <mcal_memory/mcal_memory_sram_generic_spi.h>
  #include <mcal_spi.h>

  namespace mcal { namespace memory { namespace sram {

  template<typename SramDeviceType>
  struct sram_device
  {
    using sram_device_type = SramDeviceType;

    static sram_device_type my_sram;
  };

  template<typename SramDeviceType>
  typename sram_device<SramDeviceType>::sram_device_type sram_device<SramDeviceType>::my_sram;

  using mcal_memory_sram_device_type =
    mcal_memory_sram_generic_spi<static_cast<mcal_sram_uintptr_t>(UINT32_C(0x00400000)),
                                 static_cast<mcal_sram_uintptr_t>(UINT32_C(0x00000400)),
                                 mcal::spi::communication_type>;

  inline auto mcal_memory_sram_device() -> mcal_memory_sram_device_type&
  {
    return sram_device<mcal_memory_sram_device_type>::my_sram;
  }

  } } } // namespace mcal::memory::sram

#endif // MCAL_MEMORY_SRAM_2020_04_09_H
