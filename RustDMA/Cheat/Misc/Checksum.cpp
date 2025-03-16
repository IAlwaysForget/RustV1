#include "Checksum.h"
#include <filesystem>
#include <iostream>
#include <unordered_set>

#include <cryptopp/md5.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <webp/decode.h>

#include "context.h"
#include "xorstr.hpp"
#include "Vorpal/API/Vorpal.h"
#include "Zip/zip.h"

std::string assets_path = xorstr_("Moe/Assets/");

std::string CalculateLocalFileChecksum(const std::string& file_path)
{
	using namespace CryptoPP;

	// Declare objects for MD5 hash and its hexadecimal representation
	MD5 hash;
	std::string digest;

	// Create pipeline: read file, calculate MD5 hash, encode to hexadecimal
	FileSource(file_path.c_str(), true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));

	return digest;
}

std::string GetRemoteChecksum(std::string url)
{
	auto ChecksumURL = url + xorstr_("/checksum");
	printf("Checksum URL: %s\n", ChecksumURL.c_str());
	std::string response;
	if (!GetRequest(ChecksumURL, response, { }))
	{
		ctx.m_logger->Error(xorstr_("Failed to get remote checksum\n"));
		return "";
	}
	if (response.empty())
	{
		ctx.m_logger->Error(xorstr_("Remote checksum response is empty\n"));
		return "";
	}
	return response;
}

void DownloadZipFile(std::string url, std::string dir_path, std::string file_name)
{
	// Ensure the directory path ends with a slash (or backslash depending on your OS)
	if (!dir_path.empty() && dir_path.back() != '/' && dir_path.back() != '\\')
	{
		dir_path += "/"; // Use '\\' for Windows
	}

	std::string file_path = dir_path + file_name; // Name of the saved file

	if (std::filesystem::exists(file_path))
	{
		std::string localChecksum = CalculateLocalFileChecksum(file_path);
		std::string remoteChecksum = GetRemoteChecksum(url);
		if (remoteChecksum.empty() && !localChecksum.empty())
		{
			ctx.m_logger->Error(xorstr_("Remote checksum is empty\n"));
			return;
		}
		printf("Local Checksum: %s\n", localChecksum.c_str());
		printf("Remote Checksum: %s\n", remoteChecksum.c_str());

		if (localChecksum == remoteChecksum && (!localChecksum.empty() && !remoteChecksum.empty()))
		{
			std::cout << file_name << " is up to date.\n";
			return;
		}
		else
		{
			std::cout << file_name << " has changed. Redownloading...\n";
		}
	}

	// Check if the file already exists
	/*if (std::filesystem::exists(file_path)) {
		std::cout << "[+] File already exists, no need to download: " << file_path << std::endl;
		return; // Exit if file exists
	}*/

	std::string response;
	if (!GetRequestProgress(url, response, { }))
	{
		std::cout << "[-] Failed to download the file from " << url << std::endl;
		return;
	}

	if (response.empty())
	{
		std::cout << "[-] The response is empty, nothing to save." << std::endl;
		return;
	}

	try
	{
		std::ofstream file(file_path, std::ios::out | std::ios::binary);
		if (!file)
		{
			std::cerr << "[-] Unable to open file for writing: " << file_path << std::endl;
			return;
		}

		file.write(response.data(), response.size());
		if (!file.good())
		{
			throw std::runtime_error("Failed to write the file content.");
		}

		file.close();
		std::cout << "[+] File downloaded successfully and saved to " << file_path << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "[-] Failed to download assets :( : " << e.what() << std::endl;
		// Consider removing the recursive call to prevent potential infinite loops
		// Maybe log the error or inform the user to try again manually
	}
}

struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator ()(const std::pair<T1, T2>& pair) const
	{
		auto hash1 = std::hash<T1> { }(pair.first);
		auto hash2 = std::hash<T2> { }(pair.second);
		return hash1 ^ hash2;
	}
};

std::unordered_map<std::pair<std::string, std::string>, ImageData, pair_hash> ImageCache;
std::unordered_map<std::string, bool> loadingImageCache;
std::unordered_set<std::string> NotFoundImages;
std::mutex imageCacheMutex;

