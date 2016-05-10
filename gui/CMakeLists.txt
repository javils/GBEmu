find_package(Qt5Widgets)
find_package(SDL2 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lSDL2 -lSDL2main")

include_directories(${GLOBAL_INC_FILES} ${SDL_INCLUDE_DIRS} ${INCLUDE_BLARGG_SOUND})

add_executable(GBGui MACOSX_BUNDLE main.cpp
        MainWindow.cpp
        RenderThread.cpp
        RenderThread.h)

target_link_libraries(GBGui GBEmu BlarggSound ${SDL_LIBRARY})

set_target_properties(GBGui PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "GBGui")
set_target_properties(GBGui PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/AppleInfo.plist")

qt5_use_modules(GBGui Core Gui Widgets)
