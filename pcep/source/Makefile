# ARM architecture is detected
ifeq ($(TARGET),arm)
	CC=arm-linux-gnueabi-g++
	CFLAGS= -Wall -g -std=gnu++11 -DNO_SSL -fPIC
	ARCH= arm
else 
	CC=g++
	CFLAGS= -Wall -g -std=c++11 -DNO_SSL -fPIC
	ARCH= x86_64
endif

CFLAGS+= $(EXTRA_FLAGS)
LFLAGS= -L /usr/lib -pthread

MAIN= bcep

#Main.cpp of ScriptCompiler is only used for testing/dev purposes
SRCS:= $(shell find src \( -name "*.cpp" ! -name "Main.cpp" \))
OBJ_DIR_ROOT=Bin
OBJ_DIR=$(OBJ_DIR_ROOT)/${ARCH}
OBJS:= $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all
all: $(MAIN)
	@echo $(SUBDIRS)
	
.PHONY: udp
udp: all

.PHONY: mqtt
	
mqtt: CFLAGS+= -DMQTT
mqtt: LFLAGS+= -lmosquitto
mqtt: all

.PHONY: debug

debug: CFLAGS+= -DCEP_DEBUG
debug: all

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LDFLAGS)

$(OBJ_DIR)/%.o:%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: test

test: all
	@python test/regtest.py ./bcep test/	

.PHONY: clean
clean:
	$(RM) -rf $(OBJ_DIR_ROOT)
	
.PHONY: distclean
distclean: clean
	$(RM) -rf $(MAIN)
