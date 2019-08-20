#include "LuaEngine.h"

LuaEngine *g_pLuaEngine = new LuaEngine();

lua_State* LuaEngine::L()
{
	return m_L;
}

void LuaEngine::report_errors(int state)
{
	if (state)
	{
		std::cerr << "ERR: " << lua_tostring(m_L, -1) << std::endl;
		lua_pop(m_L, 1); //remove error
	}
}

void LuaEngine::ExecuteFile(const char* file)
{
	if (!file || !m_L)
		return;

	int state = luaL_dofile(m_L, file);
	report_errors(state);
}

void LuaEngine::ExecuteFileBuffer(const std::string& identifier, const std::string& code) {
	if (!m_L)
		return;
	
	int state = luaL_loadbuffer(m_L, code.c_str(), code.length(), identifier.c_str()) || lua_pcall(m_L, 0, LUA_MULTRET, 0);
	report_errors(state);
}

void LuaEngine::ExecuteString(const char* expression)
{
	if (!expression || !m_L)
	{
		std::cerr << "ERR: null expression passed to script engine!" << std::endl;
		return;
	}

	int state = luaL_dostring(m_L, expression);
	report_errors(state);
}

int LuaEngine::SetLuaPath(const char* path)
{
	if (!path || !m_L)
	{
		std::cerr << "ERR: null expression passed to script engine!" << std::endl;
		return 0;
	}
	
    lua_getglobal(m_L, "package" );
    lua_getfield(m_L, -1, "path" ); // get field "path" from table at top of stack (-1)
    std::string cur_path = lua_tostring(m_L, -1 ); // grab path string from top of stack
    cur_path.append( ";" ); // do your path magic here
    cur_path.append( path );
    lua_pop(m_L, 1 ); // get rid of the string on the stack we just pushed on line 5
	printf("%s\n", cur_path.c_str());
    lua_pushstring(m_L, cur_path.c_str() ); // push the new one
    lua_setfield(m_L, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop(m_L, 1 ); // get rid of package table from top of stack
    return 0; // all done!
}