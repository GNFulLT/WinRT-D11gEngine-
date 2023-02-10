#include "pch.h"

#include "ITextureLoader.h"
#include <directxpackedvector.h>
#include "Image/Bitmap.h"
#include "UtilsCubemap.h"
#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif
namespace GNF::Texturing
{
	HRESULT ITextureLoader::CreateTextureFromImage2D(
		_In_ const Image::IImage* img,
		_Out_opt_ ID3D11ShaderResourceView1** srv,
		_In_opt_ D3D11_TEXTURE_LAYOUT layout,
		_In_opt_ D3D11_USAGE usage,
		_In_opt_ UINT cpuaccessFlag 
	)
	{
		auto device = GetDevice();
		
		if (!device)
		{
			OutputDebugString(L"Device was nullptr");
			return E_FAIL;
		}

		CD3D11_TEXTURE2D_DESC desc(img->GetFormat(), img->GetWidth(), img->GetHeight(),
			img->m_metaData.arraySize,img->m_metaData.mipLevels,8U,usage,cpuaccessFlag);

		CD3D11_TEXTURE2D_DESC1 desc1(desc, layout);

		if (img->IsCubemap())
		{
			desc1.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
		}

		ID3D11Texture2D1* texture;

		HRESULT hr = S_OK;
		if (img->IsCubemap())
		{
			
			D3D11_SUBRESOURCE_DATA initData[6];
			for (int i = 0; i < 6; i++)
			{
				auto selectedImg = img->m_image.GetImage(0, i, 0);
				//initData[i].pSysMem = img->m_cube.data_.data() + ( img->m_cube.w_ * img->m_cube.h_ * i * DirectX::BitsPerPixel(img->GetFormat())/8);
				initData[i].pSysMem = selectedImg->pixels;
				//initData[i].SysMemPitch = img->m_cube.w_ * DirectX::BitsPerPixel(img->GetFormat()) / 8;
				initData[i].SysMemPitch = selectedImg->rowPitch;
				//initData[i].SysMemSlicePitch = initData[i].SysMemPitch * img->m_cube.h_;
				initData[i].SysMemSlicePitch = selectedImg->slicePitch;
			}
			hr = device->CreateTexture2D1(&desc1, initData, &texture);

			if (FAILED(hr))
			{
				return hr;
			}
		}
		else
		{
			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = img->m_image.GetPixels();
			initData.SysMemPitch = img->m_rawImage->rowPitch;
			initData.SysMemSlicePitch = img->m_rawImage->slicePitch;

			hr = device->CreateTexture2D1(&desc1, &initData, &texture);

			if (FAILED(hr))
			{
				return hr;
			}
		}
		

		CD3D11_SHADER_RESOURCE_VIEW_DESC1 srvDesc1(texture, D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D);

		if (img->IsCubemap())
		{
			srvDesc1 = CD3D11_SHADER_RESOURCE_VIEW_DESC1(texture, D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURECUBE);
		}
		hr = device->CreateShaderResourceView1(texture, &srvDesc1, srv);

		return hr;
	}

	//vec3 faceCoordsToXYZ(int i, int j, int faceID, int faceSize)
	//{
	//	const float A = 2.0f * float(i) / faceSize;
	//	const float B = 2.0f * float(j) / faceSize;

	//	if (faceID == 0) return vec3(-1.0f, A - 1.0f, B - 1.0f);
	//	if (faceID == 1) return vec3(A - 1.0f, -1.0f, 1.0f - B);
	//	if (faceID == 2) return vec3(1.0f, A - 1.0f, 1.0f - B);
	//	if (faceID == 3) return vec3(1.0f - A, 1.0f, 1.0f - B);
	//	if (faceID == 4) return vec3(B - 1.0f, A - 1.0f, 1.0f);
	//	if (faceID == 5) return vec3(1.0f - B, A - 1.0f, -1.0f);

	//	return vec3();
	//}

	//using namespace DirectX;

