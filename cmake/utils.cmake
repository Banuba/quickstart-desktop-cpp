#
# Helper functions and utils for CMake
#

include(CMakeParseArguments)

#
# Define a grouping for source files for a given target based on real file system layout.
# Very usefull for IDE project generation (mainly in XCode and MSVC).
#

macro(group_sources_impl)
    # Делаем группировку в CMake 3.8 или старше т.к только в этих версиях полявилось ключевое слово TREE.
    # Без него делать автоматическую группировку гораздо сложнее.

    # We can't use GREATER_EQUAL comparison operator because it appears only in CMake version 3.7 and above.
    # So, some crappy code needed.

    set(maj ${CMAKE_MAJOR_VERSION})
    set(min ${CMAKE_MINOR_VERSION})

    if(maj GREATER 3 OR maj EQUAL 3)
        set(maj_cond TRUE)
    else()
        set(maj_cond FALSE)
    endif()

    if(min GREATER 8 OR min EQUAL 8)
        set(min_cond TRUE)
    else()
        set(min_cond FALSE)
    endif()

    if(maj_cond AND min_cond)
        source_group(TREE ${root} FILES ${sources})
    else()
        # Выводим предупрежедения об отсутсвии группировки только для генераторов в которых это актуально.
        # Например в Android NDK используется CMake 3.6, но там это предупреждение не имеет значения.
        # TODO(a_cherkes): убрать, когда верси под Android повысится
        if(MSVC OR XCODE)
            message(WARNING "Your CMake version doesn't support source grouping. Can't group sources for target ${target}. Consider to use CMake 3.8 or higher.")
        endif()
    endif()
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