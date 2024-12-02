include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(Catch2)

add_executable(test_validated test_validated.cpp)
target_link_libraries(test_validated PRIVATE Catch2::Catch2WithMain ck::validated)

include(CTest)
include(Catch)
catch_discover_tests(test_validated)