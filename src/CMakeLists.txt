cmake_minimum_required(VERSION 3.5)
project(org_chart CXX)

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cc)

# ##############################################################################
# https://github.com/drogonframework/drogon
add_subdirectory(third_party/jsoncpp)
add_subdirectory(third_party/drogon)

target_link_libraries(${PROJECT_NAME} PRIVATE drogon)

# https://github.com/Thalhammer/jwt-cpp
add_subdirectory(third_party/jwt-cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE jwt-cpp)

# https://github.com/trusch/libbcrypt
add_subdirectory(third_party/libbcrypt)
target_link_libraries(${PROJECT_NAME} PRIVATE bcrypt)

# Add bundled jsoncpp
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/jsoncpp/include)
target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp_lib)

# ##############################################################################
# REMOVE or COMMENT OUT these lines, since you are using the bundled Drogon:
# find_package(Drogon CONFIG REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

# ##############################################################################

if (CMAKE_CXX_STANDARD LESS 17)
    # With C++14, use boost to support any, string_view and filesystem
    message(STATUS "use c++14")
    find_package(Boost 1.61.0 REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Boost::boost)
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

# Gather sources from subdirectories
aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)
aux_source_directory(utils UTIL_SRC)

# Include directories for models and libraries
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/models
        ${CMAKE_CURRENT_SOURCE_DIR}/controllers
        ${CMAKE_CURRENT_SOURCE_DIR}/filters
        ${CMAKE_CURRENT_SOURCE_DIR}/plugins
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libbcrypt/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/jsoncpp/include
)

# Add all sources to the project (removed ${SRC_DIR} as it was not defined)
target_sources(${PROJECT_NAME}
    PRIVATE
        main.cc
        ${CTL_SRC}
        ${FILTER_SRC}
        ${PLUGIN_SRC}
        ${MODEL_SRC}
        ${UTIL_SRC}
)

# ##############################################################################
# uncomment the following line for dynamically loading views
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################

add_subdirectory(test)

# add_executable(${PROJECT_NAME}_test test/test_main.cc)
# target_link_libraries(${PROJECT_NAME}_test PRIVATE drogon)
# ParseAndAddDrogonTests(${PROJECT_NAME}_test)
