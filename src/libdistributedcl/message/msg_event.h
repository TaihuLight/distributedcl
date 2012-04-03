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
#ifndef _DCL_EVENT_MESSAGES_H_
#define _DCL_EVENT_MESSAGES_H_

#include "message.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
    //msgWaitForEvents            = 62,
    //msgGetEventInfo             = 63,
    //msgRetainEvent              = 64,
    //msgReleaseEvent             = 65,
    //msgGetEventProfilingInfo    = 66,
//-----------------------------------------------------------------------------
// msgWaitForEvents
//-----------------------------------------------------------------------------
template<>
class dcl_message< msgWaitForEvents > : public base_message
{
public:
    dcl_message< msgWaitForEvents >() : 
        base_message( msgWaitForEvents, true, sizeof( dcl::remote_id_t ), 0 ),
        id_( 0xffff ){}

    // Request
    MSG_PARAMETER_GET_SET( dcl::remote_id_t, id_, event_id )

private:
    dcl::remote_id_t id_;

    virtual void create_request( void* payload_ptr );
    virtual void parse_request( const void* payload_ptr );
};
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
#endif // _DCL_EVENT_MESSAGES_H_