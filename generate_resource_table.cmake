find_program(PYTHON_PROGRAM python python3)
if(PYTHON_PROGRAM)
  if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/resource_table.h
	  OR NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/resource_table.c)
	if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/gen/resource_table.h
		OR NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/gen/resource_table.c)
	  message(STATUS "Generating resource table")
	  execute_process(
		COMMAND
		${PYTHON_PROGRAM}
		generate_resource_table.py
		--config ${CONFIG_FILE}
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gen
		COMMAND_ERROR_IS_FATAL ANY
	  )
	endif()
	message(STATUS "Copying resource table from gen/ to src/")
	execute_process(
	  COMMAND ${CMAKE_COMMAND} -E copy gen/resource_table.h src/
	  COMMAND ${CMAKE_COMMAND} -E copy gen/resource_table.c src/
	  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
	  COMMAND_ERROR_IS_FATAL ANY
	)
  endif()
else()
  message(
	FATAL_ERROR
	"Could not find python on system; it is necessary to generate a resource table.
https://www.python.org/downloads/"
  )
endif()
