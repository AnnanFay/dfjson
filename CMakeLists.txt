PROJECT (dfjson)

# Generate the encoding files
#execute_process(
#    COMMAND cd ../plugins/dgc
#    COMMAND java -cp lib/clojure-1.3.0.jar clojure.main src/dfjson/core.clj ../../library/include/df/codegen.out.xml
#    COMMAND CD ../../build)

# WTF
include_directories (
    .
    include/
    ${CMAKE_SOURCE_DIR}
)

# A list of source files
SET(PROJECT_SRCS
    encode-df.cpp
    encode.cpp
)
## A list of headers
SET(PROJECT_HDRS
    df-headers.h
    encode-df.h
    encode.h
)

SET_SOURCE_FILES_PROPERTIES( ${PROJECT_HDRS} PROPERTIES HEADER_FILE_ONLY TRUE)

SET(PROJECT_LIBS
    ${PROJECT_LIBS}
)

# mash them together (headers are marked as headers and nothing will try to compile them)
LIST(APPEND PROJECT_SRCS ${PROJECT_HDRS})

#linux
IF(UNIX)
    add_definitions(-DLINUX_BUILD)
    SET(PROJECT_LIBS
        # add any extra linux libs here
        ${PROJECT_LIBS}
    )
# windows
ELSE(UNIX)
    SET(PROJECT_LIBS
        # add any extra linux libs here
        ${PROJECT_LIBS}
        $(NOINHERIT)
    )
ENDIF(UNIX)

# this makes sure all the stuff is put in proper places and linked to dfhack
# DFHACK_PLUGIN(dfjson ${PROJECT_SRCS} LINK_LIBRARIES ${PROJECT_LIBS})

#install(DIRECTORY gui/
#        DESTINATION "dgc"
#        PATTERN "CVS" EXCLUDE
#        PATTERN ".svn" EXCLUDE
#        PATTERN ".git" EXCLUDE)

