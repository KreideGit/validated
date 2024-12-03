cmake_minimum_required(VERSION 3.20)
project(validated VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Enable or disable building tests" ON)
option(BUILD_EXAMPLES "Enable or disable building examples" ON)

add_library(${PROJECT_NAME} INTERFACE)
add_library(ck::${PROJECT_NAME} ALIAS validated)

target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

install(DIRECTORY include/ DESTINATION include)
install(TARGETS ${PROJECT_NAME} EXPORT validatedConfig)

install(EXPORT validatedConfig
        DESTINATION lib/cmake/${PROJECT_NAME}
        NAMESPACE ck::
)