#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>

class ConsoleManager
{
public:
	ConsoleManager();

	void Think();

	void AddItem(std::string command, std::function<void(std::vector<std::string>)> callback);
	
	std::map<std::string, std::function<void(std::vector<std::string>)>> m_commands;
private:
	std::vector<std::string> split(std::string& str, char delimiter);
};