#pragma once
#include "Config/Config.h"
#include "Plugins/plugin.h"
#include "UI/Draw.h"
#include "Utils/Logging.h"

class c_context
{
public:
	c_context() : m_logger(std::make_shared<c_logger>()), m_draw(std::make_shared<c_draw>()), m_plugin(std::make_shared<c_plugin>())
	{
	}

	~c_context()
	{
	}

	//Settings
	std::shared_ptr<c_logger> m_logger;

	/*Drawing*/
	std::shared_ptr<c_draw> m_draw;
	std::shared_ptr<c_plugin> m_plugin;

	/*Settings, normal settings*/
	c_settings m_settings;
};

extern c_context ctx;
