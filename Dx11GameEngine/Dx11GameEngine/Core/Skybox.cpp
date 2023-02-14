#include "pch.h"

#include "Skybox.h"
#include "Game.h"
#include "Core/VertexBuffer/SkyboxVertexBuffer.h"
namespace GNF::Core
{
	Skybox::Skybox(const wchar_t* imgPath) : m_imagePath(imgPath),IResource("Skybox")
	{

	}

	void Skybox::Init()
	{
		auto id = Core::Game::GetInstance()->GetCurrentTextureManager()->CreateImage(m_imagePath.c_str());
		
		if (id == 0)
			throw std::runtime_error("Skybox couldn't loaded");

		bool succeed = Core::Game::GetInstance()->GetCurrentTextureManager()->ChangeImageToCubemap(id);
		
		if(!succeed)
			throw std::runtime_error("Skybox couldn't loaded");
		
		m_textureId = Core::Game::GetInstance()->GetCurrentTextureManager()->CreateTextureFromImageAndDelete(id);

		if(m_textureId == 0)
			throw std::runtime_error("Skybox couldn't loaded");

		m_vertexShader.reset(Core::Game::GetInstance()->GetGraphicEngine()->CreateVertexShader<VertexBuffer::SkyboxVertexBuffer>(L"SkyboxVertexShader.cso"));	
		m_pixelShader.reset(Core::Game::GetInstance()->GetGraphicEngine()->CreatePixelShader(L"SkyboxPixelShader.cso"));
		
		std::vector<VertexBuffer::SkyboxVertexBuffer> box =
		{
			{-1.0,-1.0, 1.0},
			{1.0,-1.0, 1.0 },
			{1.0, 1.0, 1.0 },
			{-1.0, 1.0, 1.0 },
			{-1.0,-1.0,-1.0 },
			{1.0,-1.0,-1.0 },
			{1.0, 1.0,-1.0 },
			{- 1.0, 1.0,-1.0}
		};
		std::vector<UINT> indices = {
			// front
			0, 1, 2, 2, 3, 0,
			// right
			1, 5, 6, 6, 2, 1,
			// back
			7, 6, 5, 5, 4, 7,
			// left
			4, 0, 3, 3, 7, 4,
			// bottom
			4, 5, 1, 1, 0, 4,
			// top
			3, 2, 6, 6, 7, 3
		};
		m_vertexBuffer.reset(Core::Game::GetInstance()->GetGraphicEngine()->CreateDefaultVertexBuff(box));
		m_indexBuffer.reset(Core::Game::GetInstance()->GetGraphicEngine()->CreateDefaultIndexBuff(indices));

		D3D11_DEPTH_STENCIL_DESC dssDesc;
		ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
		dssDesc.DepthEnable = true;
		dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DDevice()->CreateDepthStencilState(&dssDesc, m_depthStencilState.GetAddressOf());

		D3D11_RASTERIZER_DESC cmdesc;
		ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
		cmdesc.FillMode = D3D11_FILL_SOLID;
		cmdesc.CullMode = D3D11_CULL_NONE;
		cmdesc.FrontCounterClockwise = true;
		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DDevice()->CreateRasterizerState(&cmdesc, m_rasterizerState.GetAddressOf());

	}
	void Skybox::Draw()
	{
		m_pixelShader->Bind();
		m_vertexShader->Bind();

		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();
		Core::Game::GetInstance()->GetCurrentTextureManager()->BindTexture(m_textureId);
		Core::Game::GetInstance()->GetCamera()->Use(m_scaleMatrix * DirectX::SimpleMath::Matrix::CreateTranslation(Core::Game::GetInstance()->GetCamera()->GetPos()) );
		
		ID3D11RasterizerState* prevRSState;
		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->RSGetState(&prevRSState);
		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->RSSetState(m_rasterizerState.Get());

		ID3D11DepthStencilState* prevDSState;
		UINT ref;
		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->OMGetDepthStencilState(&prevDSState, &ref);
		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->OMSetDepthStencilState(m_depthStencilState.Get(),0);

		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->DrawIndexed(36, 0,0);


		//!: Set Prev Raster States
		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->RSSetState(prevRSState);
		Core::Game::GetInstance()->GetGraphicEngine()->GetD3DContext()->OMSetDepthStencilState(prevDSState, ref);


	}
}