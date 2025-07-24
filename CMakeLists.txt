cmake_minimum_required(VERSION 3.30)

set(CMAKE_C_COMPILER $ENV{GCC_LATEST}/bin/gcc-15.1)
set(CMAKE_CXX_COMPILER $ENV{GCC_LATEST}/bin/g++-15.1)
add_compile_options(-std=c++26)
set(CMAKE_BUILD_TYPE Debug)

project(qt_vk)

include_directories("modules")

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

if (DEFINED ENV{VULKAN_SDK} AND NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    message( "VULKAN_SDK is set to $ENV{VULKAN_SDK}")
    include_directories($ENV{VULKAN_SDK}/include)
else()
    message(SEND_ERROR "VULKAN_SDK is not set")
endif()

set(QT_VERSION 6.9)
list(APPEND CMAKE_PREFIX_PATH $ENV{LOCAL_LATEST_QT})
find_package(Qt6 ${QT_VERSION} COMPONENTS
    Core
    Gui
    GuiPrivate
    Quick
    QuickWidgets
    QuickControls2
    Qml
)

# Module algebra
# ================================
file(GLOB MOD_ALGEBRA_SRC "modules/algebra/*.cpp" "modules/algebra/*.h")
add_library(algebra)
target_sources(algebra PRIVATE ${MOD_ALGEBRA_SRC})
set_target_properties(algebra PROPERTIES LINKER_LANGUAGE CXX)

# Module log
# ================================
add_library(log
    modules/log/log.h
    modules/log/log.cpp
)
#global user defined output messages macro definition
add_compile_definitions(ENABLE_DEBUG_OUTPUT=false)
add_compile_definitions(ENABLE_WARNING_OUTPUT=true)
add_compile_definitions(ENABLE_INFO_OUTPUT=true)
add_compile_definitions(ENABLE_NOTICE_OUTPUT=true)
add_compile_definitions(ENABLE_ERROR_OUTPUT=true)
set_target_properties(log PROPERTIES LINKER_LANGUAGE CXX)

# Module mesh
# ================================
file(GLOB MOD_MESH_SRC "modules/mesh/*.cpp")
add_library(mesh)
target_sources(mesh
  PUBLIC
    FILE_SET CXX_MODULES FILES
    ${MOD_MESH_SRC}
)
target_compile_features(mesh PUBLIC cxx_std_26)

# Module render
# ================================
file(GLOB_RECURSE MOD_RENDER_SRC "modules/render/*.cpp" "modules/render/*.h")
add_library(render)
target_sources(render PRIVATE ${MOD_RENDER_SRC})
target_link_libraries(render vulkan algebra log mesh)

# The Service
# ================================
file(GLOB MOD_TIRE_SRC "src/*.cpp" "src/*.h")
add_executable(${PROJECT_NAME} ${MOD_TIRE_SRC})
set_target_properties(qt_vk PROPERTIES AUTOMOC TRUE)
target_link_libraries(${PROJECT_NAME}
    render
    Qt::Core
    Qt::Gui
    Qt6::GuiPrivate
    Qt::Quick
    Qt::QuickWidgets
    Qt::QuickControls2
    Qt::Qml
)

# VK test 
#add_subdirectory(test/vk)
#target_link_libraries(vk_test PUBLIC
#    log
#    algebra
#    render
#    X11
#    Xfixes
#    GL
#    Vulkan::Vulkan
#)
