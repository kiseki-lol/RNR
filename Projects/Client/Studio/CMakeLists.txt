create_resources(Resource ${CMAKE_BINARY_DIR}/Resource/Studio.hpp)

set(CMAKE_AUTOMOC ON)

list(APPEND SOURCE
    Source/main.cpp
    Source/MainWindow.cpp
    Source/PropertyViewer.cpp
)

list(APPEND HEADER
    ${CMAKE_BINARY_DIR}/Resource/Studio.hpp
    Header/MainWindow.hpp
    Header/PropertyViewer.hpp
)

if(WIN32)
    list(APPEND HEADER Resource/Windows/Script.rc)
endif()

qt_add_executable(Studio ${SOURCE} ${HEADER})

target_include_directories(Studio PRIVATE Header)
target_link_libraries(Studio PRIVATE Common Engine)

set_target_properties(Studio PROPERTIES OUTPUT_NAME "RNR.Studio")

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(Studio PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
    )
endif()