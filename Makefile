LIBPS4 := ps4-payload-sdk/libPS4

CC := gcc
OBJCOPY := objcopy

ODIR := build
SDIR := source
IDIRS := -I$(LIBPS4)/include -Iinclude

CFILES := $(shell find $(SDIR) -type f -name '*.c')
SFILES := $(shell find $(SDIR) -type f -name '*.s')
MAPFILE := $(shell basename "$(CURDIR)").map
CFLAGS := $(IDIRS) -Os -std=c11 -ffunction-sections -fdata-sections -fno-builtin -nostartfiles -nostdlib -Wall -Wextra -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small -fpie -fPIC
LFLAGS := -L$(LIBPS4) -Xlinker -T $(LIBPS4)/linker.x -Xlinker -Map="$(MAPFILE)" -Wl,--build-id=none -Wl,--gc-sections -lPS4

OBJS := $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES))
TARGET := $(shell basename "$(CURDIR)").bin

$(TARGET): $(ODIR) $(OBJS)
	cd $(LIBPS4) && $(MAKE)
	$(CC) $(LIBPS4)/crt0.s $(OBJS) -o temp.t $(CFLAGS) $(LFLAGS)
	$(OBJCOPY) -O binary temp.t "$(TARGET)"
	rm -f temp.t

$(ODIR)/%.o: $(SDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	cd $(LIBPS4) && $(MAKE) clean
	rm -rf "$(TARGET)" "$(MAPFILE)" $(ODIR)
