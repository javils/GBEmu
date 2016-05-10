find_package(SDL2 REQUIRED)

# Set src and include
set(SRC_BLARGG_SOUND
        Blip_Buffer.cpp
        Gb_Apu.cpp
        Gb_Oscs.cpp
        Multi_Buffer.cpp
        Sound_Queue.cpp)

set(INCLUDE_BLARGG_SOUND
        blargg_common.h
        blargg_source.h
        Blip_Buffer.h
        Blip_Synth.h
        Gb_Apu.h
        Gb_Oscs.h
        Multi_Buffer.h
        Sound_Queue.h)
include_directories(${SDL_INCLUDE_DIRS})
# Build library using header and source files.
add_library(BlarggSound STATIC ${SRC_BLARGG_SOUND} ${INCLUDE_BLARGG_SOUND})
target_link_libraries(BlarggSound ${SDL_LIBRARY})


# Prepare library
if (APPLE)
    set_target_properties(BlarggSound PROPERTIES PREFIX "lib" SUFFIX ".jnilib")
elseif (WIN32)
    set_target_properties(BlarggSound PROPERTIES PREFIX "" SUFFIX ".dll")
elseif (UNIX)
    set_target_properties(BlarggSound PROPERTIES PREFIX "lib" SUFFIX ".so")
ENDIF (APPLE)