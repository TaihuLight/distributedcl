/*
 * Copyright (c) 2009-2011 Andr� Tupinamb� (andrelrt@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
//-----------------------------------------------------------------------------
#ifndef _DCL_NETWORK_MESSAGE_PACKET_H_
#define _DCL_NETWORK_MESSAGE_PACKET_H_

#include <vector>
#include "distributedcl_internal.h"
#include "network/platform.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
class base_message;
typedef std::vector< base_message* > message_vector_t;
//-----------------------------------------------------------------------------
class packet
{
public:
    inline packet( uint8_t* buffer_ptr, std::size_t buffer_size ) :
        buffer_size_( buffer_size ), buffer_ptr_( buffer_ptr )
    {
        header_ptr_ = reinterpret_cast< packet_header* >( buffer_ptr );
    }

    inline packet( uint8_t* buffer_ptr, std::size_t buffer_size, 
                   dcl::network::platform::session_id_t session_id, 
                   dcl::network::platform::session_sequence_t sequence ) :
        buffer_size_( buffer_size ), buffer_ptr_( buffer_ptr )
    {
        header_ptr_ = reinterpret_cast< packet_header* >( buffer_ptr );

        length_ = sizeof( packet_header );

        header_ptr_->version = packet_v1_0;
        header_ptr_->message_count = 0;
        header_ptr_->session_id = session_id;
        header_ptr_->sequence_number = sequence;
    }

    inline ~packet(){}

    void parse();
    void parse_messages();

    inline uint8_t get_message_count() const
    {
        return header_ptr_->message_count;
    }

    inline uint16_t get_length() const
    {
        return static_cast< uint16_t >( length_ );
    }

    inline uint16_t get_session_id() const
    {
        return header_ptr_->session_id;
    }

    inline uint16_t get_sequence_number() const
    {
        return header_ptr_->sequence_number;
    }

    inline message_vector_t& get_messages()
    {
        return messages_;
    }

    inline uint8_t* get_buffer()
    {
        return buffer_ptr_;
    }

    inline std::size_t get_buffer_size()
    {
        return buffer_size_;
    }

    static inline std::size_t get_header_size()
    {
        return sizeof( packet_header );
    }

    static inline std::size_t get_packet_size( uint8_t* buffer )
    {
        packet_header* header_ptr = reinterpret_cast< packet_header* >( buffer );

        return network_to_host( header_ptr->length );
    }

    void add( base_message* base_message_ptr );
    void create_packet();

private:
    enum{ packet_v1_0 = 0x10 };

    #pragma pack( push, 1 )

    // Better when aligned in 32 bits boundary
    struct packet_header
    {
        uint8_t version;
        uint8_t message_count;
        uint16_t length;
        dcl::network::platform::session_id_t session_id;
        dcl::network::platform::session_sequence_t sequence_number;
    };
    #pragma pack( pop )

    std::size_t length_;
    std::size_t buffer_size_;
    packet_header* header_ptr_;
    uint8_t* buffer_ptr_;
    message_vector_t messages_;
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_MESSAGE_PACKET_H_