	//void XMUBYTEN4ToFloat4(XMFLOAT4& result, const DirectX::PackedVector::XMUBYTEN4& input)
	//{
	//	result.x = (float)input.x / 255.0f;
	//	result.y = (float)input.y / 255.0f;
	//	result.z = (float)input.z / 255.0f;
	//	result.w = (float)input.w / 255.0f;
	//}

	//void GetPixel(const DirectX::ScratchImage& image, int x, int y, XMFLOAT4& pixel)
	//{
	//	assert(x >= 0 && x < image.GetMetadata().width);
	//	assert(y >= 0 && y < image.GetMetadata().height);

	//	const DirectX::Image* img = image.GetImage(0, 0, 0);
	//	const uint8_t* pSrc = img->pixels + y * img->rowPitch + x * DirectX::BitsPerPixel(img->format) / 8;

	//	DirectX::XMFLOAT4 temp;
	//	XMUBYTEN4ToFloat4(temp, (DirectX::PackedVector::XMUBYTEN4)pSrc);
	//	pixel = temp;
	//}

	//XMFLOAT4 SampleEquirectangular(const DirectX::ScratchImage& equirectangular, float u, float v)
	//{
	//	XMFLOAT2 invSphereMapSize(1.0f / (equirectangular.GetMetadata().width - 1), 1.0f / (equirectangular.GetMetadata().height - 1));

	//	u = u * invSphereMapSize.x - 0.5f;
	//	v = 0.5f - v * invSphereMapSize.y;

	//	float phi = 2.0f * XM_PI * u;
	//	float theta = XM_PI * v;

	//	float x = sinf(theta) * cosf(phi);
	//	float y = sinf(theta) * sinf(phi);
	//	float z = cosf(theta);

	//	XMFLOAT3 dir(x, y, z);

	//	XMFLOAT4 color;
	//	GetPixel(equirectangular, u / invSphereMapSize.x + 0.5f, 0.5f - v / invSphereMapSize.y, color);

	//	return color;
	//	/*
	//	// Clamp texture coordinates to [0, 1]
	//	u = std::clamp(u, 0.0f, 1.0f);
	//	v = std::clamp(v, 0.0f, 1.0f);

	//	// Convert texture coordinates to pixel coordinates
	//	int x = (int)(u * equirectangular.GetMetadata().width);
	//	int y = (int)(v * equirectangular.GetMetadata().height);

	//	// Get the four nearest pixels to the given texture coordinates
	//	XMFLOAT4 p00, p01, p10, p11;
	//	GetPixel(equirectangular,x,y,p00);
	//	GetPixel(equirectangular, x+1, y, p01);
	//	GetPixel(equirectangular, x, y+1, p10);
	//	GetPixel(equirectangular, x+1, y+1, p11);

	//	// Interpolate the colors of the four nearest pixels to get the final color
	//	float fx = u * equirectangular.GetMetadata().width - x;
	//	float fy = v * equirectangular.GetMetadata().height - y;
	//	float fx1 = 1.0f - fx;
	//	float fy1 = 1.0f - fy;

	//	XMFLOAT4 result;
	//	result.x = p00.x * fx1 + p10.x * fx;
	//	result.y = p00.y * fx1 + p10.y * fx;
	//	result.z = p00.z * fx1 + p10.z * fx;
	//	result.w = p00.w * fx1 + p10.w * fx;

	//	result.x = result.x * fy1 + (p01.x * fx1 + p11.x * fx) * fy;
	//	result.y = result.y * fy1 + (p01.y * fx1 + p11.y * fx) * fy;
	//	result.z = result.z * fy1 + (p01.z * fx1 + p11.z * fx) * fy;
	//	result.w = result.w * fy1 + (p01.w * fx1 + p11.w * fx) * fy;

