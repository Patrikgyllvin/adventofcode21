RTDIR = .
SRCDIR = $(RTDIR)/src
INCDIR = $(RTDIR)/include
BINDIR = $(RTDIR)/bin
OBJDIR = $(RTDIR)/obj

CXX = g++

CXXFLAGS = --std=c++17 -Wall -O3 -I$(INCDIR)
LDFLAGS =

SOURCES := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
INCLUDES := $(wildcard $(INCDIR)/**/*.h) $(wildcard $(INCDIR)/**/*.hpp) 
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

.PHONY: all $(BINDIR)/% clean

all: $(OBJECTS)

.SECONDEXPANSION:
$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $$(dir $$@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR)/%.out: $(OBJDIR)/%.o $(OBJDIR)/main.o | $(BINDIR)
	if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	$(CXX) $(LDFLAGS) $^ -o $@

.SECONDARY:
$(BINDIR)/%: $(BINDIR)/%.out
	$< $(SRCDIR)/$(*D)/input.in

clean:
	rm -rf $(BINDIR) $(OBJDIR)

$(OBJDIR):
	mkdir -p $@

$(OBJDIR)/%:
	mkdir -p $@

$(BINDIR):
	mkdir -p $@