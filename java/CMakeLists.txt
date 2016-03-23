# Find and use Java package
find_package(Java REQUIRED)
include(UseJava)

# Compile .jar file. Add all the .java files here
add_jar(GBGui GBGui.java)

# Get .jar path.
get_target_property(_jarFile GBGui JAR_FILE)

# Add custom command "make run" to run the java application.
add_custom_target(run COMMAND ${Java_JAVA_EXECUTABLE} -cp ${_jarFile} -Djava.library.path=${CMAKE_LIBRARY_OUTPUT_DIRECTORY} GBGui)