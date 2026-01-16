#-------------------------------------------------
# Configure files and directory
#-------------------------------------------------
set(LOG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/logging")
set(LOG_SRCS
        ${LOG_DIR}/src/Log.cpp)
set(LOG_PUBLIC
        ${LOG_DIR}/include/logging/Log.h
        ${LOG_DIR}/include/logging/Config.h)

#-------------------------------------------------
# Find Packages
#-------------------------------------------------
find_package(spdlog CONFIG REQUIRED)

#-------------------------------------------------
# Create library
#-------------------------------------------------
add_library(logging SHARED
        ${LOG_SRCS}
        ${LOG_PUBLIC}
)

target_include_directories(logging
        PUBLIC
        ${LOG_DIR}/include
)

target_compile_features(logging PUBLIC cxx_std_20)

target_link_libraries(logging
        PUBLIC
        spdlog::spdlog_header_only
        lightyear::common
)
#-------------------------------------------------
# Compile Definations
#-------------------------------------------------
target_compile_definitions(logging PRIVATE LOGGING_BUILD_DLL)

#-------------------------------------------------
# Install Sources
#-------------------------------------------------
target_sources(logging
        PRIVATE
            ${LOG_DIR}/src/Log.cpp
        PUBLIC
        FILE_SET public_headers TYPE HEADERS
        BASE_DIRS ${LOG_DIR}/include
        FILES
            ${LOG_DIR}/include/logging/Log.h
            ${LOG_DIR}/include/logging/Config.h
)