	//	return result;
	//	*/
	//}
	//void SetPixel(ScratchImage& image, int x, int y, const XMFLOAT4& color)
	//{
	//	XMFLOAT4* pixels = reinterpret_cast<XMFLOAT4*>(image.GetPixels());
	//	pixels[x + y * image.GetMetadata().width] = color;
	//}
	//void EquirectangularToCubemap(ScratchImage& equirectangular, ScratchImage& cubemap)
	//{
	//	/*
	//	// Define the cube map face sizes
	//	const int faceWidth = equirectangular.GetMetadata().width / 4;
	//	const int faceHeight = equirectangular.GetMetadata().height / 3;

	//	// Create a description for the cube map image
	//	TexMetadata cubemapMetadata;
	//	cubemapMetadata.width = faceWidth;
	//	cubemapMetadata.height = faceHeight;
	//	cubemapMetadata.depth = 1;
	//	cubemapMetadata.arraySize = 6;
	//	cubemapMetadata.mipLevels = 1;
	//	cubemapMetadata.format = equirectangular.GetMetadata().format;
	//	cubemapMetadata.dimension = TEX_DIMENSION_TEXTURE2D;

	//	// Create the cube map
	//	cubemap.Initialize(cubemapMetadata);

	//	// Define the lookup table for each cube map face
	//	XMFLOAT2 lookups[6][4] =
	//	{
	//		{ { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f } }, // +X
	//		{ { 1.0f, 0.0f },{ 0.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } }, // -X
	//		{ { 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f } }, // +Y
	//		{ { 0.0f, 1.0f },{ 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f } }, // -Y
	//		{ { 0.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f },{ 1.0f, 0.0f } }, // +Z
	//		{ { 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f },{ 0.0f, 1.0f } }  // -Z
	//	};

	//	// Copy each face of the cube map from the equirectangular panorama
	//	for (int faceIndex = 0; faceIndex < 6; faceIndex++)
	//	{
	//		XMFLOAT2 uv[4];
	//		for (int i = 0; i < 4; i++)
	//		{
	//			float x = lookups[faceIndex][i].x * (float)faceWidth;
	//			float y = lookups[faceIndex][i].y * (float)faceHeight;
	//			uv[i].x = (x + 0.5f) / (float)equirectangular.GetMetadata().width;
	//			uv[i].y = (y + 0.5f) / (float)equirectangular.GetMetadata().height;
	//		}
	//		const DirectX::Image * pSrcImage = equirectangular.GetImage(0, 0, 0);
	//		const DirectX::Image* pDestImage = cubemap.GetImage(0, faceIndex, 0);

	//		for (int y = 0; y < faceHeight; y++)
	//		{
	//			for (int x = 0; x < faceWidth; x++)
	//			{
	//				float u = uv[0].x + (uv[1].x - uv[0].x) * x / (float)faceWidth + (uv[3].x - uv[0].x) * y / (float)faceHeight;
	//				float v = uv[0].y + (uv[1].y - uv[0].y) * x / (float)faceWidth + (uv[3].y - uv[0].y) * y / (float)faceHeight;
	//				XMVECTOR vc;
	//				
	//				XMFLOAT4 color = SampleEquirectangular(equirectangular, u, v);
	//				//! XMVECTOR color = equirectangular.Sample(pSrcImage, u, v);
	//				XMStoreFloat4((XMFLOAT4*)pDestImage + (y * faceWidth + x), color);
	//			}
	//		}
	//	}
	//	*/
	//	const int faceWidth = equirectangular.GetMetadata().width / 4;
	//	const int faceHeight = equirectangular.GetMetadata().height / 3;

	//	// Create a description for the cube map image
	//	TexMetadata cubemapMetadata;
	//	cubemapMetadata.width = faceWidth;
	//	cubemapMetadata.height = faceHeight;
	//	cubemapMetadata.depth = 1;
	//	cubemapMetadata.arraySize = 6;
	//	cubemapMetadata.mipLevels = 1;
	//	cubemapMetadata.format = equirectangular.GetMetadata().format;
	//	cubemapMetadata.dimension = TEX_DIMENSION_TEXTURE2D;

