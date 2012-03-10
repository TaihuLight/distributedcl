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
#ifndef _DCL_SERVER_PLATFORM_H_
#define _DCL_SERVER_PLATFORM_H_

#include "distributedcl_internal.h"
#include "server_command.h"
#include "info/object_manager.h"
#include "message/message.h"
#include "composite/composite_device.h"
#include "composite/composite_context.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
class GetDeviceIDs_command : 
    public server_command< dcl::network::message::msgGetDeviceIDs >
{
public:
    GetDeviceIDs_command( recv_ptr_t message_ptr ) : 
        server_command< dcl::network::message::msgGetDeviceIDs >( message_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class GetDeviceInfo_command : 
    public server_command< dcl::network::message::msgGetDeviceInfo >
{
public:
    GetDeviceInfo_command( recv_ptr_t message_ptr ) : 
        server_command< dcl::network::message::msgGetDeviceInfo >( message_ptr ) {}

    void execute();
};
//-----------------------------------------------------------------------------
class server_platform
{
private:
    typedef dcl::info::object_manager< dcl::composite::composite_device > device_manager_t;
    typedef dcl::info::object_manager< dcl::composite::composite_context > context_manager_t;

    device_manager_t device_manager_;
    context_manager_t context_manager_;
    static server_platform instance_;

    server_platform(){}

public:
    static server_platform& get_instance()
    {
        return instance_;
    }

    inline device_manager_t& get_device_manager()
    {
        return device_manager_;
    }

    inline context_manager_t& get_context_manager()
    {
        return context_manager_;
    }
};
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------
#endif // _DCL_SERVER_PLATFORM_H_