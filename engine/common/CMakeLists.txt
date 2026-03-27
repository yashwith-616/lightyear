add_library(LyCommon INTERFACE)

add_library(lightyear::common ALIAS LyCommon)
target_include_directories(LyCommon
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(LyCommon
        INTERFACE cxx_std_20
)

#-------------------------------------------
# INSTALL PART
#-------------------------------------------
install(
        DIRECTORY include
        DESTINATION include
)

install(
        TARGETS LyCommon
        EXPORT LyCommonTargets
)

install(
        EXPORT LyCommonTargets
        FILE LyCommonTargets.cmake
        NAMESPACE Lightyear::
        DESTINATION lib/cmake/LightyearCommon
)

