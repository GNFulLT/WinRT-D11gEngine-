#pragma once

#include "ICommand.h"
#include <stack>
#include <memory>

namespace GNF::Core::Command
{
	class CommandManager
	{
	public:
        void ExecuteCommand(ICommand* command);

        void Undo();

        void Redo();

		bool CanUndo() const;

        bool CanRedo() const;
	private:
		std::stack<std::unique_ptr<ICommand>> m_undoStack;
		std::stack<std::unique_ptr<ICommand>> m_redoStack;
	};
}