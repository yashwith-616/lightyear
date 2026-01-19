#-------------------------------------------------
# Configure files and directory
#-------------------------------------------------
set(VK_SRCS
        src/Queue.cpp
        src/Queue.h)

set(VK_PRIVATE
        private/bootstrap/Instance.cpp
        private/bootstrap/Instance.h
        private/bootstrap/Surface.cpp
        private/bootstrap/Surface.h
        private/device/PhysicalDevice.cpp
        private/device/PhysicalDevice.h
        private/device/LogicalDevice.cpp
        private/device/LogicalDevice.h)

#-------------------------------------------------
# Dependencies
#-------------------------------------------------
find_package(Vulkan REQUIRED)
find_package(VulkanHeaders CONFIG)

#-------------------------------------------------
# Target configuration
#-------------------------------------------------
add_library(VulkanRenderer SHARED
        ${VK_HDRS}
        ${VK_SRCS}
        ${VK_PRIVATE}
)

set_target_properties(VulkanRenderer PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(VulkanRenderer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(VulkanRenderer
        PRIVATE
            project_compile_options
        PUBLIC
            Vulkan::Headers
            Vulkan::Vulkan
)

target_compile_definitions(VulkanRenderer
        PRIVATE
            VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
            VULKAN_HPP_NOEXCEPT
)

#-------------------------------------------------
# Vulkan Optional flags
#-------------------------------------------------
option(ENABLE_VULKAN_VALIDATION_LAYERS "Enable Vulkan validation layers" ON)

#-------------------------------------------------
# Vulkan Validation Layers
#-------------------------------------------------
if (ENABLE_VULKAN_VALIDATION_LAYERS)
    set(VK_VLAYER_PATH "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")

    target_compile_definitions(VulkanRenderer
            PRIVATE
            ENABLE_VK_VALIDATION=1
            VALIDATION_LAYER_PATH="${VK_VLAYER_PATH}")
else()
    target_compile_definitions(VulkanRenderer
            PRIVATE
            ENABLE_VK_VALIDATION=0)
endif()