	//	// Create the cube map
	//	cubemap.Initialize(cubemapMetadata);

	//	// Define the lookup table for each cube map face
	//	XMFLOAT2 lookups[6][4] =
	//	{
	//		{ { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 1.0f, 1.0f },{ 0.0f, 1.0f } }, // +X
	//		{ { 1.0f, 0.0f },{ 0.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } }, // -X
	//		{ { 1.0f, 1.0f },{ 0.0f, 1.0f },{ 0.0f, 0.0f },{ 1.0f, 0.0f } }, // +Y
	//		{ { 0.0f, 1.0f },{ 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f } }, // -Y
	//		{ { 0.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f },{ 1.0f, 0.0f } }, // +Z
	//		{ { 1.0f, 1.0f },{ 1.0f, 0.0f },{ 0.0f, 0.0f },{ 0.0f, 1.0f } }  // -Z
	//	};

	//	const int cubeMapSize = cubemap.GetMetadata().width;
	//	const XMFLOAT2 invCubeMapSize(1.0f / (float)(cubeMapSize - 1), 1.0f / (float)(cubeMapSize - 1));
	//	
	//	XMFLOAT4 color;
	//	for (int face = 0; face < 6; face++)
	//	{
	//		for (int y = 0; y < cubeMapSize; y++)
	//		{
	//			for (int x = 0; x < cubeMapSize; x++)
	//			{
	//				float u = x * invCubeMapSize.x;
	//				float v = y * invCubeMapSize.y;

	//				switch (face)
	//				{
	//				case 0: // Positive X
	//					color = SampleEquirectangular(equirectangular, 0.5f - v, 0.5f - u);
	//					break;
	//				case 1: // Negative X
	//					color = SampleEquirectangular(equirectangular, 0.5f - v, u + 0.5f);
	//					break;
	//				case 2: // Positive Y
	//					color = SampleEquirectangular(equirectangular, u + 0.5f, v + 0.5f);
	//					break;
	//				case 3: // Negative Y
	//					color = SampleEquirectangular(equirectangular, u + 0.5f, 0.5f - v);
	//					break;
	//				case 4: // Positive Z
	//					color = SampleEquirectangular(equirectangular, u, v);
	//					break;
	//				case 5: // Negative Z
	//					color = SampleEquirectangular(equirectangular, u + 1.0f, v);
	//					break;
	//				}
	//				SetPixel(cubemap, x, y, color);
	//				//cubemap.SetPixel(x, y, face, color);
	//			}
	//		}
	//	}
	//}


