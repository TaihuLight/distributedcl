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
#include "server_program.h"
#include "server_platform.h"
#include "message/msg_program.h"
#include "composite/composite_context.h"
#include "composite/composite_program.h"
using dcl::composite::composite_context;
using dcl::composite::composite_program;
//-----------------------------------------------------------------------------
namespace dcl {
namespace server {
//-----------------------------------------------------------------------------
void CreateProgramWithSource_command::execute()
{
    server_platform& server = server_platform::get_instance();

    remote_id_t context_id = message_.get_context_id();

    composite_context* context_ptr = server.get_context_manager().get( context_id );

    composite_program* program_ptr = 
        reinterpret_cast<composite_program*>( context_ptr->create_program( message_.get_source_code() ) );

    remote_id_t id = server.get_program_manager().add( program_ptr );

    message_.set_remote_id( id );
}
//-----------------------------------------------------------------------------
void BuildProgram_command::execute()
{
    remote_id_t program_id = message_.get_program_id();

    composite_program* program_ptr = 
        server_platform::get_instance().get_program_manager().get( program_id );

    program_ptr->build( message_.get_build_options() );
}
//-----------------------------------------------------------------------------
}} // namespace dcl::server
//-----------------------------------------------------------------------------