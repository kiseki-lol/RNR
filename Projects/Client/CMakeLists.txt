include_directories(${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(FORCE_ENABLE_VSYNC ON)

if(COMPILE_PLAYER OR COMPILE_STUDIO OR COMPILE_UTEST)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)
    set(QT6_LIBRARIES_INCL Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets Qt6::GuiPrivate)
    
    add_subdirectory(Common)
    
    if(FORCE_ENABLE_VSYNC)
        target_compile_definitions(Common PUBLIC FORCE_ENABLE_VSYNC)
    endif()

    if(COMPILE_PLAYER)
        add_subdirectory(Player)
    endif()
    
    if(COMPILE_STUDIO)
        add_subdirectory(Studio)
    endif()

    if(COMPILE_UTEST)
        add_subdirectory(UTest)
    endif()

    if(NOT CI)
        file(COPY ${CMAKE_SOURCE_DIR}/Content/RNR/ DESTINATION ${CMAKE_BINARY_DIR}/content)
        if(EXISTS ${CMAKE_SOURCE_DIR}/Content/Roblox)
            file(COPY ${CMAKE_SOURCE_DIR}/Content/Roblox/ DESTINATION ${CMAKE_BINARY_DIR}/content)
        endif()
    endif()
endif()

if(COMPILE_SERVER)
    add_subdirectory(Server)
endif()
