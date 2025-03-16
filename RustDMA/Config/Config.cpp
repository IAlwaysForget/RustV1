#include "Config.h"
#include <fstream>

void holder_t::add(node_t* node_ptr)
{
	m_nodes.push_back(node_ptr);
}

void holder_t::load(const char* path, const char* file)
{
	std::filesystem::path pathObj(path);
	std::filesystem::path fileObj(file);
	std::filesystem::path config_path = pathObj / fileObj;

	auto config_file = OpenFile(config_path.string());
	if (!config_file)
		return;

	std::string config_str = ReadFile(config_file);

	for (auto x : m_nodes)
		x->load_setting(config_str);

	CloseFile(config_file);
}

void holder_t::load(std::string& config)
{
	for (auto x : m_nodes)
		x->load_setting(config);
}

void holder_t::save(std::string& config)
{
	for (auto x : m_nodes)
		x->save_setting(config);
}

void holder_t::save(const char* path, const char* file)
{
	std::filesystem::path pathObj(path);
	std::filesystem::path fileObj(file);
	std::string config_path = pathObj.string() + "\\" + fileObj.string();

	CreateFolder(pathObj.string());

	//SAve File
	auto files = OpenFile(config_path, std::ios::app | std::ios::in);
	WriteToFile("", files);
	CloseFile(files);

	std::ofstream config_file = OpenFile(config_path);

	std::string config_str;

	for (auto x : m_nodes)
		x->save_setting(config_str);

	if (!WriteToFile(config_str, config_file))
	{
		//Logger Failed to save config
	}
	else
	{
		//Logger Successfully Saved Config.
	}
	CloseFile(config_file);
}
