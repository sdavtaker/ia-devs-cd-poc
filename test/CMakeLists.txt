find_package(Catch2 REQUIRED)
ENABLE_TESTING()

add_executable(trivial_test)
target_sources(
        trivial_test
        PRIVATE
        test_placeholder.cpp
)
target_link_libraries(
        trivial_test
        ia_devs_cd_lib
        Catch2::Catch2WithMain
)

add_test(NAME trivial_test COMMAND trivial_test)
