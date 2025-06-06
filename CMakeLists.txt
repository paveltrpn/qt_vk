cmake_minimum_required(VERSION 3.30)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-std=c++26)
#add_compile_options(-stdlib=libc++)
#add_link_options(-stdlib=libc++)
set(CMAKE_BUILD_TYPE Debug)

project(tire)

include_directories("modules")

if (DEFINED ENV{VULKAN_SDK} AND NOT "$ENV{VULKAN_SDK}" STREQUAL "")
    message( "VULKAN_SDK is set to $ENV{VULKAN_SDK}")
    include_directories($ENV{VULKAN_SDK}/include)
else()
    message(SEND_ERROR "VULKAN_SDK is not set")
endif()

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

# Module render
# ================================
file(GLOB_RECURSE MOD_RENDER_SRC "modules/render/*.cpp" "modules/render/*.h")
add_library(render)
target_sources(render PRIVATE ${MOD_RENDER_SRC})
target_link_libraries(render algebra log)

set(QT_VERSION 6.9)
list(APPEND CMAKE_PREFIX_PATH $ENV{LOCAL_LATEST_QT})
find_package(Qt6 ${QT_VERSION} COMPONENTS
    Core
    Gui
    Quick
    QuickWidgets
    QuickControls2
    Qml)

# ===========================================================

add_library(window
    src/window/window.h
    src/window/window.cpp
    src/window/theme_manager.h
    src/window/theme_manager.cpp
    src/window/tree_model.h
    src/window/tree_model.cpp
    src/window/list_model.h
    src/window/list_model.cpp
)
set_target_properties(window PROPERTIES AUTOMOC TRUE)
target_link_libraries(window PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::QuickWidgets
    Qt::QuickControls2
    Qt::Qml)

# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME}
    window)

# VK test 
add_subdirectory(test/vk)
target_link_libraries(vk_test PUBLIC 
    log 
    algebra 
    render 
    X11
    Xfixes
    Vulkan::Vulkan 
)
