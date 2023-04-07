cmake_minimum_required(VERSION 3.24)
set(COURSE_NAME "CG2")
project(${COURSE_NAME} C CXX)

# pre-set CGV Framework options
option(CGV_BUILD_EXAMPLES off)
option(CGV_BUILD_WITH_AUDIO off)

# Include the CGV Framework
if (DEFINED ENV{CGV_DIR})
	# Use system-installed CGV Framework if present
	set(CGV_DIR $ENV{CGV_DIR})
	message("Using CGV Framework source found at:")
	message("> ${CGV_DIR}")
	add_subdirectory(${CGV_DIR} ./framework)
else()
	# Use CGV Framework bundled in form of the Git submodule
	add_subdirectory(${CMAKE_SOURCE_DIR}/framework)
endif()

# Add exercises
# - configure for common root
set(CG2_ROOT_DIR ${CMAKE_SOURCE_DIR})
# - add exercises (FIXME: in reverse order so the debug targets appear in correct order in VS Code)
add_subdirectory(exercise45)
add_subdirectory(exercise2)
add_subdirectory(exercise1)

# Visual Studio fluff
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CG2_exercise1)
