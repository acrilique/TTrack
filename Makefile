# Project Name
TARGET = TTrack

# C++ Standard
# CXXFLAGS += -std=gnu++2a

# Sources and headers
CPP_SOURCES = $(wildcard src/*.cpp)

# Optimization level
# OPT = -O3

# Library Locations
LIBDAISY_DIR = ../DaisyExamples/libDaisy/
DAISYSP_DIR = ../DaisyExamples/DaisySP/
# ETL_DIR = ./lib/etl/include

# C_INCLUDES += -I $(ETL_DIR)

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
