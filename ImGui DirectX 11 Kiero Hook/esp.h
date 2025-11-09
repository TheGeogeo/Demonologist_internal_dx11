#pragma once

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dwmapi.lib")
#pragma comment(lib,"d3dcompiler")

namespace esp
{
	void HealthBar(SDK::FVector2D TopBox, SDK::FVector2D BottomBox, float health, float maxHealth, FLOAT aLineWidth);
	void ArmorBar(SDK::FVector2D TopBox, SDK::FVector2D BottomBox, float armor, float maxArmor, FLOAT aLineWidth);
}

namespace overlay
{
	std::string string_To_UTF8(const std::string& str);
	auto DrawLine(const ImVec2& aPoint1, const ImVec2 aPoint2, ImU32 aColor, const FLOAT aLineWidth) -> VOID;
	auto DrawBox(float x, float y, float w, float h, ImColor color) -> VOID;
	auto RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags) -> VOID;
	auto RectNotFilled(ImVec2 t1, ImVec2 t2, ImVec2 b1, ImVec2 b2, ImColor color, FLOAT aLineWith) -> VOID;
	void HealthBar(ImVec2 t1, ImVec2 b1, float aLineWith, int health);
	void ShieldBar(ImVec2 t1, ImVec2 b1, float aLineWith, int shield);
	auto DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) -> VOID;
	auto DrawCorneredBox(float X, float Y, float W, float H, const ImU32& color, float thickness) -> VOID;
	auto DrawCircle(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor, const FLOAT aLineWidth) -> VOID;
	auto DrawCircle(float x, float y, float radius, ImVec4 color, int segments) -> VOID;
	auto DrawFilledCircle(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor) -> VOID;
	auto DrawString(const ImVec2& aPos, const std::string& aString, ImU32 aColor) -> VOID;
	auto TabButton(const char* label, int* index, int val, bool sameline) -> VOID;
	void DrawCircleFilledCursor();
	void DefaultImGuiItemWidth();
	void SetImGuiItemWidth(float val);
	void Draw3dBox(SDK::AActor* actor, ImColor color, bool isACharacter);
}