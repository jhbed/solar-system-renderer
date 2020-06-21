#Wall - gives us warning messages
#-g will allow us to use the debugger later on 
# -o specifies outfile name
# $@ - alias for the current target
#-c compile
# -I include directory; makes it so we can do <geometry> instead of <../geometry>; the dot is for the current path
# mkdir $(subst /,\,$(@D)) > nul 2>&1 || (exit 0)
CC = g++
CFLAGS = -std=c++11 -Wall -g
PROJECT_DIR = .
OBJS_DIR = objs
EXTERNAL_LIB_DIR = /Users/jake/Projects/lib
NLOHMANN_JSON = $(EXTERNAL_LIB_DIR)/json/include

#wildcard searches all cpp files
ALL_SRC = $(wildcard $(PROJECT_DIR)/**/*.cpp) $(wildcard $(PROJECT_DIR)/**/**/*.cpp)
#changes all cpp files to o files
ALL_OBJ = $(patsubst $(PROJECT_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(ALL_SRC))

tinyraytracer: $(ALL_OBJ)
	$(CC) $(CFLAGS) -o $@ $(ALL_OBJ)

#makes the directory, 
$(OBJS_DIR)/%.o: $(PROJECT_DIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(PROJECT_DIR) -I$(NLOHMANN_JSON)


clean: 
	rm tinyraytracer
	#rm objs/*
	rm objs/*/*
	rm out.ppm

