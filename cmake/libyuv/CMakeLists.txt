# CMakeLists for libyuv
# Originally created for "roxlu build system" to compile libyuv on windows
# Run with -DTEST=ON to build unit tests

PROJECT ( YUV C CXX )	# "C" is required even for C++ projects
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
OPTION( TEST "Built unit tests" OFF )

SET ( ly_base_dir	${PROJECT_SOURCE_DIR} )
SET ( ly_src_dir	${ly_base_dir}/source )
SET ( ly_inc_dir	${ly_base_dir}/include )
SET ( ly_lib_name	yuv )
SET ( ly_lib_static	${ly_lib_name} )

FILE ( GLOB_RECURSE	ly_source_files ${ly_src_dir}/*.cc )
LIST ( SORT			ly_source_files )

INCLUDE_DIRECTORIES( BEFORE ${ly_inc_dir} )

ADD_LIBRARY				( ${ly_lib_static} STATIC ${ly_source_files} )