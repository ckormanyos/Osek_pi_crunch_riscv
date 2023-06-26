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
    template<typename CommunicationBackendType>
    class communication final : private util::noncopyable
    {
    public:
      using communication_backend_type = CommunicationBackendType;

      static auto init() -> void { communication_backend_type::backend_init(); }

      static auto recv() -> std::uint8_t { return my_recv_buffer; }

      static auto   select() -> void { communication_backend_type::backend_select(); }
      static auto deselect() -> void { communication_backend_type::backend_deselect(); }

      static auto send(const std::uint8_t byte_to_send) -> bool
      {
        return communication_backend_type::backend_send(byte_to_send, &my_recv_buffer);
      }

      template<typename SendIteratorType>
      static auto send_n(SendIteratorType first, SendIteratorType last) -> bool
      {
        auto result_send_is_ok = true;

        while(first != last)
        {
          result_send_is_ok =
          (
               communication_backend_type::backend_send(static_cast<std::uint8_t>(*first++), &my_recv_buffer)
            && result_send_is_ok
          );
        }

        return result_send_is_ok;
      }

    private:
      static std::uint8_t my_recv_buffer;
    };

    template<typename CommunicationBackendType>
    std::uint8_t communication<CommunicationBackendType>::my_recv_buffer;
  }

#endif // UTIL_COMMUNICATION_2012_05_31_H
