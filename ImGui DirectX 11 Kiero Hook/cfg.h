#pragma once

inline namespace Configuration
{
	class Settings : public Singleton<Settings>
	{
	public:
		bool b_MenuShow = true;

		//player
		bool b_playerRect = false;
		bool b_playerName = false;
		bool b_player3dBox = false;
		bool b_playerSkeleton = false;
		bool b_playerShowDead = false;
		bool b_playerInformation = false;

		//ghost
		bool b_ghostCircle = false;
		bool b_ghost3dBox = false;
		bool b_ghostSkeleton = false;
		bool b_ghostShowSelf = false;
		bool b_ghostInformation = false;
		bool b_ghostFavouriteRoom = false;
		bool b_ghostEvent = false;
		bool b_ghostThrowItemEvent = false;
		bool b_ghostHuntEvent = false;
		bool b_ghostTeleportToFavouriteRoom = false;

		//ghost value
		const std::vector<int32_t> ghostToSkip = { 0 };

		//object
		bool b_objectTools = false;
		bool b_objectToolsPickup = false;
		bool b_objectHiddingSpotVisual = false;
		bool b_objectHiddingSpotVisual3D = false;

		//color esp
		ImColor boxVisiblePlayerColor = ImColor{ 55,255,0 };
		float f_boxVisiblePlayerColor[3] = { boxVisiblePlayerColor.Value.x, boxVisiblePlayerColor.Value.y, boxVisiblePlayerColor.Value.z };

		ImColor boxNotVisiblePlayerColor = ImColor{ 255,55,0 };
		float f_boxNotVisiblePlayerColor[3] = { boxNotVisiblePlayerColor.Value.x, boxNotVisiblePlayerColor.Value.y, boxNotVisiblePlayerColor.Value.z };

		ImColor boxVisibleGhostColor = ImColor{ 255,0,0 };
		float f_boxVisibleGhostColor[3] = { boxVisibleGhostColor.Value.x, boxVisibleGhostColor.Value.y, boxVisibleGhostColor.Value.z };

		ImColor boxNotVisibleGhostColor = ImColor{ 255,55,0 };
		float f_boxNotVisibleGhostColor[3] = { boxNotVisibleGhostColor.Value.x, boxNotVisibleGhostColor.Value.y, boxNotVisibleGhostColor.Value.z };

		ImColor skeletonPlayerColor = ImColor{ 0, 234, 255 };
		float f_skeletonPlayerColor[3] = { skeletonPlayerColor.Value.x, skeletonPlayerColor.Value.y, skeletonPlayerColor.Value.z };

		ImColor skeletonGhostColor = ImColor{ 0, 234, 255 };
		float f_skeletonGhostColor[3] = { skeletonGhostColor.Value.x, skeletonGhostColor.Value.y, skeletonGhostColor.Value.z };

		ImColor toolColor = ImColor{ 143, 79, 200 };
		float f_toolColor[3] = { toolColor.Value.x, toolColor.Value.y, toolColor.Value.z };

		ImColor hidingSpotVisualColor = ImColor{ 252, 186, 3 };
		float f_hidingSpotVisualColor[3] = { hidingSpotVisualColor.Value.x, hidingSpotVisualColor.Value.y, hidingSpotVisualColor.Value.z };

		//misc
		bool b_miscFullbright = false;
		bool b_miscInfStamina = false; // stamina not replicated
		bool b_miscNoClip = false;

		//troll
		std::map<std::string, bool> map_forceFlashlightByName;

		//save
		bool b_saveMoney = false;
		bool b_saveLevel = false;

		//save value
		double saveMoneyValue = 0.0;
		double saveLevelValue = 0.0;

		//debug
		int tabIndex = 0;
		bool debugAllActors = false;
		bool debugAllActorsOnlyCharacter = false;
		bool debugAllActorsOnlyCharacterBones = false;
		float debugDistanceMax = 1000.f;

		// Vector of actors
		std::vector<SDK::AActor*> v_allActorsDebug = {};
		std::vector<SDK::AActor*> v_allTools = {};
		std::vector<SDK::AActor*> v_allRoomIdentifiers = {};
		std::vector<SDK::AActor*> v_allEntities = {};
		std::vector<SDK::AActor*> v_allPlayers = {};
		std::vector<SDK::AActor*> v_allLobbyPlayers = {};
		std::vector<SDK::AActor*> v_allSpectatorPlayers = {};
		std::vector<SDK::AActor*> v_allHidingSpotVisuals = {};

		//GAMEVARS
		const char* dwProcessName = "PAYDAY3Client-Win64-Shipping.exe";
		DWORD dwProcessId = NULL;
		uint64_t dwProcess_Base = NULL;
		HWND gameHWND = NULL;

		int ScreenHeight = NULL;
		int ScreenWidth = NULL;

		// color
		ImColor colorWhite = ImColor{ 255,255,255 };
		ImColor colorBlack = ImColor{ 0,0,0 };
		ImColor colorRed = ImColor{ 255,0,0 };
		ImColor colorGreen = ImColor{ 0,255,0 };
		ImColor colorBlue = ImColor{ 0,0,255 };
	};
#define CFG Configuration::Settings::Get()
}