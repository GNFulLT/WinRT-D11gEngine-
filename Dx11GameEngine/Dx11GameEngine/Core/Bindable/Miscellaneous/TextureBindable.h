#pragma once

#include "../IBindable.h"
#include <ITextureLoader.h>
#include <Image/IImage.h>


namespace GNF::Core::Bindable::Miscellaneous
{
	class TextureBindable : public Bindable::IBindable 
	{
	public:
		TextureBindable(const TextureBindable&) = delete;
		TextureBindable& operator =(const TextureBindable&) = delete;
		TextureBindable(Texturing::ITextureLoader* textureLoader,const Image::IImage* img);

		void Bind() override;
		void Bind(ID3D11DeviceContext3* ctx) override;
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView1> m_texture;
	};
}