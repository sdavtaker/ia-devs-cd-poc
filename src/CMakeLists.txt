add_executable(ia_devs_cd_poc main.cpp)
target_link_libraries(
        ia_devs_cd_poc
        PRIVATE
        ia_devs_cd_lib
)