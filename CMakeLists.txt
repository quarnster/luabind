cmake_minimum_required(VERSION 2.8)

find_package(Boost REQUIRED)

include_directories(
    "."
    ${Boost_INCLUDE_DIRS}
)

if(ANDROID)
    include_directories(${ANDROID_STANDALONE_TOOLCHAIN}/user/include/luajit-2.0)
else(ANDROID)
    include_directories(/usr/local/include/luajit-2.0)
endif(ANDROID)

add_library(luabind STATIC
    src/class.cpp
    src/class_info.cpp
    src/class_registry.cpp
    src/class_rep.cpp
    src/create_class.cpp
    src/error.cpp
    src/exception_handler.cpp
    src/function.cpp
    src/inheritance.cpp
    src/link_compatibility.cpp
    src/object_rep.cpp
    src/open.cpp
    src/pcall.cpp
    src/scope.cpp
    src/stack_content_by_name.cpp
    src/weak_ref.cpp
    src/wrapper_base.cpp
)
