#include "plugin.h"

#include "Huds.h"
#include "map.h"

c_plugin::c_plugin()
{
	//Note: initialize plugins here, do it in the order you want to display / load them.
	AddPlugin(Map::RenderMap);
	AddPlugin(HUDS::Run);
}

c_plugin::~c_plugin()
{
}