	HRESULT ITextureLoader::ConvertImageToCubemap(_In_ Image::IImage* pImage, _Out_opt_ Image::IImage** outImg)
	{
		if (outImg == nullptr)
			return E_INVALIDARG;
		//!: First EquirectangularMap To VerticalCross Map
		//! 
		//!: After Vertical Cross to CubeMap
		
		auto img = CreateInstanceFromResponsibleImage();

		Bitmap in(pImage->GetWidth(), pImage->GetHeight(), 4, eBitmapFormat_Float, pImage->m_image.GetPixels());
		Bitmap out = convertEquirectangularMapToVerticalCross(in);
		//!: Destroy in image and create cube faces from out 
		in = convertVerticalCrossToCubeMapFaces(out);
		//!: Destroy out image
		out = Bitmap();
		
		//img->m_isCubeMap = true;
		img->m_image.InitializeCube(pImage->GetFormat(), in.w_, in.h_,1,1);
		
		auto src = in.data_.data();
		auto dst = img->m_image.GetPixels();
		auto pixelSize = DirectX::BitsPerPixel(img->GetFormat()) / 8;

		for (int face = 0; face < 6; face++)
		{
			for (int h = 0; h < in.h_; h++)
			{
				for (int w = 0; w < in.w_; w++)
				{
					//!: Copy one pixel from dst to src
					memcpy(dst, src, pixelSize);

					//! Go to the next pixel
					src += pixelSize;
					dst += pixelSize;
				}
			}
		}
		/*
		auto hr = img->m_image.InitializeCube(pImage->GetFormat(), cube.w_, cube.w_, 1, 1);

		if (FAILED(hr))
		{
			delete img;
			return hr;
		}

		auto dst = img->m_image.GetImages()->pixels;
		auto src = cube.data_.data();
		auto pixelSize = DirectX::BitsPerPixel(pImage->GetFormat());

		for (int face = 0; face < 6; face++)
		{
			for (int w = 0; w < cube.w_; w++)
			{
				for (int h = 0; h < cube.h_; h++)
				{
					memcpy(dst, src + (h * pImage->GetWidth() + w) * pixelSize / 8, pixelSize / 8);
					dst += pixelSize / 8;

				}
			}
		}
		*/
		//EquirectangularToCubemap(pImage->m_image, img->m_image);
		/*
		DirectX::ScratchImage cubeFace;
		auto hr = cubeFace.InitializeCube(pImage->GetFormat(), newWidth, newHeight, 1, 1);

		if (FAILED(hr))
		{
			return hr;
		}
		
		auto src = pImage->m_image.GetImages()->pixels;
		auto dst = cubeFace.GetImages()->pixels;
		auto pixelSize = DirectX::BitsPerPixel(pImage->GetFormat());
		
		//!: Iterate faces
		for (int face = 0; face != 6; ++face)
		{
			//!: Iterate all height
			for (int j = 0; j != newHeight; ++j)
			{
				for (int i = 0; i != newWidth; ++i)
				{
					int x = 0;
					int y = 0;
					switch (face)
					{
						// TEXTURE_CUBE_MAP_POSITIVE_X

					case 0:
						x = i;
						y = newHeight + j;
						break;

						// TEXTURE_CUBE_MAP_NEGATIVE_X
					case 1:
						x = 2 * newWidth + i;
						y = newHeight + j;
						break;

						// TEXTURE_CUBE_MAP_POSITIVE_Y
					case 2:
						x = 2 * newWidth - (i + 1);
						y = newHeight - (j + 1);
						break;

						// TEXTURE_CUBE_MAP_NEGATIVE_Y
					case 3:
						x = 2 * newWidth - (i + 1);
						y = 3 * newHeight - (j + 1);
						break;

						// TEXTURE_CUBE_MAP_POSITIVE_Z
					case 4:
						x = 2 * newWidth - (i + 1);
						y = pImage->GetHeight() - (j + 1);
						break;

						// TEXTURE_CUBE_MAP_NEGATIVE_Z
					case 5:
						x = newWidth + i;
						y = newHeight + j;
						break;
					}
					/*
					int selectedImageIndex = y / newHeight;
					auto selectedImage = cubeFace->GetImage(0, selectedImageIndex, 0);
					auto selectedBegin = selectedImage->pixels;*/
				/*
				memcpy(dst, src + (y * pImage->GetWidth() + x) * pixelSize/8, pixelSize/8);
					dst += pixelSize/8;
					*/
				/* }
			}
		}
		*/
		/*
		if (newWidth > newHeight)
		{
			hr = DirectX::Resize(cubeFace.GetImages(), 6, cubeFace.GetMetadata(), (size_t)newWidth, (size_t)newWidth, DirectX::TEX_FILTER_CUBIC, img->m_image);

		}
		else
		{
			hr = DirectX::Resize(cubeFace.GetImages(), 6, cubeFace.GetMetadata(), (size_t)newHeight, (size_t)newHeight, DirectX::TEX_FILTER_CUBIC, img->m_image);
		}
		*/
		/*
		if (FAILED(hr))
		{
			delete img;
			return hr;
		}
		*/
		(*outImg) = img;
	
		img->InitPrivateData();
		img->m_metaData = img->m_image.GetMetadata();

		return S_OK;
	}
}