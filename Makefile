PROJECT  := cpu-emulator

BUILDDIR := build
OBJDIR   := obj
SRCDIR   := src

CC       := gcc
CFLAGS   := -std=c99 -Wall -Wextra -I. -MMD -MP
LDFLAGS  := -lm

SRCS := $(shell find $(SRCDIR) -name "*.c")
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS := $(OBJS:.o=.d)

TARGET := $(BUILDDIR)/$(PROJECT)

.PHONY: all clean

all: $(TARGET)

$(BUILDDIR):
	mkdir -p $@

$(TARGET): $(OBJS) | $(BUILDDIR)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(OBJDIR)

-include $(DEPS)
