#pragma once


namespace GNF::Core::State
{
	class IState
	{
	public:
		virtual ~IState() = default;

		virtual void Init() = 0;

		virtual void OnCreate() = 0;

		virtual void Update(const float dt) = 0;

		virtual void Render() = 0;

		virtual bool IsDestroyed() = 0;

		virtual void Destroy() = 0;
	};
}