void LoadImageFromZipEx(std::string dir_path, std::string FileName, std::string ImageName, float Scale, ImVec2 Offset)
{
	loadingImageCache[dir_path + FileName] = true;
	ImageData data = { };
	std::vector<uint8_t> bffr;
	void* buf = NULL;
	size_t bufsize;

	auto cacheKey = std::make_pair(FileName, ImageName);

	if (!std::filesystem::exists(dir_path))
	{
		ctx.m_logger->Error(xorstr_("Error Finding \n"));
		ctx.m_logger->Error(dir_path.c_str());
		loadingImageCache[dir_path + FileName] = false;
		return;
	}

	struct zip_t* zip = zip_open(dir_path.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'r');
	if (zip == NULL)
	{
		ctx.m_logger->Error(xorstr_("Error opening zip file\n"));
		loadingImageCache[dir_path + FileName] = false;
		return; // handle error
	}

	int err = zip_entry_open(zip, FileName.c_str());
	if (err != 0)
	{
		NotFoundImages.insert(dir_path + FileName);
		// print the dirpath and filename
		ctx.m_logger->Error(xorstr_("Error opening zip entry: %i\n"), err);
		zip_close(zip);
		loadingImageCache[dir_path + FileName] = false;
		return; // handle error
	}

	err = zip_entry_read(zip, &buf, &bufsize);
	if (err < 0)
	{
		ctx.m_logger->Error(xorstr_("Failed to read zip entry: %i\n"), err);
		loadingImageCache[dir_path + FileName] = false;
		return;
	}

	bffr = std::vector<uint8_t>(static_cast<uint8_t*>(buf), static_cast<uint8_t*>(buf) + bufsize);

	zip_entry_close(zip);
	zip_close(zip);
	free(buf);

	int width, height;
	uint8_t* decodedImage = WebPDecodeRGBA(bffr.data(), bffr.size(), &width, &height);
	if (decodedImage != nullptr)
	{
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = decodedImage;
		initialData.SysMemPitch = width * 4;
		initialData.SysMemSlicePitch = 0;

		ID3D11Texture2D* texture = nullptr;
		HRESULT hrTexture = g_pd3dDevice->CreateTexture2D(&desc, &initialData, &texture);
		if (SUCCEEDED(hrTexture))
		{
			ID3D11ShaderResourceView* imageSRV = nullptr;
			HRESULT hrSRV = g_pd3dDevice->CreateShaderResourceView(texture, nullptr, &imageSRV);
			if (SUCCEEDED(hrSRV))
			{
				// Bind the imageSRV to the pixel shader
				g_pd3dDeviceContext->PSSetShaderResources(0, 1, &imageSRV);

				data.texture = imageSRV; // Set the blurred image as a texture
				data.height = height;
				data.width = width;
				data.name = ImageName;
				data.scale = Scale;
				data.offset = Offset;
				if (data.texture != nullptr)
				{
				//	imageCacheMutex.lock();
					ImageCache[cacheKey] = data;
				//	imageCacheMutex.unlock();
				}
			}
			else
			{
				ctx.m_logger->Error(xorstr_("Failed to create shader resource view %i\n"), hrSRV);
			}

			texture->Release();
		}
		else
		{
			LPVOID errorMsg;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				hrTexture,
				0,
				(LPSTR)&errorMsg,
				0,
				NULL
			);
			// Output or log the error messages
			ctx.m_logger->Error(xorstr_("CreateTexture2D failed with error %08X: %s\n"), hrTexture, errorMsg);
		}

		free(decodedImage);
	}
	else
	{
		printf("Name: %s\n", ImageName.c_str());
		ctx.m_logger->Error(xorstr_("Failed to decode webp image\n"));
	}
}

ImageData LoadImageFromZip(std::string dir_path, std::string FileName, std::string ImageName, float Scale, ImVec2 Offset, bool IsEncrypted)
{
	if (!g_pd3dDevice || !g_pd3dDeviceContext)
	{
		ctx.m_logger->Error(xorstr_("Failed to load texture from file, device or context is null\n"));
		return ImageData();
	}
	if (NotFoundImages.find(dir_path + FileName) != NotFoundImages.end())
	{
		return ImageData();
	}

	auto cacheKey = std::make_pair(FileName, ImageName);
//	imageCacheMutex.lock();
	auto it = ImageCache.find(cacheKey);
	if (it != ImageCache.end())
	{
		ImageData cachedImageData = it->second;
		imageCacheMutex.unlock();
		return cachedImageData;
	}
//	imageCacheMutex.unlock();
	if (loadingImageCache.find(dir_path + FileName) == loadingImageCache.end())
	{
		loadingImageCache[dir_path + FileName] = false;
	}
	if (!loadingImageCache[dir_path + FileName])
	{
		LoadImageFromZipEx( dir_path, FileName, ImageName, Scale, Offset);
	}
	return ImageData();
}

ImageData LoadItemIconFromZip(std::string id)
{
	std::string AssetPath = std::string(assets_path);
	std::string FolderPath = std::filesystem::path(getenv(xorstr_("USERPROFILE")) + (std::string)xorstr_("\\Documents")).string() + "/" + AssetPath;
	std::string FileName = xorstr_("/icons.zip");
	std::string ImageName = id;
	std::string ImagePath = id + xorstr_(".webp");
	return LoadImageFromZip(FolderPath + FileName, ImagePath, ImageName, 1.0, ImVec2(0, 0), false);
}

void DownloadAssets()
{
	std::string AssetPath = std::string(assets_path);
	std::string FolderPath = std::filesystem::path(getenv(xorstr_("USERPROFILE")) + (std::string)(xorstr_("\\Documents"))).string() + xorstr_("/") + AssetPath;

	if (!std::filesystem::exists(FolderPath))
		std::filesystem::create_directory(FolderPath);

	// removed
}
