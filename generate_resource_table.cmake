find_program(PYTHON_PROGRAM python python3)
if(PYTHON_PROGRAM)
  if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/resource_table.h
	  OR NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/resource_table.c)
	if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/gen/resource_table.h
		OR NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/gen/resource_table.c)
	  message("-- Generating resource table")
	  execute_process(
		COMMAND ${PYTHON_PROGRAM} generate_resource_table.py
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/gen
	  )
	endif()
	  message("-- Copying resource table from gen/ to src/")
	execute_process(
	  COMMAND ${CMAKE_COMMAND} -E copy gen/resource_table.h src/
	  COMMAND ${CMAKE_COMMAND} -E copy gen/resource_table.c src/
	  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
	)
  endif()
else()
  message(
	FATAL_ERROR
	"ERROR: Could not find python on system; it is necessary to generate a resource table."
  )
endif()
