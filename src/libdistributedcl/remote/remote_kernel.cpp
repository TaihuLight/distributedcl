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
#include "remote_kernel.h"
#include "remote_command_queue.h"
#include "message/msg_kernel.h"
using dcl::network::message::dcl_message;
using dcl::network::message::base_message;
using dcl::network::message::msgEnqueueNDRangeKernel;
using dcl::network::message::msgSetKernelArg;
using dcl::info::ndrange;
using dcl::info::generic_kernel;
using dcl::info::generic_memory;
using dcl::info::generic_command_queue;
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
void remote_kernel::execute( const generic_command_queue* queue_ptr, 
                             const ndrange& offset, const ndrange& global, 
                             const ndrange& local )
{
    dcl_message< msgEnqueueNDRangeKernel >* msg_ptr = new dcl_message< msgEnqueueNDRangeKernel >();

    msg_ptr->set_command_queue_id( reinterpret_cast<const remote_command_queue*>( queue_ptr )->get_remote_id() );
    msg_ptr->set_kernel_id( get_remote_id() );

    msg_ptr->get_offset().copy( offset );
    msg_ptr->get_global().copy( global );
    msg_ptr->get_local().copy( local );

    session_ref_.send_message( reinterpret_cast< base_message* >( msg_ptr ) );
}
//-----------------------------------------------------------------------------
void remote_kernel::set_argument( uint32_t arg_index, const generic_memory* memory_ptr )
{
    dcl_message< msgSetKernelArg >* msg_ptr = new dcl_message< msgSetKernelArg >();

    msg_ptr->set_index( arg_index );
    msg_ptr->set_kernel_id( get_remote_id() );
    msg_ptr->set_memory_id( reinterpret_cast<const remote_memory*>( memory_ptr )->get_remote_id() );

    session_ref_.enqueue_message( reinterpret_cast< base_message* >( msg_ptr ) );
}
//-----------------------------------------------------------------------------
void remote_kernel::set_argument( uint32_t arg_index, size_t arg_size, const void* arg_value )
{
    dcl_message< msgSetKernelArg >* msg_ptr = new dcl_message< msgSetKernelArg >();

    msg_ptr->set_index( arg_index );
    msg_ptr->set_buffer( arg_value, arg_size );
    msg_ptr->set_kernel_id( get_remote_id() );

    session_ref_.enqueue_message( reinterpret_cast< base_message* >( msg_ptr ) );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
