create_resources(Resource ${CMAKE_BINARY_DIR}/Resource/Server.hpp)

list(APPEND SOURCE Source/main.cpp)
list(APPEND HEADER ${CMAKE_BINARY_DIR}/Resource/Server.hpp)

if(WIN32)
    list(APPEND HEADER Resource/Windows/Script.rc)
endif()

add_executable(Server 
    Header/ThumbnailGenerator.hpp
    Source/ThumbnailGenerator.cpp
    ${SOURCE} ${HEADER})

target_include_directories(Server PRIVATE Header)
target_link_libraries(Server PRIVATE Engine)

set_target_properties(Server PROPERTIES OUTPUT_NAME "RNR.Server")