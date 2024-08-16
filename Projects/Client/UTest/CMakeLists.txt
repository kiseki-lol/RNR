project(UTest)

qt_add_executable(UTest
    Source/main.cpp
)

target_include_directories(UTest PRIVATE Header)
target_link_libraries(UTest PRIVATE Common Engine)
set_target_properties(UTest PROPERTIES OUTPUT_NAME "RNR.UTest")

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(UTest PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
    )
endif()