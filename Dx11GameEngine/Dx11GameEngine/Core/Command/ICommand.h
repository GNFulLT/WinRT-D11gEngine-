#pragma once


namespace GNF::Core::Command
{
	class ICommand
	{
	public:
		virtual ~ICommand() = default;
		ICommand(const ICommand&) = delete;
		ICommand& operator=(const ICommand&) = delete;

		virtual void Invoke() noexcept = 0;
		virtual bool CanInvoke() noexcept = 0;
		virtual void Undo() noexcept = 0;
	private:
	};
}