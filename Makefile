TARGET_EXEC := final_program

BUILD_DIR := ./build
SRC_DIRS := ./

SRCS := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.s' -not -path './test/*')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d) 
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# GTK3 flags
GTK3_CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
GTK3_LDFLAGS := $(shell pkg-config --libs gtk+-3.0)

CJSON_CFLAGS := $(shell pkg-config --cflags libcjson)
CJSON_LDFLAGS := $(shell pkg-config --libs libcjson)

CPPFLAGS := $(INC_FLAGS) $(GTK3_CFLAGS) $(CJSON_CFLAGS) -MMD -MP
LDFLAGS := $(GTK3_LDFLAGS) $(CJSON_LDFLAGS)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)
