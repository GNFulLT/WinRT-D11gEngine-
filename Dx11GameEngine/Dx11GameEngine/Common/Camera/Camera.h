#pragma once

#include "./CameraPositioner/ICameraPositioner.h"
#include "Core/Bindable/Buffer/ConstantBufferBindable.h"
#include <memory>
#include "boost/align/detail/is_aligned.hpp"

namespace GNF::Common::Camera
{
	//!: Don't needed to add custom data to Per Frame Structure.
	//! 
	/*
	template<typename T,typename SFINAE = std::true_type>
	class Camera
	{};

	template<typename T,
		std::enable_if_t<alignof(T)> = 16
	>
	class Camera
	{
		struct PerFrameData
		{
			DirectX::SimpleMath::Matrix mvp;
			T dataStruct;
		};

	public:
		//!: Cam doesn't own your struct data
		//! Custum data for per frame data. Custom data will be added after MVP Structure
		Camera(CameraPositioner::ICameraPositioner* camPositioner,const T& initialData);
		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;

		void SetCameraPositioner(CameraPositioner::ICameraPositioner* positioner);
		CameraPositioner::ICameraPositioner* GetSelectedCameraPositioner();
		void Update(const T& initialData,const float deltaTime);

	private:
		CameraPositioner::ICameraPositioner* p_positioner;
		std::unique_ptr<Core::Bindable::Buffer::ConstantBufferBindable> m_constBuffer;
	};
	*/
	class Camera
	{
		
		struct alignas(16) PerFrameData
		{
			DirectX::SimpleMath::Matrix mvp;
			//float totalTime = 0;
		};

	public:
		//!: Camera isn't responsible for destructing ICameraPositioner It's just a wrapper class
		//!: Camera Sets total time in Update func
		Camera(CameraPositioner::ICameraPositioner* camPositioner);
		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;

		void SetCameraPositioner(CameraPositioner::ICameraPositioner* positioner);
		CameraPositioner::ICameraPositioner* GetSelectedCameraPositioner();
		void Update(const float deltaTime);
		void Use(const DirectX::SimpleMath::Matrix& modelMatrix);
		void RenderGui();
	private:
		CameraPositioner::ICameraPositioner* p_positioner;
		std::unique_ptr<Core::Bindable::Buffer::ConstantBufferBindable> m_constBuffer;
		PerFrameData data;
		
	};

}