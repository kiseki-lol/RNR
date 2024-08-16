# Luau
set(LUAU_BUILD_CLI OFF CACHE BOOL "" FORCE)
set(LUAU_BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(${DEPENDENCIES_DIR}/Luau ${CMAKE_BINARY_DIR}/Dependencies/Luau)

# RNR

add_subdirectory(Engine)
add_subdirectory(Client)