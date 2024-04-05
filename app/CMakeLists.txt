# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.cpp")
add_executable(security_project ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(security_project LINK_PRIVATE hal)

# Copy executable to final location (change `security_project` to security_projectect name as needed)
add_custom_command(TARGET security_project POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:security_project>"
     "~/cmpt433/public/" 
  COMMENT "Copying ARM executable to public NFS directory")

