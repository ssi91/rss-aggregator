cmake_minimum_required(VERSION 3.2)
project(rss_aggregator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(/usr/include/libxml++-2.6)
include_directories(/usr/include/glibmm-2.4)
include_directories(/usr/lib/x86_64-linux-gnu/glibmm-2.4/include)
include_directories(/usr/include/glib-2.0)
include_directories(/usr/lib/x86_64-linux-gnu/glib-2.0/include)
include_directories(/usr/lib/libxml++-2.6/include)


include_directories("/home/ssi/mongo-client-install/include")
link_directories("/home/ssi/mongo-client-install/lib")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)

set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.54.0 REQUIRED COMPONENTS thread system regex filesystem)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(PkgConfig)
pkg_check_modules(XMLPP libxml++-2.6)

link_directories(${XMLPP_LIBRARY_DIRS})

set(SOURCE_FILES
    feed/post/Post.cpp
#    maintest.cpp
    feed/Feed.cpp
    feed/rss/RSS.cpp
    feed/Idb.h
    feed/mongo/Mongo.cpp
    dir.cpp
    renderexception.cpp
    Stack.h
    testimport.cpp
    some-functions/sameFunctions.cpp
    )
add_executable(rss_aggregator ${SOURCE_FILES})
target_link_libraries(rss_aggregator
    mongoclient
    pthread
    sasl2
    crypto
    ssl
    ${XMLPP_LIBRARIES}
    ${Boost_LIBRARIES}
    SimpleAmqpClient
    curl
    )