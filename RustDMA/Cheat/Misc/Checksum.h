#pragma once
#include <d3d11.h>
#include <string>

#include "imgui.h"

struct ImageData
{
	ID3D11ShaderResourceView* texture = nullptr;
	int width;
	int height;
	std::string name;
	ImVec2 offset;
	float scale = 1.0;
	float angle = 0.0;

	void CleanImage()
	{
		texture->Release();
	}
};

extern void DownloadAssets();
extern ImageData LoadItemIconFromZip(std::string id);
