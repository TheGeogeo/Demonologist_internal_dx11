#include "includes.h"

#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

namespace esp
{
	void HealthBar(SDK::FVector2D TopBox, SDK::FVector2D BottomBox, float health, float maxHealth, FLOAT aLineWidth)
	{
		if (TopBox == SDK::FVector2D() || BottomBox == SDK::FVector2D()) return;
		if (health > 0 && maxHealth > 0)
		{
			float procentageHp = health / maxHealth * 100;
			overlay::HealthBar({ (float)TopBox.X - 4, (float)TopBox.Y }, { (float)BottomBox.X - 4, (float)BottomBox.Y }, aLineWidth, procentageHp);
		}
	}

	void ArmorBar(SDK::FVector2D TopBox, SDK::FVector2D BottomBox, float armor, float maxArmor, FLOAT aLineWidth)
	{
		if (TopBox == SDK::FVector2D() || BottomBox == SDK::FVector2D()) return;
		if (armor > 0 && maxArmor > 0)
		{
			float procentageArmor = armor / maxArmor * 100;
			overlay::ShieldBar({ (float)TopBox.X - 8, (float)TopBox.Y }, { (float)BottomBox.X - 8, (float)BottomBox.Y }, aLineWidth, procentageArmor);
		}
	}
}

namespace overlay
{
	ImFont* Verdana, * DefaultFont;

	typedef struct
	{
		DWORD R;
		DWORD G;
		DWORD B;
		DWORD A;
	}RGBA;

	std::string string_To_UTF8(const std::string& str)
	{
		int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
		wchar_t* pwBuf = new wchar_t[nwLen + 1];
		ZeroMemory(pwBuf, nwLen * 2 + 2);
		::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
		char* pBuf = new char[nLen + 1];
		ZeroMemory(pBuf, nLen + 1);
		::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
		std::string retStr(pBuf);
		delete[]pwBuf;
		delete[]pBuf;
		pwBuf = NULL;
		pBuf = NULL;
		return retStr;
	}

