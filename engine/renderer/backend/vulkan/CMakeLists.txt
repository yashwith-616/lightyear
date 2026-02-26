#-------------------------------------------------
# Configure files and directory
#-------------------------------------------------
set(VK_PUBLIC
        public/vk/VulkanContext.h
)

set(VK_PRIVATE
        private/bootstrap/Instance.cpp
        private/bootstrap/Instance.h
        private/bootstrap/Surface.h
        private/device/PhysicalDevice.cpp
        private/device/PhysicalDevice.h
        private/device/LogicalDevice.cpp
        private/device/LogicalDevice.h
        private/device/QueueFamilyData.h
        private/pipeline/Pipeline.cpp
        private/pipeline/Pipeline.h
        private/pipeline/Shader.cpp
        private/pipeline/Shader.h
        private/systems/SwapchainSystem.cpp
        private/systems/SwapchainSystem.h
        private/systems/VulkanFrameContext.cpp
        private/systems/VulkanFrameContext.h
        private/components/SwapchainData.h
        private/components/FrameData.h
        private/components/CommandData.h
        private/components/FrameContext.h
        private/command/CommandObject.cpp
        private/command/CommandObject.h
        private/command/CommandRegistry.cpp
        private/command/CommandRegistry.h
        private/command/CommandType.h)

#-------------------------------------------------
# Dependencies
#-------------------------------------------------
find_package(Vulkan REQUIRED)
find_package(VulkanHeaders CONFIG)

#-------------------------------------------------
# Options
#-------------------------------------------------
option(ENABLE_ASAN "enable address sanitizer" OFF)

#-------------------------------------------------
# Target configuration
#-------------------------------------------------
add_library(VulkanRenderer STATIC
        ${VK_PRIVATE}
)

set_target_properties(VulkanRenderer PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(VulkanRenderer
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/private
        ${CMAKE_CURRENT_SOURCE_DIR}/public)

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
        VULKAN_HPP_NO_EXCEPTIONS
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
else ()
    target_compile_definitions(VulkanRenderer
            PRIVATE
            ENABLE_VK_VALIDATION=0)
endif ()
