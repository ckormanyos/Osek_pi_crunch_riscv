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
    namespace communication_buffer_depth_one_byte
    {
      using buffer_value_type = std::uint8_t;
    } // namespace communication_buffer_depth_one_byte

    template<typename CommunicationEngineType>
    class communication final : private util::noncopyable
    {
    public:
      using communication_engine_type = CommunicationEngineType;

      static auto recv() -> communication_buffer_depth_one_byte::buffer_value_type { return my_com_engine.recv(); }

      static auto   select() -> void { communication_engine_type::select(); }
      static auto deselect() -> void { communication_engine_type::deselect(); }

      static auto send(const std::uint8_t byte_to_send) -> bool { return my_com_engine.send(byte_to_send); }

      template<typename SendIteratorType>
      static auto send_n(SendIteratorType first, SendIteratorType last) -> bool
      {
        auto result_send_is_ok = true;

        while(first != last)
        {
          result_send_is_ok = (my_com_engine.send(static_cast<std::uint8_t>(*first++)) && result_send_is_ok);
        }

        return result_send_is_ok;
      }

      static auto get() -> communication_engine_type& { return my_com_engine; }

    private:
      static communication_engine_type my_com_engine;
    };

    template<typename CommunicationEngineType>
    typename communication<CommunicationEngineType>::communication_engine_type communication<CommunicationEngineType>::my_com_engine;

    template<typename CommunicationEngineType>
    using communication_type = communication<CommunicationEngineType>;
  }

#endif // UTIL_COMMUNICATION_2012_05_31_H
