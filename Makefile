############################################################################# 
# 
# Generic Makefile Template for C/C++ Projects 
# 
# License: GPL (General Public License) 
# Note: GPL only applies to this file; you can use this Makefile in non-GPL projects. 
# Author: $PFX
# Date: 2026/01/03 (version 0.8)
# Author: Pear <service AT pear DOT hk> 
# Date: 2016/04/26 (version 0.7) 
# Author: kevin1078 <kevin1078 AT 126 DOT com> 
# Date: 2012/04/24 (version 0.6) 
# Author: whyglinux <whyglinux AT gmail DOT com> 
# Date: 2008/04/05 (version 0.5) 
# 2007/06/26 (version 0.4) 
# 2007/04/09 (version 0.3) 
# 2007/03/24 (version 0.2) 
# 2006/03/04 (version 0.1) 
#===========================================================================
#
# Changes:
# - Object files go to ./obj/
# - Dependency files go to ./obj/
# - Directory structure preserved
# - Executable remains in project root
#
#############################################################################

PROGRAM = EngK

CXX = g++

OBJDIR = obj

EXTRA_CFLAGS = -fdata-sections -ffunction-sections

EXTRA_LDFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

INCLUDE =
CPPFLAGS = -Wall -Wextra $(INCLUDE)

LDFLAGS =

SRCDIRS :=

EXTRA_CFLAGS_MACOS =
EXTRA_LDFLAGS_MACOS = -Wl,-search_paths_first -Wl,-dead_strip
LDFLAGS_MACOS =

EXTRA_CFLAGS_LINUX =
EXTRA_LDFLAGS_LINUX = -Wl,--gc-sections -Wl,--strip-all
LDFLAGS_LINUX =

EXTRA_CFLAGS_WINDOWS =
EXTRA_LDFLAGS_WINDOWS =
LDFLAGS_WINDOWS =

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	EXTRA_CFLAGS  += $(EXTRA_CFLAGS_MACOS)
	EXTRA_LDFLAGS += $(EXTRA_LDFLAGS_MACOS)
	LDFLAGS       += $(LDFLAGS_MACOS)
else ifeq ($(UNAME_S),Linux)
	EXTRA_CFLAGS  += $(EXTRA_CFLAGS_LINUX)
	EXTRA_LDFLAGS += $(EXTRA_LDFLAGS_LINUX)
	LDFLAGS       += $(LDFLAGS_LINUX)
else
	EXTRA_CFLAGS  += $(EXTRA_CFLAGS_WINDOWS)
	EXTRA_LDFLAGS += $(EXTRA_LDFLAGS_WINDOWS)
	LDFLAGS       += $(LDFLAGS_WINDOWS)
endif

SRCEXTS = .c .cpp
HDREXTS = .h .hpp

CFLAGS   = -O3
CXXFLAGS = -O3

RM = rm -f

ifeq ($(SRCDIRS),)
	SRCDIRS := $(shell find . -type d)
endif

SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(SRCEXTS))))
HEADERS = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)/*,$(HDREXTS))))

OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(filter %.cpp,$(SOURCES))) \
       $(patsubst %.c,$(OBJDIR)/%.o,$(filter %.c,$(SOURCES)))

DEPS = $(OBJS:.o=.d)

COMPILE.c   = $(CXX) $(EXTRA_CFLAGS) $(CFLAGS)   $(CPPFLAGS) -c
COMPILE.cxx = $(CXX) $(EXTRA_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c
LINK.cxx    = $(CXX) $(EXTRA_CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)

.PHONY: all clean distclean

all: $(PROGRAM)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	mkdir -p $(dir $@)
	$(COMPILE.cxx) $< -o $@

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(COMPILE.c) $< -o $@

$(OBJDIR)/%.d: %.cpp | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CXX) -MM $(CPPFLAGS) $< > $@

$(OBJDIR)/%.d: %.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CXX) -MM $(CPPFLAGS) $< > $@

$(PROGRAM): $(OBJS)
	$(LINK.cxx) $(OBJS) $(EXTRA_LDFLAGS) -o $@

-include $(DEPS)

clean:
	$(RM) -r $(OBJDIR) $(PROGRAM) $(PROGRAM).exe

distclean: clean