	auto DrawLine(const ImVec2& aPoint1, const ImVec2 aPoint2, ImU32 aColor, const FLOAT aLineWidth) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddLine(aPoint1, aPoint2, aColor, aLineWidth);
	}

	auto DrawBox(float x, float y, float w, float h, ImColor color) -> VOID
	{
		DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, 1.3f); // top
		DrawLine(ImVec2(x, y - 1.3f), ImVec2(x, y + h + 1.4f), color, 1.3f); // left
		DrawLine(ImVec2(x + w, y - 1.3f), ImVec2(x + w, y + h + 1.4f), color, 1.3f);  // right
		DrawLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, 1.3f);   // bottom
	}
	auto RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
	}

	auto RectNotFilled(ImVec2 t1, ImVec2 t2, ImVec2 b1, ImVec2 b2, ImColor color, FLOAT aLineWith) -> VOID
	{
		DrawLine(t1, t2, color, aLineWith);
		DrawLine(t2, b1, color, aLineWith);
		DrawLine(b1, b2, color, aLineWith);
		DrawLine(b2, t1, color, aLineWith);
	}

	void HealthBar(ImVec2 t1, ImVec2 b1, float aLineWith, int health)
	{
		auto vList = ImGui::GetBackgroundDrawList();
		if (!vList) return;  // Guard against null pointers.

		if (health > 100) health = 100;
		float percent = (float)health / 100;
		percent = (b1.y - t1.y) * percent;

		ImColor barColor = ImColor(
			min(510 * (100 - health) / 100, 255),
			min(510 * health / 100, 255),
			25,
			255
		);

		vList->AddLine(b1, { t1.x, b1.y - percent }, barColor, aLineWith);
	}

	void ShieldBar(ImVec2 t1, ImVec2 b1, float aLineWith, int shield)
	{
		auto vList = ImGui::GetBackgroundDrawList();
		if (!vList) return;  // Guard against null pointers.

		if (shield > 100) shield = 100;
		float percent = (float)shield / 100;
		percent = (b1.y - t1.y) * percent;

		vList->AddLine(b1, { t1.x, b1.y - percent }, ImColor{ 10,10,255 }, aLineWith);
	}

	auto DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center) -> VOID
	{
		ImGui::PushFont(Verdana);
		std::stringstream stream(text);
		std::string line;

		float y = 0.0f;
		int index = 0;

		while (std::getline(stream, line))
		{
			ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

			if (center)
			{
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * index) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * index) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
			}
			else
			{//
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * index) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * index) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * index), ImGui::GetColorU32(color), line.c_str());
			}

			y = pos.y + textSize.y * (index + 1);
			index++;
		}
		ImGui::PopFont();
	}

	auto DrawCorneredBox(float X, float Y, float W, float H, const ImU32& color, float thickness) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();

		float lineW = (W / 3);
		float lineH = (H / 3);
		//black outlines
		auto col = ImGui::GetColorU32(color);

		//corners
		vList->AddLine(ImVec2(X, Y - thickness / 2), ImVec2(X, Y + lineH), col, thickness);//top left
		vList->AddLine(ImVec2(X - thickness / 2, Y), ImVec2(X + lineW, Y), col, thickness);

		vList->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W + thickness / 2, Y), col, thickness);//top right horizontal
		vList->AddLine(ImVec2(X + W, Y - thickness / 2), ImVec2(X + W, Y + lineH), col, thickness);

		vList->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H + (thickness / 2)), col, thickness);//bot left
		vList->AddLine(ImVec2(X - thickness / 2, Y + H), ImVec2(X + lineW, Y + H), col, thickness);

		vList->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W + thickness / 2, Y + H), col, thickness);//bot right
		vList->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H + (thickness / 2)), col, thickness);
	}

	auto DrawCircle(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor, const FLOAT aLineWidth) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddCircle(aPoint, aR, aColor, 120, aLineWidth);
	}
	auto DrawCircle(float x, float y, float radius, ImVec4 color, int segments) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments);
	}
	auto DrawFilledCircle(const ImVec2& aPoint, const FLOAT aR, ImU32 aColor) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddCircleFilled(aPoint, aR, aColor, 120);
	}
	auto DrawString(const ImVec2& aPos, const std::string& aString, ImU32 aColor) -> VOID
	{
		auto vList = ImGui::GetBackgroundDrawList();
		vList->AddText(aPos, aColor, aString.data());
	}
	auto TabButton(const char* label, int* index, int val, bool sameline) -> VOID
	{
		if (*index == val)
		{
			if (ImGui::Button(label, ImVec2(80, 25)))
				*index = val;
			if (sameline)
				ImGui::SameLine();
		}
		else
		{
			if (ImGui::Button(label, ImVec2(80, 25)))
				*index = val;
			if (sameline)
				ImGui::SameLine();
		}
	}

	void DrawCircleFilledCursor()
	{
		auto vList = ImGui::GetForegroundDrawList();

		POINT p;
		GetCursorPos(&p);

		vList->AddCircleFilled(ImVec2{ (float)p.x, (float)p.y }, 4.f, ImColor{ 255,20,20 });
	}

	void DefaultImGuiItemWidth()
	{
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
	}

	void SetImGuiItemWidth(float val)
	{
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * val);
	}

	static void Draw3dBox_FirstVersion(SDK::AActor* actor, ImColor color)
	{
		if (!actor) return;

		SDK::FVector origin, extent;
		actor->GetActorBounds(true, &origin, &extent, true);
		SDK::FRotator rotation = actor->K2_GetActorRotation();
		float yaw = ue->MathLib->DegreesToRadians((int)(rotation.Yaw + 450.0f) % 360);

		SDK::FVector t1 = origin, t2 = origin, t3 = origin, t4 = origin, b1 = origin, b2 = origin, b3 = origin, b4 = origin;
		t1.X -= 35.f; t1.Y -= 35.f; t1.Z -= extent.Z;
		t2.X += 35.f; t2.Y -= 35.f; t2.Z -= extent.Z;
		t3.X += 35.f; t3.Y += 35.f; t3.Z -= extent.Z;
		t4.X -= 35.f; t4.Y += 35.f; t4.Z -= extent.Z;

		t1 = ue->RotateCorner(origin, t1, yaw);
		t2 = ue->RotateCorner(origin, t2, yaw);
		t3 = ue->RotateCorner(origin, t3, yaw);
		t4 = ue->RotateCorner(origin, t4, yaw);

		SDK::FVector2D ts1, ts2, ts3, ts4;
		if (!ue->PlayerController->ProjectWorldLocationToScreen(t1, &ts1, false) ||
			!ue->PlayerController->ProjectWorldLocationToScreen(t2, &ts2, false) ||
			!ue->PlayerController->ProjectWorldLocationToScreen(t3, &ts3, false) ||
			!ue->PlayerController->ProjectWorldLocationToScreen(t4, &ts4, false))
		{
			return;
		}

		b1.X -= 35.f; b1.Y -= 35.f; b1.Z += extent.Z;
		b2.X += 35.f; b2.Y -= 35.f; b2.Z += extent.Z;
		b3.X += 35.f; b3.Y += 35.f; b3.Z += extent.Z;
		b4.X -= 35.f; b4.Y += 35.f; b4.Z += extent.Z;

		b1 = ue->RotateCorner(origin, b1, yaw);
		b2 = ue->RotateCorner(origin, b2, yaw);
		b3 = ue->RotateCorner(origin, b3, yaw);
		b4 = ue->RotateCorner(origin, b4, yaw);

		SDK::FVector2D bs1, bs2, bs3, bs4;
		if (!ue->PlayerController->ProjectWorldLocationToScreen(b1, &bs1, false) ||
			!ue->PlayerController->ProjectWorldLocationToScreen(b2, &bs2, false) ||
			!ue->PlayerController->ProjectWorldLocationToScreen(b3, &bs3, false) ||
			!ue->PlayerController->ProjectWorldLocationToScreen(b4, &bs4, false))
		{
			return;
		}

		auto L = [&](SDK::FVector2D a, SDK::FVector2D b) {
			overlay::DrawLine({ (float)a.X,(float)a.Y }, { (float)b.X,(float)b.Y }, color, 1.f);
			};

		L(ts1, ts2); L(ts2, ts3); L(ts3, ts4); L(ts4, ts1);
		L(bs1, bs2); L(bs2, bs3); L(bs3, bs4); L(bs4, bs1);
		L(ts1, bs1); L(ts2, bs2); L(ts3, bs3); L(ts4, bs4);
	}

	// Construit les axes monde d'un rotateur (équivalent FRotationMatrix)
	// Helper: construit un repère (X=avant, Y=droite, Z=haut) à partir d’un rotateur
	static inline void BuildBasisFromYaw(const SDK::FRotator& r, SDK::FVector& fwd, SDK::FVector& rgt, SDK::FVector& up)
	{
		const double yaw = ue->MathLib->DegreesToRadians(std::fmod(r.Yaw + 360.0, 360.0));
		const double cy = std::cos(yaw), sy = std::sin(yaw);
		fwd = SDK::FVector(cy, sy, 0.0);
		rgt = SDK::FVector(-sy, cy, 0.0);
		up = SDK::FVector(0.0, 0.0, 1.0);
	}

	// === Version "bones" ===
	// Retourne true si tracée depuis les bones (sinon false => fallback)
	static bool Draw3dBox_FromBones(SDK::USkeletalMeshComponent* mesh, SDK::AActor* actor, ImColor color)
	{
		if (!mesh || !actor) return false;

		// 1) Repère de la box aligné sur le yaw de l’actor (lisible en jeu)
		SDK::FVector F, R, U;
		BuildBasisFromYaw(actor->K2_GetActorRotation(), F, R, U);

		// 2) Itération des bones via l'API SkinnedMesh (présente dans ton SDK)  [GetNumBones/GetBoneName/GetBoneTransform]
		const int32_t nb = mesh->GetNumBones();           // :contentReference[oaicite:1]{index=1}
		if (nb <= 0) return false;

		// Accumulateurs min/max dans le repère (R,F,U)
		double minR = std::numeric_limits<double>::infinity();
		double maxR = -std::numeric_limits<double>::infinity();
		double minF = std::numeric_limits<double>::infinity();
		double maxF = -std::numeric_limits<double>::infinity();
		double minU = std::numeric_limits<double>::infinity();
		double maxU = -std::numeric_limits<double>::infinity();

		int used = 0;

		for (int32_t i = 0; i < nb; ++i)
		{
			const auto boneName = mesh->GetBoneName(i);   // :contentReference[oaicite:2]{index=2}

			// NOTE:
			// - Dans la plupart des SDK UE, ERelativeTransformSpace::World (ou RTS_World) vaut 0.
			// - Adapte l’énum si besoin (World / Component). L’essentiel est d’obtenir une position monde.
			const auto xform = mesh->GetBoneTransform(boneName, (SDK::ERelativeTransformSpace)0); // monde  :contentReference[oaicite:3]{index=3}

			// Selon ton FTransform, utilise .Translation (ou .GetLocation())
			const SDK::FVector p = xform.Translation;

			// Optionnel: skip NaN/inf
			if (!std::isfinite(p.X) || !std::isfinite(p.Y) || !std::isfinite(p.Z))
				continue;

			// 3) Projette le point os sur (R,F,U)
			const double pr = p.Dot(R);
			const double pf = p.Dot(F);
			const double pu = p.Dot(U);

			if (pr < minR) minR = pr; if (pr > maxR) maxR = pr;
			if (pf < minF) minF = pf; if (pf > maxF) maxF = pf;
			if (pu < minU) minU = pu; if (pu > maxU) maxU = pu;

			++used;
		}

		if (used < 1) return false;

		// 4) Marges demandées : étirer le bas de -5 et le haut de +5 => hauteur totale +10
		minU -= 5.0;
		maxU += 5.0;

		// 5) Centre et demi-extensions
		const double cR = 0.5 * (minR + maxR);
		const double cF = 0.5 * (minF + maxF);
		const double cU = 0.5 * (minU + maxU);

		const double eR = 0.5 * (maxR - minR);
		const double eF = 0.5 * (maxF - minF);
		const double eU = 0.5 * (maxU - minU);

		// 6) Centre monde reconstruit
		const SDK::FVector origin = (R * cR) + (F * cF) + (U * cU);

		// 7) Sommets de l’OBB (orientée yaw)
		const SDK::FVector topCenter = origin + (U * eU);
		const SDK::FVector bottomCenter = origin - (U * eU);

		const SDK::FVector offs[4] = {
			(R * -eR) + (F * -eF),
			(R * eR) + (F * -eF),
			(R * eR) + (F * eF),
			(R * -eR) + (F * eF),
		};

		SDK::FVector corners3D[8] = {
			topCenter + offs[0],
			topCenter + offs[1],
			topCenter + offs[2],
			topCenter + offs[3],
			bottomCenter + offs[0],
			bottomCenter + offs[1],
			bottomCenter + offs[2],
			bottomCenter + offs[3],
		};

		// 8) Projection & tracé
		SDK::FVector2D c2D[8];
		for (int i = 0; i < 8; ++i)
			if (!ue->PlayerController->ProjectWorldLocationToScreen(corners3D[i], &c2D[i], false))
				return true; // calcul OK (bones), juste pas affichable ce frame

		auto L = [&](int a, int b) {
			overlay::DrawLine({ (float)c2D[a].X, (float)c2D[a].Y },
				{ (float)c2D[b].X, (float)c2D[b].Y }, color, 1.f);
			};

		// Top / Bottom / Montants
		L(0, 1); L(1, 2); L(2, 3); L(3, 0);
		L(4, 5); L(5, 6); L(6, 7); L(7, 4);
		L(0, 4); L(1, 5); L(2, 6); L(3, 7);

		return true;
	}

	// === Wrapper demandé ===
	void Draw3dBox(SDK::AActor* actor, ImColor color, bool isACharacter)
	{
		if (!actor) return;

		// 1) Si on a un ACharacter, essayer la version bones
		if (isACharacter)
		{
			SDK::ACharacter* character = reinterpret_cast<SDK::ACharacter*>(actor);
			if (character && character->mesh)
			{
				if (Draw3dBox_FromBones(character->mesh, character, color))
					return;
			}
		}

		// 2) Fallback : ta toute première version
		Draw3dBox_FirstVersion(actor, color);
	}
}