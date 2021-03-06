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
#ifndef _DCL_DEVICE_H_
#define _DCL_DEVICE_H_

#include "distributedcl_internal.h"
#include "opencl_single.h"
#include "info/device_info.h"
#include "opencl_library.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace single {
//-----------------------------------------------------------------------------
class device :
    public dcl::info::generic_device,
    public opencl_object< cl_device_id >
{
public:
    virtual ~device()
    {
        if( loaded_devices_ )
            loaded_devices_->erase( get_id() );
    }

    static device* get_device( const platform* platform_ptr, cl_device_id id );

private:
    device( const platform* platform_ptr, cl_device_id id );
    virtual bool load_device_info();

    void load_info_data( cl_device_info info );
    void load_info_string( cl_device_info info, std::string& str );

    static std::map<cl_device_id, device*>* loaded_devices_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::single
//-----------------------------------------------------------------------------
#endif // _DCL_DEVICE_H_
