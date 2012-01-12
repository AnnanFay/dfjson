PROJECT (dgc)

# WTF
include_directories (
    .
    include/
    include/boost_1_48_0/
    include/json_spirit/
    ${CMAKE_SOURCE_DIR}
)

# A list of source files
SET(PROJECT_SRCS
    encode.cpp
    encode-df.cpp
    dgc.cpp
)
# A list of headers
SET(PROJECT_HDRS
    extract.h
    df-headers.h
    encode.h
    encode-df.h
    dgc.h
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
DFHACK_PLUGIN(dgc ${PROJECT_SRCS} LINK_LIBRARIES ${PROJECT_LIBS})

install(DIRECTORY gui/
        DESTINATION "dgc"
        PATTERN "CVS" EXCLUDE
        PATTERN ".svn" EXCLUDE
        PATTERN ".git" EXCLUDE)

