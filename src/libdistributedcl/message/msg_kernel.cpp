/*
 * Copyright (c) 2009-2012 André Tupinambá (andrelrt@gmail.com)
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
#include <string.h>
#include "msg_kernel.h"
using dcl::remote_id_t;
using dcl::info::ndrange;
//-----------------------------------------------------------------------------
namespace dcl {
namespace network {
namespace message {
//-----------------------------------------------------------------------------
// msgCreateKernel
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::create_request( void* payload_ptr )
{
    msgCreateKernel_request* request_ptr =
        reinterpret_cast< msgCreateKernel_request* >( payload_ptr );

    request_ptr->program_id_ = host_to_network( program_id_ );
    request_ptr->name_len_ = host_to_network( static_cast<uint32_t>( name_.length() ) );

    memcpy( request_ptr->name_, name_.data(), name_.length() );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::parse_request( const void* payload_ptr )
{
    const msgCreateKernel_request* request_ptr =
        reinterpret_cast< const msgCreateKernel_request* >( payload_ptr );

    program_id_ = network_to_host( request_ptr->program_id_ );

    name_.assign( reinterpret_cast<const char*>( request_ptr->name_ ), 
                  network_to_host( request_ptr->name_len_ ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::create_response( void* payload_ptr )
{
    remote_id_t* response_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    *response_ptr = host_to_network( remote_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgCreateKernel >::parse_response( const void* payload_ptr )
{
    const remote_id_t* response_ptr =
        reinterpret_cast< const remote_id_t* >( payload_ptr );

    remote_id_ = network_to_host( *response_ptr );
}
//-----------------------------------------------------------------------------
// msgEnqueueNDRangeKernel
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueNDRangeKernel >::create_request( void* payload_ptr )
{
    void* enqueue_ptr = enqueue_message::create_enqueue_request( payload_ptr );

    msgEnqueueNDRangeKernel_request* request_ptr =
        reinterpret_cast< msgEnqueueNDRangeKernel_request* >( enqueue_ptr );

    request_ptr->kernel_id_ = host_to_network( kernel_id_ );
    request_ptr->offset_dimensions_ = offset_.get_dimensions();
    request_ptr->global_dimensions_ = global_.get_dimensions();
    request_ptr->local_dimensions_ = local_.get_dimensions();

    for( size_t i = 0; i < global_.get_dimensions(); i++ )
    {
        request_ptr->offset_[ i ] = host_to_network( static_cast<uint32_t>( offset_.get_range()[ i ] ) );
        request_ptr->global_[ i ] = host_to_network( static_cast<uint32_t>( global_.get_range()[ i ] ) );
        request_ptr->local_[ i ]  = host_to_network( static_cast<uint32_t>( local_.get_range()[ i ] ) );
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgEnqueueNDRangeKernel >::parse_request( const void* payload_ptr )
{
    const void* enqueue_ptr = enqueue_message::parse_enqueue_request( payload_ptr );

    const msgEnqueueNDRangeKernel_request* request_ptr =
        reinterpret_cast< const msgEnqueueNDRangeKernel_request* >( enqueue_ptr );

    kernel_id_ = network_to_host( request_ptr->kernel_id_ );
    uint32_t offset_dimensions = request_ptr->offset_dimensions_;
    uint32_t global_dimensions = request_ptr->global_dimensions_;
    uint32_t local_dimensions = request_ptr->local_dimensions_;
    size_t value[ 3 ];

    value[ 0 ] = network_to_host( request_ptr->offset_[ 0 ] );
    value[ 1 ] = network_to_host( request_ptr->offset_[ 1 ] );
    value[ 2 ] = network_to_host( request_ptr->offset_[ 2 ] );
    offset_.copy( ndrange( offset_dimensions, value ) );

    value[ 0 ] = network_to_host( request_ptr->global_[ 0 ] );
    value[ 1 ] = network_to_host( request_ptr->global_[ 1 ] );
    value[ 2 ] = network_to_host( request_ptr->global_[ 2 ] );
    global_.copy( ndrange( global_dimensions, value ) );

    value[ 0 ] = network_to_host( request_ptr->local_[ 0 ] );
    value[ 1 ] = network_to_host( request_ptr->local_[ 1 ] );
    value[ 2 ] = network_to_host( request_ptr->local_[ 2 ] );
    local_.copy( ndrange( local_dimensions, value ) );
}
//-----------------------------------------------------------------------------
// msgSetKernelArg
//-----------------------------------------------------------------------------
void dcl_message< msgSetKernelArg >::create_request( void* payload_ptr )
{
    void* enqueue_ptr = enqueue_message::create_enqueue_request( payload_ptr );

    if( is_object() )
    {
        msgSetKernelArg_memory_request* request_ptr =
            reinterpret_cast< msgSetKernelArg_memory_request* >( enqueue_ptr );

        request_ptr->argument_type_ = argument_type_;
        request_ptr->index_ = index_;
        request_ptr->kernel_id_ = host_to_network( kernel_id_ );
        request_ptr->argmuent_id_ = host_to_network( argument_id_ );
        //std::cerr << "set_kernel_arg - ids: " << std::hex << kernel_id_ << "," << argument_id_ << std::endl;
    }
    else
    {
        msgSetKernelArg_buffer_request* request_ptr =
            reinterpret_cast< msgSetKernelArg_buffer_request* >( enqueue_ptr );

        request_ptr->argument_type_ = unknow_type;
        request_ptr->is_null_ = is_null_? 1 : 0;
        request_ptr->index_ = index_;
        request_ptr->kernel_id_ = host_to_network( kernel_id_ );
        request_ptr->size_ = host_to_network( size_ );
        //std::cerr << "set_kernel_arg - id: " << std::hex << kernel_id_ << std::endl;

        if( !is_null_ )
        {
            memcpy( request_ptr->buffer_, buffer_.data(), buffer_.size() );
        }
    }
}
//-----------------------------------------------------------------------------
void dcl_message< msgSetKernelArg >::parse_request( const void* payload_ptr )
{
    const void* enqueue_ptr = enqueue_message::parse_enqueue_request( payload_ptr );

    const msgSetKernelArg_memory_request* request_ptr =
        reinterpret_cast< const msgSetKernelArg_memory_request* >( enqueue_ptr );

    argument_type_ = static_cast<argument_type_t>( request_ptr->argument_type_ );

    if( is_object() )
    {
        index_ = request_ptr->index_;
        kernel_id_ = network_to_host( request_ptr->kernel_id_ );
        argument_id_ = network_to_host( request_ptr->argmuent_id_ );
        //std::cerr << "set_kernel_arg - ids: " << std::hex << kernel_id_ << "," << argument_id_ << std::endl;
    }
    else
    {
        const msgSetKernelArg_buffer_request* req_ptr =
            reinterpret_cast< const msgSetKernelArg_buffer_request* >( enqueue_ptr );

        argument_type_ = unknow_type;

        size_ = req_ptr->size_;
        index_ = network_to_host( req_ptr->index_ );
        kernel_id_ = network_to_host( req_ptr->kernel_id_ );
        is_null_ = req_ptr->is_null_? true : false;

        if( !is_null_ )
        {
            buffer_.assign( req_ptr->buffer_, req_ptr->buffer_ + network_to_host( req_ptr->size_ ) );
        }
        //std::cerr << "set_kernel_arg - id: " << std::hex << kernel_id_ << std::endl;
    }
}
//-----------------------------------------------------------------------------
// msgGetKernelWorkGroupInfo
//-----------------------------------------------------------------------------
void dcl_message< msgGetKernelWorkGroupInfo >::create_request( void* payload_ptr )
{
    remote_id_t* request_ptr = reinterpret_cast< remote_id_t* >( payload_ptr );

    request_ptr[ 0 ] = host_to_network( kernel_id_ );
    request_ptr[ 1 ] = host_to_network( device_id_ );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetKernelWorkGroupInfo >::parse_request( const void* payload_ptr )
{
    const remote_id_t* request_ptr = reinterpret_cast< const remote_id_t* >( payload_ptr );

    kernel_id_ = network_to_host( request_ptr[ 0 ] );
    device_id_ = network_to_host( request_ptr[ 1 ] );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetKernelWorkGroupInfo >::create_response( void* payload_ptr )
{
    msgGetKernelWorkGroupInfo_response* response_ptr =
        reinterpret_cast< msgGetKernelWorkGroupInfo_response* >( payload_ptr );

    response_ptr->local_mem_size_ = host_to_network( info_.local_mem_size_ );
    response_ptr->work_group_size_ = host_to_network( static_cast<uint32_t>( info_.work_group_size_ ) );
    response_ptr->private_mem_size_ = host_to_network( info_.private_mem_size_ );
    response_ptr->preferred_work_group_size_multiple_ = host_to_network( static_cast<uint32_t>( info_.preferred_work_group_size_multiple_ ) );

    response_ptr->compile_work_group_size_[ 0 ] = host_to_network( static_cast<uint32_t>( info_.compile_work_group_size_[ 0 ] ) );
    response_ptr->compile_work_group_size_[ 1 ] = host_to_network( static_cast<uint32_t>( info_.compile_work_group_size_[ 1 ] ) );
    response_ptr->compile_work_group_size_[ 2 ] = host_to_network( static_cast<uint32_t>( info_.compile_work_group_size_[ 2 ] ) );
}
//-----------------------------------------------------------------------------
void dcl_message< msgGetKernelWorkGroupInfo >::parse_response( const void* payload_ptr )
{
    const msgGetKernelWorkGroupInfo_response* response_ptr =
        reinterpret_cast< const msgGetKernelWorkGroupInfo_response* >( payload_ptr );

    info_.local_mem_size_ = network_to_host( response_ptr->local_mem_size_ );
    info_.work_group_size_ = network_to_host( response_ptr->work_group_size_ );
    info_.private_mem_size_ = network_to_host( response_ptr->private_mem_size_ );
    info_.preferred_work_group_size_multiple_ = network_to_host( response_ptr->preferred_work_group_size_multiple_ );

    info_.compile_work_group_size_[ 0 ] = network_to_host( response_ptr->compile_work_group_size_[ 0 ] );
    info_.compile_work_group_size_[ 1 ] = network_to_host( response_ptr->compile_work_group_size_[ 1 ] );
    info_.compile_work_group_size_[ 2 ] = network_to_host( response_ptr->compile_work_group_size_[ 2 ] );
}
//-----------------------------------------------------------------------------
}}} // namespace dcl::network::message
//-----------------------------------------------------------------------------
