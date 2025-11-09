#include "includes.h"
#include "kiero/minhook/include/MinHook.h"
#include "HookFunction.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Toujours feed ImGui (il mettra à jour io.WantCapture...)
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	if (CFG.b_MenuShow)
	{
		switch (uMsg)
		{
			// Clavier
		case WM_KEYDOWN: case WM_KEYUP:
		case WM_SYSKEYDOWN: case WM_SYSKEYUP:
		case WM_CHAR: case WM_SYSCHAR:
			// Souris
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN: case WM_LBUTTONUP: case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN: case WM_RBUTTONUP:
		case WM_MBUTTONDOWN: case WM_MBUTTONUP:
		case WM_XBUTTONDOWN:  case WM_XBUTTONUP:
		case WM_MOUSEWHEEL:   case WM_MOUSEHWHEEL:
			// Raw Input (nombreux jeux FPS)
		case WM_INPUT:
			return 1; // avale l’input => le jeu ne reçoit rien
		}
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void GettingActors()
{
	if (nullptr == ue->World) return;

	SDK::UClass* staticTool = SDK::ABP_Tool_C::StaticClass();
	SDK::UClass* staticRoomIdentifier = SDK::ABP_RoomIdentifier_C::StaticClass();
	SDK::UClass* staticEntity = SDK::ABP_Entity_C::StaticClass();
	SDK::UClass* staticPlayer = SDK::ABP_PlayerCharacter_C::StaticClass();
	SDK::UClass* staticLobbyPlayer = SDK::ABP_LobbyCharacter_C::StaticClass();
	SDK::UClass* staticSpectatorPlayer = SDK::ABP_SpectatorCharacter_C::StaticClass();
	SDK::UClass* staticHidingSpotVisual = SDK::ABP_HidingSpotVisual_C::StaticClass();

	for (SDK::ULevel* level : ue->World->Levels)
	{
		for (SDK::AActor* actor : level->Actors)
		{
			if (!actor)
				continue;

			if (CFG.debugAllActors)
			{
				if (CFG.debugAllActorsOnlyCharacter && actor->IsA(SDK::ACharacter::StaticClass()))
				{
					CFG.v_allActorsDebug.push_back(actor);
				}
				else if (!CFG.debugAllActorsOnlyCharacter)
				{
					CFG.v_allActorsDebug.push_back(actor);
				}
			}

			if (staticTool && actor->IsA(staticTool))
			{
				CFG.v_allTools.push_back(actor);
			}
			/*else if (staticRoomIdentifier && actor->IsA(staticRoomIdentifier))
			{
				CFG.v_allRoomIdentifiers.push_back(actor);
			}*/
			else if (staticEntity && actor->IsA(staticEntity))
			{
				CFG.v_allEntities.push_back(actor);
			}
			else if (staticPlayer && actor->IsA(staticPlayer))
			{
				CFG.v_allPlayers.push_back(actor);
			}
			else if (staticLobbyPlayer && actor->IsA(staticLobbyPlayer))
			{
				CFG.v_allLobbyPlayers.push_back(actor);
			}
			else if (staticSpectatorPlayer && actor->IsA(staticSpectatorPlayer))
			{
				CFG.v_allSpectatorPlayers.push_back(actor);
			}
			else if (staticHidingSpotVisual && actor->IsA(staticHidingSpotVisual))
			{
				CFG.v_allHidingSpotVisuals.push_back(actor);
			}
		}
	}
}

void ClearAllArray()
{
	CFG.v_allActorsDebug.clear();
	CFG.v_allTools.clear();
	//CFG.v_allRoomIdentifiers.clear();
	CFG.v_allEntities.clear();
	CFG.v_allPlayers.clear();
	CFG.v_allLobbyPlayers.clear();
	CFG.v_allSpectatorPlayers.clear();
	CFG.v_allHidingSpotVisuals.clear();
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;

			utilFunction::ChangeStyleImGui(&ImGui::GetStyle());
		}
		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ue->ReDefineAttribut();
	hotkey::AsyncHotkey();
	GettingActors();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//-------
	MenuGUI::Menu();
	CheatMain::Cheat();
	//-------

	ImGui::Render();
	ClearAllArray();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

void HookCall()
{
	// ---- Define adress function ----
	/*hookOffsets::baseGame = (uintptr_t)GetModuleHandleA(NULL);

	SDK::UFunction* KillTargetPlayerFunc = SDK::ABP_Entity_C::StaticClass()->GetFunction("BP_Entity_C", "KillTargetPlayer");

	if (KillTargetPlayerFunc)
		hookOffsets::KillTargetPlayer = (uintptr_t)KillTargetPlayerFunc->ExecFunction;*/

		// ----------------------------------

		/*printf("[ DEBUG ] baseGame Address: 0x%llX\n", hookOffsets::baseGame);
		printf("[ DEBUG ] KillTargetPlayer Address: 0x%llX\n", hookOffsets::KillTargetPlayer);
		printf("----------------------------------------------------------\n");
		printf("\n");

		if (MH_CreateHook(reinterpret_cast<LPVOID*>(hookOffsets::KillTargetPlayer), hookFunction::KillTargetPlayer_Hook, (LPVOID*)&hookFunction::KillTargetPlayer_o) == MH_OK)
		{
			if (MH_EnableHook(reinterpret_cast<LPVOID*>(hookOffsets::KillTargetPlayer)) != MH_OK)
			{
				std::cout << "KillTargetPlayer failed enable" << std::endl;
			}
		}
		else
		{
			std::cout << "KillTargetPlayer failed hook" << std::endl;
		}*/
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	utilFunction::PreThread();

	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			HookCall();
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		/*AllocConsole();
		FILE* fileptr;
		freopen_s(&fileptr, "CONOUT$", "w", stdout);
		freopen_s(&fileptr, "CONOUT$", "w", stderr);
		freopen_s(&fileptr, "CONIN$", "r", stdin);*/

		//SDK::InitGObjects();

		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}