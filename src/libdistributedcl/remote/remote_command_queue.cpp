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
#include "remote_command_queue.h"
#include "message/message.h"
#include "message/msg_command_queue.h"
using dcl::network::message::dcl_message;
using dcl::network::message::base_message;
using dcl::network::message::msgFlush;
using dcl::network::message::msgFinish;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
void remote_command_queue::flush() const
{
    get_queue_session().async_flush_queue();

    //dcl_message< msgFlush >* msg_ptr = new dcl_message< msgFlush >();

    //msg_ptr->set_remote_id( get_remote_id() );

    //message_sp_t message_sp( msg_ptr );
    //get_queue_session().send_message( message_sp );
}
//-----------------------------------------------------------------------------
void remote_command_queue::finish() const
{
    dcl_message< msgFinish >* msg_ptr = new dcl_message< msgFinish >();

    msg_ptr->set_remote_id( get_remote_id() );

    message_sp_t message_sp( msg_ptr );
    get_queue_session().send_message( message_sp );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
