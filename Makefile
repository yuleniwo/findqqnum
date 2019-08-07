.PHONY: clean cleanobj

empty :=
CROSS_COMPILE	:= 
ANDROID := $(shell echo $(CROSS_COMPILE) | grep -i 'android')
CC			:= $(CROSS_COMPILE)gcc
CFLAGS		:= -D NDEBUG -Wall -O2 -Wno-unused -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
LD			:= $(CROSS_COMPILE)gcc
ifeq ($(ANDROID), )
LDFLAGS		:= -lm -lrt -lpthread -ldl -L.
else
LDFLAGS		:= -lm -pthread -ldl -L.
endif

OBJ_DIR		:= ./$(CROSS_COMPILE)obj
BIN_DIR		:= ./$(CROSS_COMPILE)bin
TARGET_APP	:= $(BIN_DIR)/findqqnum

$(shell mkdir -p $(OBJ_DIR) || 0)
$(shell mkdir -p $(BIN_DIR) || 0)

vpath %.c src

C_SRCS		:= $(notdir $(wildcard ./src/*.c))
C_OBJS		:= $(C_SRCS:%.c=$(OBJ_DIR)/%.o)
C_DEPS		:= $(C_OBJS:%.o=%.d)
EXISTS_DEPS	:= $(wildcard $(C_DEPS))

all: $(C_DEPS) $(TARGET_APP)
	@echo done.

$(C_DEPS): $(OBJ_DIR)/%.d:%.c
	$(CC) -c $(CFLAGS) -Wp,-MMD,$@ -MT $(@:%.d=%.o) -MT $@ -o $(@:%.d=%.o) $<

$(TARGET_APP): $(C_OBJS)
	@echo "---- Build : $@ ----"
	$(LD) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(OBJ_DIR)/*.d
	rm -f $(TARGET_APP)

cleanobj:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(OBJ_DIR)/*.d

ifeq ($(findstring $(MAKECMDGOALS), clean cleanobj),)
ifneq ($(EXISTS_DEPS),)
sinclude $(EXISTS_DEPS)
endif
endif
