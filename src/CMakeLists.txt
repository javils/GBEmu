# Header files are on include/ folder.
include_directories(${GLOBAL_INC_FILES})

# Add all the files .c or .cpp here.
set(SRC_SOURCE_FILES CPU/Z80.cpp
        CPU/Bus.cpp
        CPU/Instructions.cpp
        Memory/MemoryDMG.cpp
        Memory/Cartridge.cpp
        Memory/MBC/MBC1.cpp
        Memory/IOHandlerDMG.cpp
        Timer/Timer.cpp
        Video/GPUDMG.cpp)

# Build library using header and source files.
add_library(GBEmu SHARED ${SRC_SOURCE_FILES} ${GLOBAL_INC_FILES})

# Prepare library to Java Native Interface (JNI)
if(APPLE)
    set_target_properties(GBEmu PROPERTIES PREFIX "lib" SUFFIX ".jnilib")
elseif(WIN32)
    set_target_properties(GBEmu PROPERTIES PREFIX "" SUFFIX ".dll")
elseif(UNIX)
    set_target_properties(GBEmu PROPERTIES PREFIX "lib" SUFFIX ".so")
ENDIF(APPLE)