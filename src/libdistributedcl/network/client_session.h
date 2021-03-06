/*
 * Copyright (c) 2009-2012 Andr� Tupinamb� (andrelrt@gmail.com)
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
#ifndef _DCL_NETWORK_CLIENT_SESSION_H_
#define _DCL_NETWORK_CLIENT_SESSION_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <queue>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "distributedcl_internal.h"
#include "network/session.h"
#include "message/message.h"
#include "message/packet.h"
#include "message/msg_internal.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace client {
//-----------------------------------------------------------------------------
template< template< class > class COMM >
class client_session : public dcl::network::platform::session< COMM >
{
public:
    client_session( const typename dcl::network::platform::session< COMM >::config_info_t& config ) :
        dcl::network::platform::session< COMM >( config ), running_( true ), async_flush_queue_semaphore_( 0 )
    {
        dcl::network::platform::session< COMM >::get_communication().startup( this );
        async_flush_queue_thread_sp_.reset( 
            new boost::thread( &dcl::network::client::client_session<COMM>::async_flush_queue_thread, this ) );
    }

    ~client_session()
    {
        running_ = false;
        async_flush_queue_semaphore_.post();
        async_flush_queue_thread_sp_->join();

        dcl::network::platform::session< COMM >::get_communication().shutdown();
        clear_received_messages();
    }
    
    std::string get_connection_string()
    {
        return dcl::network::platform::session< COMM >::get_communication().get_config().connection_string_;
    }

    client_session<COMM>* create_child()
    {
        client_session<COMM>* child =
            new client_session<COMM>( dcl::network::platform::session< COMM >::get_communication().get_config() );

        childs_.push_back( child );

        child->connect();

        return child;
    }

    inline void connect()
    {
        dcl::network::platform::session< COMM >::get_communication().connect();

        //TODO: send handshake base_messages
        dcl::network::platform::session< COMM >::set_session_id( 1 );
        dcl::network::platform::session< COMM >::set_sequence_number( 200 );
        dcl::network::platform::session< COMM >::set_remote_sequence_number( 100 );
    }

    inline void close()
    {
        dcl::network::platform::session< COMM >::get_communication().shutdown();
    }

    inline bool queue_empty() const
    {
        return message_queue_.empty();
    }

    //inline void async_send_message( message_sp_t message_sp )
    //{
        //scoped_lock_t lock( queue_mutex_ );

        //message_queue_.push( message_sp );

        //send_semaphore_.post();
    //}

    inline void async_flush_queue()
    {
        async_flush_queue_semaphore_.post();
    }
    
    inline void flush_childs()
    {
        typename clients_t::iterator it;
        for( it = childs_.begin(); it != childs_.end(); ++it )
        {
            (*it)->flush_queue();
        }
    }

    inline void flush_queue()
    {
        scoped_lock_t lock( queue_mutex_ );

        if( message_queue_.empty() )
            return;

        flush();
    }

    inline void wait()
    {
        this->wait_response_.wait();
    }

    inline void enqueue_message( message_sp_t message_sp )
    {
        scoped_lock_t lock( queue_mutex_ );

        message_queue_.push( message_sp );
    }

    inline void send_message( message_sp_t message_sp )
    {
        scoped_lock_t lock( queue_mutex_ );

        message_queue_.push( message_sp );

        flush();
    }

    inline dcl::message_vector_t& get_received_messages()
    {
        return received_messages_;
    }

private:
    typedef std::queue< message_sp_t > message_queue_t;
    typedef std::map< uint16_t, message_sp_t > message_map_t;
    typedef std::vector< client_session<COMM>* > clients_t;

    bool running_;
    clients_t childs_;
    dcl::mutex_t queue_mutex_;
    message_queue_t message_queue_;
    message_map_t pending_messages_;
    dcl::message_vector_t received_messages_;
    boost::scoped_ptr<boost::thread> async_flush_queue_thread_sp_;
    boost::interprocess::interprocess_semaphore async_flush_queue_semaphore_;

    void flush()
    {
        try
        {
            send_request();
        }
        catch( dcl::library_exception& )
        {
            // Connection reset, reconnect
            // TODO: Reconnect
            return;
        }

        packet_sp_t recv_packet_sp;

        try
        {
            // Wait response
            recv_packet_sp = dcl::network::platform::session< COMM >::receive_packet();
        }
        catch( dcl::library_exception& )
        {
            // Connection reset, reconnect
            // TODO: Reconnect
            return;
        }

        // Fill internal received base_messages
        clear_received_messages();

        recv_packet_sp->parse( false );

        dcl::message_vector_t::iterator recv_message_it;
        dcl::message_vector_t& recv_messages = recv_packet_sp->get_messages();

        for( recv_message_it = recv_messages.begin(); recv_message_it != recv_messages.end(); ++recv_message_it )
        {
            if( (*recv_message_it)->get_type() == dcl::network::message::msg_error_message )
            {
                (*recv_message_it)->parse_response( (*recv_message_it)->get_payload() );

                dcl::network::message::dcl_message< dcl::network::message::msg_error_message >* error_message_ptr =
                    reinterpret_cast<dcl::network::message::dcl_message< dcl::network::message::msg_error_message >*>( (*recv_message_it).get() );

                if( error_message_ptr->get_error_code() != CL_SUCCESS )
                {
                    throw dcl::library_exception( "Received exception", error_message_ptr->get_error_code() );
                }

                received_messages_.push_back( *recv_message_it );
            }
            else
            {
                message_map_t::iterator it = pending_messages_.find( (*recv_message_it)->get_id() );

                if( it != pending_messages_.end() )
                {
                    it->second->parse_response( (*recv_message_it)->get_payload() );

                    pending_messages_.erase( it );
                }
            }
        }
    }

    inline void send_request()
    {
        packet_sp_t packet_sp( dcl::network::platform::session< COMM >::create_packet() );

        while( !message_queue_.empty() )
        {
            message_sp_t message_sp = message_queue_.front();

            //std::cerr << "inicio: " << message_sp.use_count();
            packet_sp->add( message_sp );

            // Saves the message object for response.
            // All other messages will be released by shared_ptr.
            if( message_sp->waiting_response() )
            {
                pending_messages_.insert( message_map_t::value_type( message_sp->get_id(), message_sp ) );
            }

            //std::cerr << " - meio: " << message_sp.use_count();

            message_queue_.pop();

            //std::cerr << " - fim: " << message_sp.use_count() << std::endl;
        }

        // Send data
        dcl::network::platform::session< COMM >::send_packet( packet_sp, true );
    }

    inline void clear_received_messages()
    {
        received_messages_.clear();
    }

    void async_flush_queue_thread()
    {
        while( 1 )
        {
            async_flush_queue_semaphore_.wait();

            if( !running_ )
                return;
            
            flush_queue();
        }
    }
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::client
//-----------------------------------------------------------------------------
#endif // _DCL_NETWORK_CLIENT_SESSION_H_
