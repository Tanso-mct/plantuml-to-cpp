# CMakeLists.txt: plantuml-to-cpp' CMake project. Including the source, define the following:
# The project-specific logic is here.
#
cmake_minimum_required(VERSION 3.8)

# Enable hot reload for the MSVC compiler if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT"$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("workspace LANGUAGES CXX")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Library ----
add_library(plantuml-to-cpp-lib STATIC
    # ---- Headers ----

    include/plantuml-to-cpp/file_io.h
    include/plantuml-to-cpp/class_diagram.h
    include/plantuml-to-cpp/converter.h
    include/plantuml-to-cpp/file_pu.h
    include/plantuml-to-cpp/file_cpp.h

    # ---- Sources ----

    src/file_io.cpp
    src/class_diagram.cpp
    src/converter.cpp
    src/file_pu.cpp
    src/file_cpp.cpp
)

# C++ 17 standard
target_compile_features(plantuml-to-cpp-lib
    PUBLIC
        cxx_std_17
)

# Public headers
target_include_directories(plantuml-to-cpp-lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Precompiled headers
target_precompile_headers(plantuml-to-cpp-lib
    PRIVATE
        "src/pch.h"
)

# ---- Executable ----
add_executable(plantuml-to-cpp
    app/main.cpp
)

# Link the library to the executable
target_link_libraries(plantuml-to-cpp
    PRIVATE
        plantuml-to-cpp-lib
)

# Compile options
target_compile_options(plantuml-to-cpp-lib 
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# ---- Tests ----
include(CTest)  # BUILD_TESTING option + enable_testing()

if (BUILD_TESTING)
    include(FetchContent)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(plantuml-to-cpp_tests
        tests/file_io.cpp
        tests/class_diagram.cpp
        tests/read.cpp
        tests/write.cpp
        tests/convert.cpp
    )

    target_link_libraries(plantuml-to-cpp_tests
        PRIVATE
            plantuml-to-cpp-lib
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(plantuml-to-cpp_tests)
endif()

# ---- Installation ----
install(TARGETS plantuml-to-cpp
    RUNTIME DESTINATION bin
)
