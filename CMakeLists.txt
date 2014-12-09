cmake_minimum_required(VERSION 2.8)

project(GDNjam)
	
SET(OPIFEX_REPOSITORY ${PROJECT_SOURCE_DIR})
SET(OPIFEX_ENGINE_REPOSITORY CACHE STRING "../OPifex.Engine/")

include(CMake/application.cmake)


#########################################
# GDNjam - LAYER 9 - Application
#########################################

# Include Directories
include_directories(
	${OPIFEX_REPOSITORY}
	.
	../
)
add_external_opifex_includes()


# Include Files
add_source_group(\\ "${PROJECT_SOURCE_DIR}/" APPLICATION_SRC)
add_source_group(include "${PROJECT_SOURCE_DIR}/include" APPLICATION_SRC)
add_source_group(include/states "${PROJECT_SOURCE_DIR}/include/states" APPLICATION_SRC)
add_source_group(include/entities "${PROJECT_SOURCE_DIR}/include/entities" APPLICATION_SRC)
add_source_group(include/controllers "${PROJECT_SOURCE_DIR}/include/controllers" APPLICATION_SRC)

# Source Files
add_source_group(src "${PROJECT_SOURCE_DIR}/src" APPLICATION_SRC)
add_source_group(src/states "${PROJECT_SOURCE_DIR}/src/states" APPLICATION_SRC)
add_source_group(src/entities "${PROJECT_SOURCE_DIR}/src/entities" APPLICATION_SRC)
add_source_group(src/controllers "${PROJECT_SOURCE_DIR}/src/controllers" APPLICATION_SRC)

# BUILD
add_executable(8_Application ${APPLICATION_SRC})
add_opifex_definitions(8_Application "")
set_target_properties(8_Application PROPERTIES OUTPUT_NAME "Application")
add_opifex_libraries(8_Application)


opifex_engine_status_messages()