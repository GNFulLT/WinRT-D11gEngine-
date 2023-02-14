#include "pch.h"

#include "CommandManager.h"

namespace GNF::Core::Command
{
    void CommandManager::ExecuteCommand(ICommand* command)
    {
        command->Invoke();
        m_undoStack.emplace(std::move(command));
        while (!m_redoStack.empty())
            m_redoStack.pop();
    }

    void CommandManager::Undo()
    {
        if (m_undoStack.empty())
            return;

        auto command = std::move(m_undoStack.top());
        m_undoStack.pop();
        command->Undo();
        m_redoStack.push(std::move(command));
    }

    void CommandManager::Redo()
    {
        if (m_redoStack.empty())
            return;

        auto command = std::move(m_redoStack.top());
        m_redoStack.pop();
        command->Invoke();
        m_undoStack.push(std::move(command));
    }

    bool CommandManager::CanUndo() const
    {
        return !m_undoStack.empty();
    }

    bool CommandManager::CanRedo() const
    {
        return !m_redoStack.empty();
    }
}