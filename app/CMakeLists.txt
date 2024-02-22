# Build the app, using the HAL

include_directories(include)
file(GLOB MY_SOURCES "src/*.cpp")
add_executable(light_sampler ${MY_SOURCES})

# Make use of the HAL library
target_link_libraries(light_sampler LINK_PRIVATE hal)

# Copy executable to final location (change `light_sampler` to project name as needed)
add_custom_command(TARGET light_sampler POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:light_sampler>"
     "~/cmpt433/public/" 
  COMMENT "Copying ARM executable to public NFS directory")
