cmake_minimum_required(VERSION 3.20)
project(GUI LANGUAGES CXX)

# ===============================
# Library target
# ===============================

file(GLOB_RECURSE SRC_FILES
    CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GUI/*.cpp
)

add_library(GUI ${SRC_FILES})
add_library(GUI::GUI ALIAS GUI)

target_compile_features(GUI PUBLIC cxx_std_20)

target_include_directories(GUI
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(GUI PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE GUI_EXPORTS
)

# ===============================
# System dependency
# ===============================

if(PROJECT_IS_TOP_LEVEL)
    find_package(Vulkan REQUIRED)
endif()
target_link_libraries(GUI PUBLIC Vulkan::Vulkan)

# ===============================
# Text rendering dependencies
# ===============================



if(PROJECT_IS_TOP_LEVEL)
    find_package(ICU REQUIRED COMPONENTS uc i18n)
    add_subdirectory(
        ${Freetype_DIR} ${CMAKE_CURRENT_BINARY_DIR}/freetype
    )

    add_subdirectory(
        ${harfbuzz_DIR} ${CMAKE_CURRENT_BINARY_DIR}/harfbuzz
    )
endif()

if(NOT TARGET freetype::freetype)
    add_library(freetype::freetype ALIAS freetype)
endif()

if(NOT TARGET harfbuzz::harfbuzz)
    add_library(harfbuzz::harfbuzz ALIAS harfbuzz)
endif()

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        freetype::freetype
        ICU::uc
        ICU::i18n
        harfbuzz::harfbuzz
)

# ===============================
# Other libraries
# ===============================

if(PROJECT_IS_TOP_LEVEL)
    find_package(CommonApi REQUIRED CONFIG)
    find_package(Containers REQUIRED CONFIG)
    find_package(PlatformKit REQUIRED CONFIG)
    add_subdirectory(
        ${GraphicsWrapper_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/GraphicsWrapper
    )
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC CommonApi::CommonApi)
target_link_libraries(${PROJECT_NAME} PUBLIC Containers::Containers)
target_link_libraries(${PROJECT_NAME} PUBLIC PlatformKit::PlatformKit)
target_link_libraries(${PROJECT_NAME} PUBLIC GraphicsWrapper)

# =========================
# Test executable
# =========================
file(GLOB_RECURSE TEST_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
)

add_executable(GUITests ${TEST_SRC_FILES})

# Link the library
target_link_libraries(GUITests PRIVATE ${PROJECT_NAME})

# Match C++ standard and warnings
target_compile_features(GUITests PUBLIC cxx_std_20)
target_compile_options(GUITests PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_include_directories(GUITests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enable CTest
enable_testing()
add_test(NAME GUITests COMMAND GUITests)

# ===============================
# Install & package (TOP-LEVEL ONLY)
# ===============================

install(TARGETS ${PROJECT_NAME}
    EXPORT GUITargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT GUITargets
    FILE GUITargets.cmake
    NAMESPACE GUI::
    DESTINATION lib/cmake/GUI
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/install/GUIConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/GUIConfig.cmake
    INSTALL_DESTINATION lib/cmake/GUI
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GUIConfig.cmake
    DESTINATION lib/cmake/GUI
)