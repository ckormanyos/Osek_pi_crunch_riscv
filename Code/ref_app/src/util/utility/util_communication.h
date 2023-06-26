///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2007 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef UTIL_COMMUNICATION_2012_05_31_H
  #define UTIL_COMMUNICATION_2012_05_31_H

  #include <algorithm>
  #include <array>
  #include <cstddef>
  #include <cstdint>

  #include <util/utility/util_noncopyable.h>

  namespace util
  {
    template<typename CommunicationEngineType>
    class communication final : private util::noncopyable
    {
    public:
      using communication_engine_type = CommunicationEngineType;

      auto recv(std::uint8_t& byte_to_recv) -> bool { return my_com_engine.recv(byte_to_recv); }

      static auto   select() -> void { communication_engine_type::select(); }
      static auto deselect() -> void { communication_engine_type::deselect(); }

      template<typename SendIteratorType>
      auto send_n(SendIteratorType first, SendIteratorType last) -> bool { return my_com_engine.send_n(first, last); }

      auto send(const std::uint8_t byte_to_send) -> bool { return my_com_engine.send(byte_to_send); }

    protected:
      communication_engine_type my_com_engine { };
    };

    namespace communication_buffer_depth_one_byte
    {
      using buffer_value_type = std::uint8_t;
    } // namespace communication_buffer_depth_one_byte
  }

#endif // UTIL_COMMUNICATION_2012_05_31_H
