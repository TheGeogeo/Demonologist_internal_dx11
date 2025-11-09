#pragma once

namespace utilFunction
{
	uintptr_t pattern_scan(std::uintptr_t Address, const char* Signature, bool Relative);
	void ChangeStyleImGui(ImGuiStyle* ptrStyle);
	std::string AddressForImGui(uintptr_t address);
	void PreThread();
	std::string to_str2(double x);
}