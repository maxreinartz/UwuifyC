CC = gcc
CFLAGS = -Iinclude -Os -s
LDFLAGS = -s
LIBS =
SRCDIR = src
OBJDIR = obj
BINDIR = output
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

ifeq ($(OS),Windows_NT)
EXEEXT = .exe
define MKDIR_P
if not exist $(1) mkdir $(1)
endef
RM = rmdir /S /Q
else
EXEEXT =
define MKDIR_P
mkdir -p $(1)
endef
RM = rm -rf
endif

TARGET := $(BINDIR)/uwuify$(EXEEXT)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(call MKDIR_P,$(BINDIR))
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
	strip $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(call MKDIR_P,$(OBJDIR))
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJDIR) $(BINDIR)

clean_obj:
	$(RM) $(OBJDIR)

.PHONY: all clean clean_obj
