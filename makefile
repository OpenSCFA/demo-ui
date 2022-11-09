LUA = lib/Lua-5.1.5
GSL = 3rd/GSL

INC = -I$(LUA)/inc -I$(GSL)/include
BIN_DIR = bin
SRC_DIR = src

CXXFLAGS = --std=c++2a -g $(INC)

default: all

all: $(BIN_DIR)/main.o bin/logger.o bin/lapi.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(BIN_DIR)/*.o -o $(BIN_DIR)/main

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

clean:
	cd $(BIN_DIR) && rm -f *.o *.exe

deepclean:
	rm -f -r *.o *.a *.exe

.PHONY: default all clean deepclean

bin/main.o: src/main.cpp $(LUA)/etc/lua.hpp

# Lua wants to be installed, not included, so we'll commandeer their internal makefile and
# mess with the object files it creates
bin/l%.o:
	@echo $@
	cd $(LUA)/src && $(MAKE) o
	mv -f $(LUA)/src/*.o bin
	cp -f $(LUA)/etc/lua.hpp $(LUA)/src/lua.h $(LUA)/src/lualib.h $(LUA)/src/luaconf.h $(LUA)/src/lauxlib.h $(LUA)/inc
	rm -f bin/luac.o bin/lua.o
# these last two guys are for the standalone interpreter and compiler
# they *both* have `main()` functions that cause problems
