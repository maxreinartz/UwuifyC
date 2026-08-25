CC = gcc
CFLAGS = -Iinclude -Os -s
LDFLAGS = -s
LIBS =
SRCDIR = src
OBJDIR = obj
BINDIR = output
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET := $(BINDIR)/uwuify.exe

all: $(TARGET)

$(TARGET): $(OBJECTS)
	if not exist $(BINDIR) mkdir $(BINDIR)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)
	strip $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	if exist $(OBJDIR) rmdir /S /Q $(OBJDIR)
	if exist $(BINDIR) rmdir /S /Q $(BINDIR)

clean_obj:
	if exist $(OBJDIR) rmdir /S /Q $(OBJDIR)

.PHONY: all clean clean_obj
