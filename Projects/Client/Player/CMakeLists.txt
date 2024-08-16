create_resources(Resource ${CMAKE_BINARY_DIR}/Resource/Player.hpp)

set(CMAKE_AUTOMOC ON)

list(APPEND SOURCE 
    Source/main.cpp
    Source/MainWindow.cpp
)

list(APPEND HEADER 
    ${CMAKE_BINARY_DIR}/Resource/Player.hpp
    Header/MainWindow.hpp
)

if(WIN32)
    list(APPEND HEADER Resource/Windows/Script.rc)
endif()

qt_add_executable(Player ${SOURCE} ${HEADER})

target_include_directories(Player PRIVATE Header)
target_link_libraries(Player PRIVATE Common Engine)

set_target_properties(Player PROPERTIES OUTPUT_NAME "RNR.Player")

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(Player PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
    )
endif()