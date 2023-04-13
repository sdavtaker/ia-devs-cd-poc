find_package(Catch2 REQUIRED)
ENABLE_TESTING()

add_executable(test_interval)
target_sources(
        test_interval
        PRIVATE
        test_interval.cpp
)
target_link_libraries(
        test_interval
        ia_devs_cd_lib
        Catch2::Catch2WithMain
)

add_test(NAME test_interval COMMAND test_interval)
