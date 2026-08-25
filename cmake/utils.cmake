#
# Helper functions and utils for CMake
#

include(CMakeParseArguments)

#
# Define a grouping for source files for a given target based on real file system layout.
# Very usefull for IDE project generation (mainly in XCode and MSVC).
#

macro(group_sources_impl)
    source_group(TREE ${root} FILES ${sources})
endmacro()

function(group_sources target root)
    get_target_property(sources ${target} SOURCES)
    group_sources_impl()
endfunction()

function(group_sources_for_files sources root)
    group_sources_impl()
endfunction()

#
# Organizes targets into a folders in an IDE.
#
function(set_target_folder target folder)
    set_target_properties(${target} PROPERTIES FOLDER ${folder})
endfunction()