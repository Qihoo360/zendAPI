// @copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Created by softboy on 2017/11/01.

#ifndef ZAPI_TEST_DUMMYEXT_CLASS_TESTCASES_H
#define ZAPI_TEST_DUMMYEXT_CLASS_TESTCASES_H

#include "zapi/ZendApi.h"

namespace dummyext 
{

using zapi::lang::Extension;

ZAPI_DECL_EXPORT void register_class_testcases(Extension &extension);
void register_basic_classes(Extension &extension);
void register_construct_and_destruct_classes(Extension &extension);
void register_namespace_classes(Extension &extension);
void register_inherit_test_classes(Extension &extension);
void register_iterator_test_classes(Extension &extension);

} // dummyext

#endif // ZAPI_TEST_DUMMYEXT_CLASS_TESTCASES_H
