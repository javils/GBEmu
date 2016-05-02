# Set src and include
set(SRC_BLARGG_SOUND
        Blip_Buffer.cpp
        Gb_Apu.cpp
        Gb_Oscs.cpp
        Multi_Buffer.cpp)

set(INCLUDE_BLARGG_SOUND
        blargg_common.h
        blargg_source.h
        Blip_Buffer.h
        Blip_Synth.h
        Gb_Apu.h
        Gb_Oscs.h
        Multi_Buffer.h)

# Build library using header and source files.
add_library(BlarggSound SHARED ${SRC_BLARGG_SOUND} ${INCLUDE_BLARGG_SOUND})

# Prepare library
if (APPLE)
    set_target_properties(BlarggSound PROPERTIES PREFIX "lib" SUFFIX ".jnilib")
elseif (WIN32)
    set_target_properties(BlarggSound PROPERTIES PREFIX "" SUFFIX ".dll")
elseif (UNIX)
    set_target_properties(BlarggSound PROPERTIES PREFIX "lib" SUFFIX ".so")
ENDIF (APPLE)