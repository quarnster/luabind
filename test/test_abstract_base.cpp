// Copyright (c) 2004 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#include "test.hpp"
#include <luabind/luabind.hpp>

#include <luabind/object.hpp>
#include <luabind/operator.hpp>
#include <luabind/raw_policy.hpp>

using namespace luabind;

namespace {

    struct abstract
    {
        virtual ~abstract() {}
        virtual std::string hello() = 0;
    }; 

    struct abstract_wrap : abstract, wrap_base
    {
        std::string hello()
        {
            return call_member<std::string>(this, "hello");
        }

        static void default_hello(abstract const&)
        {
            throw std::runtime_error("abstract function");
        }
    };

    std::string call_hello(abstract& a)
    {
        return a.hello();
    }

} // namespace unnamed

#include <vector>

void test_abstract_base()
{
    COUNTER_GUARD(abstract);

    lua_state L;

    module(L)
    [
        class_<abstract, abstract_wrap>("abstract")
            .def(constructor<>())
            .def("hello", &abstract::hello/*, &abstract_wrap::default_hello*/),

        def("call_hello", &call_hello)
    ];
    
    DOSTRING_EXPECTED(L,
        "x = abstract()\n"
        "x:hello()\n"
      , "pure virtual function called");

    DOSTRING_EXPECTED(L, 
        "call_hello(x)\n"
      , "pure virtual function called");
    
    DOSTRING(L,
        "class 'concrete' (abstract)\n"
        "  function concrete:__init()\n"
        "      super()\n"
        "  end\n"

        "  function concrete:hello()\n"
        "      return 'hello from lua'\n"
        "  end\n");

    DOSTRING(L,
        "y = concrete()\n"
        "y:hello()\n");

    DOSTRING(L, "call_hello(y)\n");
}
