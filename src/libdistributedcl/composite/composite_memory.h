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
#ifndef _DCL_COMPOSITE_MEMORY_H_
#define _DCL_COMPOSITE_MEMORY_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include "distributedcl_internal.h"
#include "composite_object.h"
#include "info/memory_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
class composite_context;
//-----------------------------------------------------------------------------
class composite_memory :
    public dcl::info::generic_memory,
    public composite_object< dcl::info::generic_memory >
{
public:
    static const char* get_name(){ return "composite_memory"; }

    composite_memory( const composite_context& context_ref,
                      const void* host_ptr, size_t size, cl_mem_flags flags );

    virtual ~composite_memory(){}

    virtual void write( dcl::info::generic_command_queue* queue_ptr, const void* data_ptr,
                        size_t size, size_t offset, cl_bool blocking, events_t& wait_events,
                        dcl::info::generic_event** ret_event_ptr );

    virtual void read( dcl::info::generic_command_queue* queue_ptr, void* data_ptr,
                       size_t size, size_t offset, cl_bool blocking, events_t& wait_events,
                       dcl::info::generic_event** ret_event_ptr );

    virtual void* map( dcl::info::generic_command_queue* queue_ptr, cl_map_flags flags,
                       size_t size, size_t offset, cl_bool blocking,
                       events_t& wait_events, dcl::info::generic_event** ret_event_ptr );

    virtual void unmap( dcl::info::generic_command_queue* queue_ptr, void* data_ptr,
                        events_t& wait_events, dcl::info::generic_event** ret_event_ptr );

    virtual void copy( dcl::info::generic_command_queue* queue_ptr,
                       dcl::info::generic_memory* src_ptr,
                       size_t size, size_t src_offset, size_t dst_offset,
                       events_t& wait_events, dcl::info::generic_event** ret_event_ptr );
};
//-----------------------------------------------------------------------------
class composite_image :
    public dcl::info::generic_image,
    public composite_object< dcl::info::generic_image >
{
public:
    static const char* get_name(){ return "composite_image"; }

    composite_image( const composite_context& context_ref,
                     const void* host_ptr, cl_mem_flags flags,
                     const cl_image_format* format, size_t width,
                     size_t height, size_t row_pitch );

    virtual ~composite_image(){}

    virtual void write( dcl::info::generic_command_queue* queue_ptr, const void* data_ptr,
                        const size_t origin[3], const size_t region[3],
                        size_t row_pitch, size_t slice_pitch, bool blocking,
                        events_t& wait_events, dcl::info::generic_event** ret_event_ptr );

    virtual void unmap( dcl::info::generic_command_queue* queue_ptr, void* data_ptr,
                        events_t& wait_events, dcl::info::generic_event** ret_event_ptr );
};
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
#endif //_DCL_COMPOSITE_MEMORY_H_
