add_executable(person person.cpp)
target_link_libraries(person PRIVATE ck::validated)

add_executable(safe_list safe_list.cpp)
target_link_libraries(safe_list PRIVATE ck::validated)