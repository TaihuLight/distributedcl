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
#ifndef _DCL_REMOTE_DEVICE_H_
#define _DCL_REMOTE_DEVICE_H_

#include "distributedcl_internal.h"
#include "library_exception.h"
#include "remote_object.h"
#include "info/device_info.h"
//-----------------------------------------------------------------------------
namespace dcl {
namespace remote {
//-----------------------------------------------------------------------------
class remote_device : 
    public cl_object< cl_device_id, cl_device_info, CL_INVALID_DEVICE >,
    public icd_object< cl_device_id, remote_device, dcl_device_id >,
    public dcl_object< dcl::info::device_info >,
    public remote_object
{
public:
    remote_device( dcl::network::client::session& session_ref, dcl::remote_id_t remote_id ) : 
        remote_object( session_ref, remote_id ), data_loaded_( false ) {}

    ~remote_device();

    const dcl_info_t& get_info();

private:
    bool data_loaded_;
};
//-----------------------------------------------------------------------------
}} // namespace dcl::remote
//-----------------------------------------------------------------------------
#endif // _DCL_REMOTE_DEVICE_H_
