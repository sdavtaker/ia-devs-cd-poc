add_library(ia_devs_cd_lib INTERFACE)
add_library(ia_devs_cd::lib ALIAS ia_devs_cd_lib)

target_include_directories(
        ia_devs_cd_lib
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)