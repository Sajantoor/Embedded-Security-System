# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.cpp")
add_executable(proj ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(proj LINK_PRIVATE hal)

# Copy executable to final location (change `proj` to project name as needed)
add_custom_command(TARGET proj POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:proj>"
     "~/cmpt433/public/" 
  COMMENT "Copying ARM executable to public NFS directory")
