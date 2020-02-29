#	Alan Witkowski - CMake module to find SDL2_ttf
#
#	Input:
#       SDL2_ROOT - Environment variable that points to the sdl2 root directory
#
#	Output:
#       SDL2_TTF_FOUND - Set to true if SDL2_ttf was found
#       SDL2_TTF_INCLUDE_DIRS - Path to SDL2_ttf.h
#       SDL2_TTF_LIBRARIES - Contains the list of SDL2_ttf libraries
#

set(SDL2_TTF_FOUND false)

# find include path
find_path(
		SDL2_TTF_INCLUDE_DIRS
	NAMES
		SDL_ttf.h
	HINTS
		ENV SDL2_TTF_ROOT
	PATHS
		/usr
		/usr/local
	PATH_SUFFIXES
		include
		SDL2
)

# find library
find_library(
		SDL2_TTF_LIBRARIES SDL2_ttf
	HINTS
		ENV SDL2_TTF_ROOT
	PATHS
		/usr/lib
		/usr/local/lib
	#PATH_SUFFIXES
)

# handle QUIET and REQUIRED
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_ttf REQUIRED_VARS SDL2_TTF_LIBRARIES SDL2_TTF_INCLUDE_DIRS)

# advanced variables only show up in gui if show advanced is turned on
mark_as_advanced(SDL2_TTF_INCLUDE_DIRS SDL2_TTF_LIBRARIES)
