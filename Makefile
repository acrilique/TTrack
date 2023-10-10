# Project Name
TARGET = TTrack

# Sources and headers
CPP_SOURCES = $(wildcard src/*.cpp)

# Library Locations
LIBDAISY_DIR = ../DaisyExamples/libDaisy/
DAISYSP_DIR = ../DaisyExamples/DaisySP/
ETL_DIR = ./lib/etl/include

C_INCLUDES += -I $(ETL_DIR)

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
