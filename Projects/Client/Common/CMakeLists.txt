list(APPEND SOURCE
    Source/GL/Adorn.cpp
    Source/OgreWidget.cpp
    Source/QtInputManager.cpp
    Source/SettingsManager.cpp
)

list(APPEND HEADER
    Header/GL/Adorn.hpp
    Header/GL/RenderContext.hpp
    Header/OgreWidget.hpp
    Header/QtInputManager.hpp
    Header/SettingsManager.hpp
)

qt_standard_project_setup()
qt_add_library(Common STATIC ${SOURCE} ${HEADER})

target_include_directories(Common PUBLIC Header)
target_link_libraries(Common PUBLIC ${QT6_LIBRARIES_INCL} Engine)