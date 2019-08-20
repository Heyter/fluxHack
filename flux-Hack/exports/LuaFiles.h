#pragma once
#include <string>
//Files in here could be executed but we don't want the client to worry about these

std::string LUA_INJECT_CODE("function include(path) \
Game.include(path) \
end \
");
