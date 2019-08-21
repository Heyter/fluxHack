#pragma once

#include <sstream>
#include <fstream>
#include <direct.h>
#include "recv.h"
#include "UtilsFuncs.h"

// another big paste. b1g lazy
class CNetworkedVariableManager {
public:
	CNetworkedVariableManager(void) {
		m_tables.clear();
		ClientClass *clientClass = g_pClient->GetAllClasses();
		if (!clientClass)
			return;
		
		while (clientClass) {
			RecvTable *recvTable = clientClass->m_pRecvTable;
			m_tables.push_back(recvTable);
			clientClass = clientClass->m_pNext;
		}
	}
	int GetOffset(const char *tableName, const char *propName)
	{
		int offset = GetProp(tableName, propName);
		if (!offset)
			return 0;
		return offset;
	}
	
	void DumpNetvars()
	{
		std::stringstream ss;
		char cwd[1024];

		for (ClientClass* pClass = g_pClient->GetAllClasses(); pClass != NULL; pClass = pClass->m_pNext)
		{
			RecvTable* table = pClass->m_pRecvTable;
			ss << DumpTable(table, 0);
		}

		_getcwd(cwd, sizeof(cwd));

		std::string netvarsPath = std::string(cwd) + "/netvars.txt";
		std::ofstream(netvarsPath) << ss.str();
	}

	DWORD HookProp(const char *tableName, const char *propName, RecvVarProxyFn function)
	{
		RecvProp *recvProp = 0;
		GetProp(tableName, propName, &recvProp);
		if (!recvProp)
			return NULL;
		DWORD oProxy = (DWORD)recvProp->m_ProxyFn;
		recvProp->m_ProxyFn = function;
		return oProxy;
	}
	std::vector<RecvTable*> m_tables;
private:
	int GetProp(const char *tableName, const char *propName, RecvProp **prop = 0)
	{
		RecvTable *recvTable = GetTable(tableName);
		if (!recvTable)
			return 0;
		int offset = GetProp(recvTable, propName, prop);
		if (!offset)
			return 0;
		return offset;
	}
	int GetProp(RecvTable *recvTable, const char *propName, RecvProp **prop = 0)
	{
		int extraOffset = 0;
		for (int i = 0; i < recvTable->m_nProps; ++i)
		{
			RecvProp *recvProp = &recvTable->m_pProps[i];
			RecvTable *child = recvProp->m_pDataTable;
			if (child
				&& (child->m_nProps > 0))
			{
				int tmp = GetProp(child, propName, prop);
				if (tmp)
					extraOffset += (recvProp->m_Offset + tmp);
			}


			if (_stricmp(recvProp->m_pVarName, propName))
				continue;
			if (prop)
				*prop = recvProp;
			return (recvProp->m_Offset + extraOffset);
		}
		return extraOffset;
	}
	RecvTable *GetTable(const char *tableName)
	{
		if (m_tables.empty())
			return 0;
		for (RecvTable *table : m_tables)
		{
			if (!table)
				continue;
			if (_stricmp(table->m_pNetTableName, tableName) == 0)
				return table;
		}
		return 0;
	}

	void DumpTableNames()
	{
		if (m_tables.empty())
			return;

		for (RecvTable *table : m_tables)
		{
			if (!table)
				continue;
			print("%s\n",table->m_pNetTableName);
		}
	}
	
	std::string DumpTable(RecvTable* table, int depth)
	{
		std::string pre("");
		std::stringstream ss;

		for (int i = 0; i < depth; i++)
			pre.append("\t");

		ss << pre << table->m_pNetTableName << "\n";

		for (int i = 0; i < table->m_nProps; i++) {
			RecvProp* prop = &table->m_pProps[i];
			if (!prop)
				continue;

			std::string varName(prop->m_pVarName);

			if (varName.find("baseclass") == 0 || varName.find("0") == 0 || varName.find("1") == 0 || varName.find("2") == 0)
				continue;

			ss << pre << "\t" << varName << " [0x" << std::hex << prop->m_Offset << "]\n";

			if (prop->m_pDataTable)
				ss << DumpTable(prop->m_pDataTable, depth + 1);
		}

		return ss.str();
	}
};