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
#include <boost/scoped_array.hpp>
#include "context.h"
#include "platform.h"
#include "device.h"
using dcl::info::generic_context;
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
context::context( const context& ctx ) : 
    generic_context( ctx.devices_ ), opencl_object( ctx.get_opencl(), ctx.get_id() )
{
}
//-----------------------------------------------------------------------------
context::context( const devices_t& devices_ref ) : 
    generic_context( devices_ref ), opencl_object( reinterpret_cast<device*>( devices_ref[ 0 ] )->get_opencl() )
{
    //if( devices_ref.empty() )
    //    throw library_exception( CL_INVALID_VALUE );

    //boost::scoped_array<cl_device_id> deviceIDs( new cl_device_id[ devices_ref.size() ] );

    //int i = 0;
    //for( devices_t::const_iterator it = devices_ref.begin(); it != devices_ref.end(); it++ )
    //{
    //    deviceIDs[ i++ ] = (*it)->get_id();
    //}

    //cl_int error_code;

    ////TODO: Work with context properties and callback function
    //cl_context context = get_opencl().clCreateContext( NULL,
    //                                                   static_cast<cl_uint>( devices_ref.size() ),
    //                                                   deviceIDs.get(),
    //                                                   static_cast<ocg::logging_fn>( NULL ),
    //                                                   static_cast<void*>( NULL ),
    //                                                   &error_code );
    //if( error_code != CL_SUCCESS )
    //    throw library_exception( error_code );

    //set_id( context );
}
//-----------------------------------------------------------------------------
context::context( const platform& platform_ref, cl_device_type device_type ) : 
    opencl_object( platform_ref.get_opencl() )
{
    cl_int error_code;
    cl_context ctx;

    //TODO: Work with callback function

    cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, reinterpret_cast< cl_context_properties >( platform_ref.get_id() ), 0 };

    ctx = opencl_.clCreateContextFromType( properties, device_type, NULL, 
                                                static_cast<void*>( NULL ), &error_code );

    if( error_code != CL_SUCCESS )
        throw library_exception( error_code );

    // Load the devices objects
    size_t param_value_size_ret;
    error_code = opencl_.clGetContextInfo( ctx, CL_CONTEXT_DEVICES, 0, NULL, &param_value_size_ret );

    if( error_code != CL_SUCCESS )
        throw library_exception( error_code );

    size_t ret_count = param_value_size_ret / sizeof( cl_device_id );

    boost::scoped_array<cl_device_id> device_ids( new cl_device_id[ ret_count ] );

    error_code = opencl_.clGetContextInfo( ctx, CL_CONTEXT_DEVICES, param_value_size_ret, device_ids.get(), NULL );

    if( error_code != CL_SUCCESS )
        throw library_exception( error_code );

    for( int i = 0; i < (int) ret_count; i++ )
    {
        devices_.push_back( new device( &platform_ref, device_ids[ i ] ) );
    }

    set_id( ctx );
}
//-----------------------------------------------------------------------------
//void context::add( command_queue& queue )
//{
//    queue.set_context( this, &queue );
//}
//-----------------------------------------------------------------------------
void context::load_image_formats( image_formats_t& image_formats, cl_mem_object_type image_type )
{
    //cl_uint num_entries;
    //cl_uint num_image_formats;

    //cl_int ret = get_opencl().clGetSupportedImageFormats( get_id(), 0, image_type, 0, 
    //                                                      static_cast<cl_image_format*>( NULL ), &num_entries );
    //if( ret != CL_SUCCESS )
    //{
    //    throw library_exception( ret );
    //}

    //boost::scoped_array<cl_image_format> formats( new cl_image_format[ num_entries ] );

    //ret = get_opencl().clGetSupportedImageFormats( get_id(), 0, image_type, num_entries, 
    //                                               formats.get(), &num_image_formats );
    //if( ret != CL_SUCCESS )
    //{
    //    throw library_exception( ret );
    //}

    //image_formats.clear();
    //image_formats.reserve( num_image_formats );
    //image_formats.assign( formats.get(), formats.get() + num_image_formats );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------