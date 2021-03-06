TARGET1 = main
TARGET2 = loop
TARGET3 = robot_main
SRC1 = Action.cpp mainBasic.cpp
SRC2 = raspberry_booting.cpp
OBJECTS1 = $(addprefix build/,${SRC1:.cpp=.o})
OBJECTS2 = $(addprefix build/,${SRC2:.cpp=.o})
CC=g++
CFLAGS = -O2 -std=c++11 -Wall -Werror
LDFLAGS= -lrobot -lrobotdriver -lpathfollower
SCRIPT = script_copy_in_initd.sh
DEST_SCRIPT = launchLoop.sh
DEST = /var/apps
DEST_INIT = /etc/init.d
VPATH = build/

vpath %.cpp src_main/
vpath %.hpp src_main/

.PHONY: clean all build

all: build build/$(TARGET1) build/$(TARGET2)

build:
	mkdir -p build
build/%.o: %.cpp build/%.d
	$(CC) -c -o $@ $< $(CFLAGS)
build/%.d : %.cpp
	$(CC) $(CFLAGS) -MM -MF $@ -MP $<
build/$(TARGET1): $(OBJECTS1)
	$(CC) $(CFLAGS) $(OBJECTS1) -o $@ $(LDFLAGS)
build/$(TARGET2): $(OBJECTS2)
	$(CC) $(CFLAGS) $(OBJECTS2) -o $@ $(LDFLAGS)

clean:
	rm -f build/*.o build/*.d
	rm -f $(DEST)/$(TARGET2)
	rm -f $(DEST)/$(TARGET3)
	rm -f $(DEST_INIT)/$(DEST_SCRIPT)

update_initd: build/$(TARGET2)
	cp build/$(TARGET2) $(DEST)/$(TARGET2)
	cp build/$(TARGET1) $(DEST)/$(TARGET3)
	cp $(SCRIPT) $(DEST_INIT)/$(DEST_SCRIPT)

	chmod 755 $(DEST_INIT)/$(DEST_SCRIPT)
	update-rc.d $(DEST_SCRIPT) defaults


-include $(subst .cpp,.d,$(SRC1))
-include $(subst .cpp,.d,$(SRC2))
