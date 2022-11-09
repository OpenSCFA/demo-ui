#include <windows.h>

#include "gsl/gsl"
#include "lua.hpp"
#include "logger.hpp"

using namespace std;
using namespace gsl;


void setup_lua_state(lua_State* state);
int panic_function(lua_State* state);
void print(lua_State* state);
int lua_LOG(lua_State* state);
int lua_SPEW(lua_State* state);
int lua_WARN(lua_State* state);

Logger logger{""};


int main(int argN, czstring args[]) {
	string init_file{};
	string log_file{};

	// process switches
	for (int i = 1; i < argN; i ++) {
		string arg = args[i];
		transform(arg.begin(), arg.end(), arg.begin(), ::tolower);
		if (i + 1 < argN) {
			if (arg.compare("/init") == 0) {
				init_file = args[i + 1];
				i ++;
			} else if (arg.compare("/log") == 0) {
				log_file = args[i + 1];
				i ++;
			} else {
				std::cout << "unknown switch " << arg;
			}
		} else {
			std::cout << "unknown switch " << arg;
		}
	}

	if (log_file.length() > 0) {
		logger.set_output(log_file);
	}

	owner<lua_State*> lua_state{lua_open()};
	setup_lua_state(lua_state);

	if (init_file.length() > 0) {
		if (luaL_dofile(lua_state, init_file.c_str()) != 0) {
			logger.log("Cannot read init file ", init_file, ":\n");
			logger.log(lua_tostring(lua_state, -1));
		}
	}

	lua_close(lua_state);
}

void setup_lua_state(lua_State* state) {
	lua_atpanic(state, panic_function);
	luaL_openlibs(state);
	lua_register(state, "LOG", lua_LOG);
	lua_register(state, "SPEW", lua_SPEW);
	lua_register(state, "WARN", lua_WARN);
}

int panic_function(lua_State* state) {
	logger.log(state);
	return 0;
}

void print(lua_State* state) {
	int n = lua_gettop(state);
	for (int i = 0; i < n; i++) {
		if (i > 0) {
			printf("\t");
		}
		if (lua_isstring(state, i)) {
			logger.log(lua_tostring(state, i));
		} else if (lua_isnil(state, i)) {
			logger.log("nil");
		} else if (lua_isboolean(state, i)) {
			logger.log(lua_toboolean(state, i) ? "true" : "false");
		} else if (lua_isnumber(state, i)) {
			logger.log(lua_tonumber(state, i));
		} else {
			logger.log(luaL_typename(state, i), ' ', lua_topointer(state, i));
		}
	}
	logger.log("\n");
}

int lua_LOG(lua_State* state) {
	logger.log("info: ");
	print(state);
	return 0;
}

int lua_SPEW(lua_State* state) {
	logger.log("debug: ");
	print(state);
	return 0;
}

int lua_WARN(lua_State* state) {
	logger.log("warning: ");
	print(state);
	return 0;
}
