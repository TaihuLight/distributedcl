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
#ifndef _DCL_COMPOSITE_COMMAND_QUEUE_H_
#define _DCL_COMPOSITE_COMMAND_QUEUE_H_
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <set>
#include "distributedcl_internal.h"
#include "info/command_queue_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace composite {
//-----------------------------------------------------------------------------
typedef dcl::info::generic_command_queue composite_command_queue;
//-----------------------------------------------------------------------------
//class composite_command_queue :
//    public dcl::info::generic_command_queue
//{
//public:
//    composite_command_queue( const dcl::info::generic_context* context_ptr,
//                             const dcl::info::generic_device* device_ptr,
//                             cl_command_queue_properties properties ) :
//        dcl::info::generic_command_queue( context_ptr, device_ptr, properties ){}
//
//    void enable_async_execution()
//    {
//        execute_thread_sp_.reset( new boost::thread( &dcl::composite::composite_command_queue::work_thread, this ) );
//    }
//
//private:
//    boost::scoped_ptr<boost::thread> execute_thread_sp_;
//
//    void work_thread()
//    {
//        while( 1 )
//        {
//        }
//    }
//};
//-----------------------------------------------------------------------------
}} // namespace dcl::composite
//-----------------------------------------------------------------------------
#endif //_DCL_COMPOSITE_COMMAND_QUEUE_H_
