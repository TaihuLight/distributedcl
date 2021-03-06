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
#ifndef _DCL_REMOTE_COMMAND_QUEUE_H_
#define _DCL_REMOTE_COMMAND_QUEUE_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
#include "remote_object.h"
#include "remote_context.h"
#include "info/command_queue_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_command_queue :
    public dcl::info::generic_command_queue,
    public remote_object< remote_command_queue, dcl::network::message::msgReleaseCommandQueue >
{
public:
    remote_command_queue( dcl::network::client::session_manager::session_t& session_ref,
                          const remote_context* context_ptr, const remote_device* device_ptr, 
                          cl_command_queue_properties properties ) :
        dcl::info::generic_command_queue( reinterpret_cast<const dcl::info::generic_context*>( context_ptr ), 
                                          reinterpret_cast<const dcl::info::generic_device*>( device_ptr ), 
                                          properties ),
        remote_object< remote_command_queue, dcl::network::message::msgReleaseCommandQueue >( context_ptr->get_session() ),
        queue_session_ref_( session_ref ){}

    ~remote_command_queue()
    {
        queue_session_ref_.close();
    }

    virtual void flush() const;
    virtual void finish() const;

    inline dcl::network::client::session_manager::session_t& get_queue_session() const
    {
        return queue_session_ref_;
    }

private:
    dcl::network::client::session_manager::session_t& queue_session_ref_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_COMMAND_QUEUE_H_
