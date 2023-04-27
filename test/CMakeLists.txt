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
        ia_devs_cd::lib
        Catch2::Catch2WithMain
)
add_test(NAME test_interval COMMAND test_interval)

add_executable(test_generator)
target_sources(
        test_generator
        PRIVATE
        test_generator.cpp
)
target_link_libraries(
        test_generator
        ia_devs_cd::lib
        Catch2::Catch2WithMain
)
add_test(NAME test_generator COMMAND test_generator)

add_executable(test_simulator)
target_sources(
        test_simulator
        PRIVATE
        test_simulator.cpp
)
target_link_libraries(
        test_simulator
        ia_devs_cd::lib
        Catch2::Catch2WithMain
)
add_test(NAME test_simulator COMMAND test_simulator)
