CC = gcc
CFLAGS = -Iinclude -O2 -flto
LDFLAGS = -s -flto
LIBS =
SRCDIR = src
OBJDIR = obj
BINDIR = output
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

ifeq ($(OS),Windows_NT)
EXEEXT = .exe
else
EXEEXT =
endif

MKDIR_P = mkdir -p
RM = rm -rf

TARGET := $(BINDIR)/uwuify$(EXEEXT)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(MKDIR_P) $(BINDIR)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
	strip $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(MKDIR_P) $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJDIR) $(BINDIR)

clean_obj:
	$(RM) $(OBJDIR)

.PHONY: all clean clean_obj
