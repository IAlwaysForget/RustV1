#pragma once
#include <any>
#include <chrono>
#include <functional>

enum e_plugin_type
{
	ONCE,
	//Only run the plugin once and remove it from the list
	ONCE_PERSISTS,
	//Run the plugin once and keep it in the list, for if you ever wanna reinitialize the plugin system
	ALWAYS,
	//Run the plugin every frame
};

class c_plugin
{
private:
	struct s_plugin
	{
		std::function<void()> plugin;
		e_plugin_type type;
		std::chrono::milliseconds delay;
		bool plugin_ran;
		std::chrono::time_point<std::chrono::steady_clock> start_run;

		bool operator==(const s_plugin& other) const
		{
			return plugin.target<void()>() == other.plugin.target<void()>();
		}
	};

	std::vector<s_plugin> plugins;

public:
	c_plugin();

	~c_plugin();

	template <typename Func>
	void AddPlugin(Func& func, e_plugin_type type = e_plugin_type::ALWAYS, std::chrono::milliseconds delay = std::chrono::milliseconds(0))
	{
		plugins.push_back({func, type, delay, false, std::chrono::steady_clock::now()});
	}

	//TODO: test ONCE & ONCE_PERSISTS
	void Run()
	{
		for (auto& plugin : plugins)
		{
			if (plugin.type == e_plugin_type::ONCE)
			{
				plugin.plugin();
				plugins.erase(std::remove(plugins.begin(), plugins.end(), plugin), plugins.end());
			}
			else if (plugin.type == e_plugin_type::ONCE_PERSISTS)
			{
				plugin.plugin();
				plugin.plugin_ran = true;
			}
			else if (plugin.type == e_plugin_type::ALWAYS)
			{
				if (std::chrono::steady_clock::now() - plugin.start_run > plugin.delay)
				{
					plugin.plugin();
					plugin.start_run = std::chrono::steady_clock::now();
				}
			}
		}
	}
};
