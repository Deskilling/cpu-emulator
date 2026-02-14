PROJECT  := cpu-emulator
BUILDDIR := build
OBJDIR   := obj
SRCDIR   := src

CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra -I. -MMD -MP
LDFLAGS := -lm

SRCS := $(wildcard $(SRCDIR)/**/*.c $(SRCDIR)/*.c)
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS := $(OBJS:.o=.d)

TARGET := $(BUILDDIR)/$(PROJECT)

.PHONY: all clean

all: $(TARGET)

$(BUILDDIR) $(OBJDIR):
	mkdir -p $@

$(TARGET): $(BUILDDIR) $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(OBJDIR)

-include $(DEPS)
