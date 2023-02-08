#pragma once

#include <imgui/imgui.h>

namespace GNF::Common::Utils
{
	
	inline void ImGui_LeftLabeledInputFloat(const char* label, float* inputTargetFloat,float rightAlign = 0, float gapBtw = 0.f,const char* labelId = "")
	{
		auto oldPos = ImGui::GetCursorPosY();
		auto newPos = oldPos + ImGui::GetStyle().FramePadding.y;
		auto textSize = ImGui::CalcTextSize(label);
		auto oldPosX = ImGui::GetCursorPosX();
		ImGui::SetCursorPosX(oldPosX - textSize.x + rightAlign);
		ImGui::SetCursorPosY(newPos);
		ImGui::Text(label);
		ImGui::SetCursorPosX(oldPosX + rightAlign + gapBtw);
		ImGui::SetCursorPosY(oldPos);

		ImGui::InputFloat(labelId, inputTargetFloat);
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
	}
	inline void ImGui_Header2(const char* headerText)
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text(headerText);
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
	}



	inline void ImGui_LeftLabeledInputRO(const char* label,char* inputTargetText,size_t buf_size, float rightAlign = 0,float gapBtw = 0.f,const char* labelId = "")
	{
		auto oldPos = ImGui::GetCursorPosY();
		auto newPos = oldPos + ImGui::GetStyle().FramePadding.y;
		auto textSize = ImGui::CalcTextSize(label);
		auto oldPosX = ImGui::GetCursorPosX();
		ImGui::SetCursorPosX(oldPosX - textSize.x + rightAlign);
		ImGui::SetCursorPosY(newPos);
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::SetCursorPosX(oldPosX + rightAlign + gapBtw);
		ImGui::SetCursorPosY(oldPos);
		ImGui::InputText(labelId, inputTargetText, buf_size, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	}

	inline void ImGui_LeftLabeledSlideFloat(const char* label,float* val,float minVal,float maxVal,float rightAlign = 0, float gapBtw = 0.f, const char* labelId = "",const char* format = "%.3f")
	{
		auto oldPos = ImGui::GetCursorPosY();
		auto newPos = oldPos + ImGui::GetStyle().FramePadding.y;
		auto oldPosX = ImGui::GetCursorPosX();
		auto textSize = ImGui::CalcTextSize(label);
		ImGui::SetCursorPosX(oldPosX - textSize.x + rightAlign);
		ImGui::SetCursorPosY(newPos);
		ImGui::Text(label);
		ImGui::SameLine();
		ImGui::SetCursorPosY(oldPos);
		ImGui::SetCursorPosX(oldPosX + rightAlign + gapBtw);
		ImGui::SliderFloat(labelId, val, minVal, maxVal,format);
	}
}