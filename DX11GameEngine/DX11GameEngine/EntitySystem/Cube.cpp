#include "Cube.h"

#include "Common/VertexTypes.h"
#include <Windows.h>
#include "Game/GameMain.h"
#include <Windows.h>

using namespace GNF::Common;
namespace GNF::Entity
{
	
    Cube::Cube()
    {   

    }

	void Cube::Draw()
	{
        auto resources = Game::GameMain::GetInstance()->GetMainWindow()->GetDeviceResources();
        auto tmanager = resources->GetTextureManager();


        m_vertexBufferBindable->Bind();




        auto m_d3d_device = resources->GetD3DDevice();
        auto m_d3d_deviceContext = resources->GetD3DContext();

        m_d3d_deviceContext->IASetIndexBuffer(squareIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        //Set the Input Layout
        m_d3d_deviceContext->IASetInputLayout(vertLayout.Get());

        //Set Primitive Topology
        m_d3d_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        tmanager->BindTexture(textureID);


        m_d3d_deviceContext->DrawIndexed(sizeOfInd, 0, 0);

	}
	void Cube::Update(const float frameTime)
	{
      
	}
	void Cube::InitResources()
	{
        auto resources = Game::GameMain::GetInstance()->GetMainWindow()->GetDeviceResources();
        auto tmanager = resources->GetTextureManager();
        
     

        
        /*
        Vertex v[] =
        {
            Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
            Vertex(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
            Vertex(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
            Vertex(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
            Vertex(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
            Vertex(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
            Vertex(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
            Vertex(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
        };

        m_vertexBufferBindable.reset(new Bindable::VertexBufferBindable(resources, v, 8*sizeof(Vertex)));

        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);


        auto hr = resources->CreateInputLayout(layout, numElements, vertLayout.GetAddressOf());

        DWORD indices[] = {
            // front face
            0, 1, 2,
            0, 2, 3,

            // back face
            4, 6, 5,
            4, 7, 6,

            // left face
            4, 5, 1,
            4, 1, 0,

            // right face
            3, 2, 6,
            3, 6, 7,

            // top face
            1, 5, 6,
            1, 6, 2,

            // bottom face
            4, 0, 3,
            4, 3, 7
        };

        sizeOfInd = ARRAYSIZE(indices);

        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(DWORD) * sizeOfInd;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA iinitData;
        iinitData.pSysMem = indices;
        hr = resources->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &iinitData, squareIndexBuffer.GetAddressOf());
        */

        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);


        auto hr = resources->CreateInputLayout(layout, numElements, vertLayout.GetAddressOf());

        auto t = m_worldPos.x - m_edgeL/2;
        auto y = m_worldPos.y - m_edgeL/2;
        auto z = m_worldPos.z - m_edgeL/2;
        VertexTextured v[] =
        {
          
            VertexTextured(m_worldPos.x - m_edgeL/2,  m_worldPos.y - m_edgeL / 2, m_worldPos.z - m_edgeL / 2, 0.0f, 1.0f),
            VertexTextured(m_worldPos.x - m_edgeL/2,  m_worldPos.y + m_edgeL / 2, m_worldPos.z - m_edgeL / 2, 0.0f, 0.0f),
            VertexTextured(m_worldPos.x + m_edgeL/2,  m_worldPos.y + m_edgeL / 2, m_worldPos.z - m_edgeL / 2, 1.0f, 0.0f),
            VertexTextured(m_worldPos.x + m_edgeL/2,  m_worldPos.y - m_edgeL / 2, m_worldPos.z - m_edgeL / 2, 1.0f, 1.0f),

            // Back Face
            VertexTextured(m_worldPos.x - m_edgeL / 2,  m_worldPos.y - m_edgeL / 2, m_worldPos.z + m_edgeL / 2, 1.0f, 1.0f),
            VertexTextured(m_worldPos.x + m_edgeL / 2,  m_worldPos.y - m_edgeL / 2,  m_worldPos.z + m_edgeL / 2, 0.0f, 1.0f),
            VertexTextured(m_worldPos.x + m_edgeL / 2,  m_worldPos.y + m_edgeL / 2,  m_worldPos.z + m_edgeL / 2, 0.0f, 0.0f),
            VertexTextured(m_worldPos.x - m_edgeL / 2,  m_worldPos.y + m_edgeL / 2,  m_worldPos.z + m_edgeL / 2, 1.0f, 0.0f),

            // Top Face
            VertexTextured(m_worldPos.x - m_edgeL / 2,  m_worldPos.y + m_edgeL / 2, m_worldPos.z - m_edgeL / 2, 0.0f, 1.0f),
            VertexTextured(m_worldPos.x - m_edgeL / 2,  m_worldPos.y + m_edgeL / 2,   m_worldPos.z + m_edgeL / 2, 0.0f, 0.0f),
            VertexTextured(m_worldPos.x + m_edgeL / 2,  m_worldPos.y + m_edgeL / 2,   m_worldPos.z + m_edgeL / 2, 1.0f, 0.0f),
            VertexTextured(m_worldPos.x + m_edgeL / 2,  m_worldPos.y + m_edgeL / 2, m_worldPos.z - m_edgeL / 2, 1.0f, 1.0f),

            // Bottom Face
            VertexTextured(m_worldPos.x - m_edgeL / 2, m_worldPos.y - m_edgeL / 2,  m_worldPos.z - m_edgeL / 2, 1.0f, 1.0f),
            VertexTextured(m_worldPos.x + m_edgeL / 2,m_worldPos.y - m_edgeL / 2,  m_worldPos.z - m_edgeL / 2, 0.0f, 1.0f),
            VertexTextured(m_worldPos.x + m_edgeL / 2, m_worldPos.y - m_edgeL / 2,   m_worldPos.z + m_edgeL / 2, 0.0f, 0.0f),
            VertexTextured(m_worldPos.x - m_edgeL / 2, m_worldPos.y - m_edgeL / 2,   m_worldPos.z + m_edgeL / 2, 1.0f, 0.0f),

            // Left Face
            VertexTextured(m_worldPos.x - m_edgeL / 2, m_worldPos.y - m_edgeL / 2,  m_worldPos.z + m_edgeL / 2, 0.0f, 1.0f),
            VertexTextured(m_worldPos.x - m_edgeL / 2,  m_worldPos.y + m_edgeL / 2,  m_worldPos.z + m_edgeL / 2, 0.0f, 0.0f),
            VertexTextured(m_worldPos.x - m_edgeL / 2,  m_worldPos.y + m_edgeL / 2, m_worldPos.z - m_edgeL / 2, 1.0f, 0.0f),
            VertexTextured(m_worldPos.x - m_edgeL / 2, m_worldPos.y - m_edgeL / 2, m_worldPos.z - m_edgeL / 2, 1.0f, 1.0f),

            // Right Face
            VertexTextured(m_worldPos.x + m_edgeL / 2, m_worldPos.y - m_edgeL / 2, m_worldPos.z - m_edgeL / 2, 0.0f, 1.0f),
            VertexTextured(m_worldPos.x + m_edgeL / 2,  m_worldPos.y + m_edgeL / 2, m_worldPos.z - m_edgeL / 2, 0.0f, 0.0f),
            VertexTextured(m_worldPos.x + m_edgeL / 2,  m_worldPos.y + m_edgeL / 2,  m_worldPos.z + m_edgeL / 2, 1.0f, 0.0f),
            VertexTextured(m_worldPos.x + m_edgeL / 2, m_worldPos.y - m_edgeL / 2,  m_worldPos.z + m_edgeL / 2, 1.0f, 1.0f),
        };
        m_vertexBufferBindable.reset(new Bindable::VertexBufferBindable(resources, v, ARRAYSIZE(v) * sizeof(VertexTextured)));

        DWORD indices[] = {
            // Front Face
            0,  1,  2,
            0,  2,  3,

            // Back Face
            4,  5,  6,
            4,  6,  7,

            // Top Face
            8,  9, 10,
            8, 10, 11,

            // Bottom Face
            12, 13, 14,
            12, 14, 15,

            // Left Face
            16, 17, 18,
            16, 18, 19,

            // Right Face
            20, 21, 22,
            20, 22, 23
        };

        sizeOfInd = ARRAYSIZE(indices);
        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA iinitData;

        iinitData.pSysMem = indices;
        resources->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);

        resources->GetD3DContext()->IASetIndexBuffer(squareIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        textureID = tmanager->CreateTexture(L"images.png");
        


     
    }

	
}