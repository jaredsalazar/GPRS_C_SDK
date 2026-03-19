## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##

# Name of the module
LOCAL_NAME := app

# List of submodules which contain code we need to include in the final lib
LOCAL_API_DEPENDS := \

LOCAL_ADD_INCLUDE := include\
                    include/std_inc \
                    include/api_inc \
                    app/include \
                    libs/gps/include \
                    libs/gps/minmea/src \
                    $(shell find app/include -type d 2>/dev/null) \


# Set this to any non-null string to signal a module which 
# generates a binary (must contain a "main" entry point). 
# If left null, only a library will be generated.
IS_ENTRY_POINT := no

## ------------------------------------ ##
## 	Add your custom flags here          ##
## ------------------------------------ ##
MYCFLAGS += 

## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##
S_SRC := $(shell find src -type f -name '*.S' 2>/dev/null | sed 's,^src/,,')
C_SRC := $(shell find src -type f -name '*.c' 2>/dev/null | sed 's,^src/,,')
CXX_SRC := $(shell find src -type f -name '*.cpp' 2>/dev/null | sed 's,^src/,,')

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/platform/compilation/cust_rules.mk
