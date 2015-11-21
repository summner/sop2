project(sop2)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")

  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        LIST(APPEND dirlist ${child})
    ENDIF()

  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

FOREACH(subdir ${SUBDIRS})

    IF((NOT "${subdir}" STREQUAL ".git") AND (NOT "${subdir}" MATCHES "[Cc][Mm]ake.*"))
        AUX_SOURCE_DIRECTORY(${subdir} ${subdir}_LIST)
        MESSAGE("${${subdir}_LIST}")
        ADD_EXECUTABLE("${subdir}.run" "${${subdir}_LIST}")
    ENDIF()

ENDFOREACH()


SET(CMAKE_CXX_FLAGS “${CMAKE_CXX_FLAGS} -Wall -Werror”)
