#pragma once

#include <iostream>
#include <sstream>
#include <memory>
#include "DeviceResources.h"

namespace GNF::Common::Device
{
	class DebugDeviceResources : public IDeviceResources
	{
		DebugDeviceResources(IDeviceResources* reso)
		{
			res.reset(reso);
		}

		virtual void Init()
		{
			debugMessages << "Init started\n";
			res->Init();
		}

		virtual bool IsMultiSamplingEnabled()
		{
			return res->IsMultiSamplingEnabled();
		}

		virtual float AspectRatio() const noexcept
		{
			return res->AspectRatio();
		}

		virtual void ClearBackgroundColor()
		{
			debugMessages << "Clearing BG Color\n";
			return res->ClearBackgroundColor();
		}

		virtual void HandleWindowSizeChanged()
		{
			debugMessages << "Window Size is Rehandling\n";
			return res->HandleWindowSizeChanged();
		}

		virtual ID3D11Device* GetD3DDevice()
		{
			return res->GetD3DDevice();
		}

		virtual ID3D11DeviceContext* GetD3DContext()
		{
			return res->GetD3DContext();
		}

		virtual HRESULT CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements, ID3D11InputLayout** layout)
		{
			debugMessages << "Creating Input Layout\n";
			return res->CreateInputLayout(desc, numElements, layout);
		}

		virtual void SwapChain()
		{
			 res->SwapChain();
		}

		virtual void Cls()
		{
			 res->Cls();
		}

		virtual void SetTargetViews()
		{
			 res->SetTargetViews();
		}

		virtual Texture::ITextureManager* GetTextureManager()
		{
			return res->GetTextureManager();
		}
	private:
		virtual void CreateDependentResources()
		{
			debugMessages << "Started to Create Dependent Resources";
			res->CreateDependentResources();
		}

		virtual void CreateInDependentResources()
		{
			debugMessages << "Started to Create Independent Resources";
			res->CreateInDependentResources();
		}

		std::unique_ptr<IDeviceResources> res;
		std::ostringstream debugMessages;

	};
}