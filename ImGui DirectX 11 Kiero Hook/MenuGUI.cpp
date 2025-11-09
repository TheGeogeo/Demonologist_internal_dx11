#include "includes.h"

namespace MenuGUI
{
	void Menu()
	{
		ImGui::GetIO().MouseDrawCursor = CFG.b_MenuShow;

		if (CFG.b_MenuShow)
		{
			ImGui::SetNextWindowSize(ImVec2(675, 530));
			ImGui::Begin("Demonologist internal | TheGeogeo", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

			overlay::TabButton("Player##menu_player", &CFG.tabIndex, 0, true);
			overlay::TabButton("Ghost##menu_ghost", &CFG.tabIndex, 1, true);
			overlay::TabButton("Object##menu_object", &CFG.tabIndex, 6, true);
			overlay::TabButton("Misc##menu_misc", &CFG.tabIndex, 2, true);
			overlay::TabButton("Troll##menu_troll", &CFG.tabIndex, 3, true);
			overlay::TabButton("Save##menu_save", &CFG.tabIndex, 4, true);
			overlay::TabButton("Settings##menu_settings", &CFG.tabIndex, 5, true);
			//overlay::TabButton("Debug##menu_debug", &CFG.tabIndex, 9, true);

			ImGui::Separator();

			//stuff GUI
			if (CFG.tabIndex == 0) // player
			{
				ImGui::Checkbox("2D Box##player_2dbox", &CFG.b_playerRect);
				ImGui::Checkbox("3D Box##player_3dbox", &CFG.b_player3dBox);
				ImGui::Checkbox("Name##player_name", &CFG.b_playerName);
				ImGui::Checkbox("Skeleton##player_skeleton", &CFG.b_playerSkeleton);
				ImGui::Checkbox("Render dead##player_show_dead", &CFG.b_playerShowDead);
				ImGui::Checkbox("Global information##player_global_information", &CFG.b_playerInformation);
			}

			if (CFG.tabIndex == 1) // ghost
			{
				ImGui::Checkbox("Circle##ghost_circle", &CFG.b_ghostCircle);
				ImGui::Checkbox("3D box##ghost_3dbox", &CFG.b_ghost3dBox);
				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::Checkbox("Favourite room##ghost_favourite_room", &CFG.b_ghostFavouriteRoom);
				ImGui::Checkbox("Global information##ghost_information", &CFG.b_ghostInformation);
				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::Text("If you are the HOST it's for all players");
				if (ImGui::Button("Tp to favourite room##ghost_tp_favourite_room"))
				{
					CFG.b_ghostTeleportToFavouriteRoom = true;
				}
				if (ImGui::Button("Appear##ghost_show_self"))
				{
					CFG.b_ghostShowSelf = true;
				}
				if (ImGui::Button("Random event##ghost_event"))
				{
					CFG.b_ghostEvent = true;
				}
				if (ImGui::Button("Throw item##ghost_throw_item"))
				{
					CFG.b_ghostThrowItemEvent = true;
				}
				if (ImGui::Button("Hunt##ghost_hunt_event"))
				{
					CFG.b_ghostHuntEvent = true;
				}
			}

			if (CFG.tabIndex == 6) // object
			{
				ImGui::Checkbox("Tools##player_items", &CFG.b_objectTools);
				ImGui::Checkbox("Pickup Tool (host)##player_pickup_items", &CFG.b_objectToolsPickup);
				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::Checkbox("Closet circle##player_hiding_sport_circle", &CFG.b_objectHiddingSpotVisual);
				ImGui::Checkbox("Closet 3D##player_hiding_sport_3d", &CFG.b_objectHiddingSpotVisual3D);
			}

			if (CFG.tabIndex == 2) // Misc
			{
				ImGui::Checkbox("Inf stamina", &CFG.b_miscInfStamina);
				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::Checkbox("Fullbright", &CFG.b_miscFullbright);
				if (ImGui::Checkbox("NoClip (host)", &CFG.b_miscNoClip) && ue->PlayerController)
				{
					if (!CFG.b_miscNoClip)
					{
						ue->ToggleNoClip(CFG.b_miscNoClip);
					}
				}
			}

			if (CFG.tabIndex == 3) // Troll
			{
				ImGui::Text("You need to be the host");
				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				for (size_t i = 0; i < CFG.v_allPlayers.size(); i++)
				{
					SDK::ABP_PlayerCharacter_C* player = reinterpret_cast<SDK::ABP_PlayerCharacter_C*>(CFG.v_allPlayers[i]);
					if (!player) continue;
					if (!ue->KismetSystemLib->IsValid(player)) continue;

					if (player->PlayerState && ue->KismetSystemLib->IsValid(player->PlayerState))
					{
						SDK::ABP_Shivers_PlayerStateBase_C* playerStateBase = reinterpret_cast<SDK::ABP_Shivers_PlayerStateBase_C*>(player->PlayerState);
						std::string playerName = playerStateBase->PlayerNamePrivate.ToString();

						ImGui::Text(playerName.c_str());

						std::string tpToYouBtnStr = "Tp to you##tp_to_you" + std::to_string(i);
						if (ImGui::Button(tpToYouBtnStr.c_str()) && ue->myCharacter)
						{
							player->K2_TeleportTo(ue->myCharacter->K2_GetActorLocation(), ue->myCharacter->K2_GetActorRotation());
						}

						std::string tpToPlayerBtnStr = "Tp to player##tp_to_player" + std::to_string(i);
						if (ImGui::Button(tpToPlayerBtnStr.c_str()) && ue->myCharacter)
						{
							ue->myCharacter->K2_TeleportTo(player->K2_GetActorLocation(), player->K2_GetActorRotation());
						}

						float playerSanity = (float)player->PlayerSanity;
						if (ImGui::SliderFloat(("Sanity##player_set_sanity" + std::to_string(i)).c_str(), &playerSanity, 0.00f, 100.00f, "%.2f"))
						{
							player->PlayerSanity = (double)playerSanity;
						}

						//-- ForceFlashlight
						bool bForceFlashlight = false;
						if (auto it = CFG.map_forceFlashlightByName.find(playerName); it != CFG.map_forceFlashlightByName.end()) {
							bForceFlashlight = it->second;
						}
						else {
							CFG.map_forceFlashlightByName[playerName] = false;
						}
						if (ImGui::Checkbox(("Force flashlight##troll_force_flashlight" + std::to_string(i)).c_str(), &bForceFlashlight))
						{
							if (bForceFlashlight)
								CFG.map_forceFlashlightByName[playerName] = true;
							else
								CFG.map_forceFlashlightByName[playerName] = false;
						}
						//-- end ForceFlashlight

						bool b_tpPlayerToEnt = false;
						std::string tpToEntBtnStr = "Tp to entity##tp_to_entity" + std::to_string(i);
						if (ImGui::Button(tpToEntBtnStr.c_str()))
							b_tpPlayerToEnt = true;

						for (size_t i = 0; i < CFG.v_allEntities.size(); i++)
						{
							SDK::ABP_Entity_C* entity = reinterpret_cast<SDK::ABP_Entity_C*>(CFG.v_allEntities[i]);
							if (!entity) continue;
							if (!ue->KismetSystemLib->IsValid(entity)) continue;

							if (b_tpPlayerToEnt)
							{
								player->K2_TeleportTo(entity->K2_GetActorLocation(), player->K2_GetActorRotation());
								b_tpPlayerToEnt = false;
							}
						}

						ImGui::Dummy(ImVec2(0.0f, 10.0f));
					}
				}
			}

			if (CFG.tabIndex == 4) // Save
			{
				ImGui::Text("After making changes, switch lobbies to load the modifications or buy something.");

				//-- money
				ImGui::SetNextItemWidth(160);
				if (ImGui::InputDouble("Money value", &CFG.saveMoneyValue, 1.0, 10.0, "%.0f"))
				{
					CFG.saveMoneyValue = std::round(CFG.saveMoneyValue);
					if (CFG.saveMoneyValue < 0.0) CFG.saveMoneyValue = 0.0;
				}
				ImGui::SameLine();
				if (ue->gameInstance)
				{
					if (ImGui::Button("Save money") && ue->gameInstance)
					{
						CFG.b_saveMoney = true;
					}
				}
				else {
					ImGui::Text("GameInstance not found");
				}
				//-- end money

				//-- exp
				ImGui::SetNextItemWidth(160);
				if (ImGui::InputDouble("Level value", &CFG.saveLevelValue, 1.0, 10.0, "%.0f"))
				{
					CFG.saveLevelValue = std::round(CFG.saveLevelValue);
					if (CFG.saveLevelValue < 0.0) CFG.saveLevelValue = 0.0;
				}
				ImGui::SameLine();
				if (ue->gameInstance)
				{
					if (ImGui::Button("Save level") && ue->gameInstance)
					{
						CFG.b_saveLevel = true;
					}
				}
				else {
					ImGui::Text("GameInstance not found");
				}
				//-- end exp
			}

			if (CFG.tabIndex == 5) // Settings
			{
				if (ImGui::ColorEdit3("Player esp color", CFG.f_boxVisiblePlayerColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.boxVisiblePlayerColor = ImColor(CFG.f_boxVisiblePlayerColor[0], CFG.f_boxVisiblePlayerColor[1], CFG.f_boxVisiblePlayerColor[2]);
				}

				if (ImGui::ColorEdit3("Ghost esp color", CFG.f_boxVisibleGhostColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.boxVisibleGhostColor = ImColor(CFG.f_boxVisibleGhostColor[0], CFG.f_boxVisibleGhostColor[1], CFG.f_boxVisibleGhostColor[2]);
				}

				if (ImGui::ColorEdit3("Tool esp color", CFG.f_toolColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.toolColor = ImColor(CFG.f_toolColor[0], CFG.f_toolColor[1], CFG.f_toolColor[2]);
				}

				if (ImGui::ColorEdit3("Closet esp color", CFG.f_hidingSpotVisualColor, ImGuiColorEditFlags_NoDragDrop))
				{
					CFG.hidingSpotVisualColor = ImColor(CFG.f_hidingSpotVisualColor[0], CFG.f_hidingSpotVisualColor[1], CFG.f_hidingSpotVisualColor[2]);
				}
			}

			if (CFG.tabIndex == 9) // Debug
			{
				std::string gworldDebugStr = "GWorld: " + utilFunction::AddressForImGui((uintptr_t)ue->GWorld);
				ImGui::Text(gworldDebugStr.c_str());

				std::string worldDebugStr = "World: " + utilFunction::AddressForImGui((uintptr_t)ue->World);
				ImGui::Text(worldDebugStr.c_str());

				std::string levelDebugStr = "Level: " + utilFunction::AddressForImGui((uintptr_t)ue->level);
				ImGui::Text(levelDebugStr.c_str());

				std::string gameStateDebugStr = "GameState: " + utilFunction::AddressForImGui((uintptr_t)ue->GameState);
				ImGui::Text(gameStateDebugStr.c_str());

				std::string gameInstanceDebugStr = "gameInstance: " + utilFunction::AddressForImGui((uintptr_t)ue->gameInstance);
				ImGui::Text(gameInstanceDebugStr.c_str());

				std::string localPlayerDebugStr = "LocalPlayer: " + utilFunction::AddressForImGui((uintptr_t)ue->LocalPlayer);
				ImGui::Text(localPlayerDebugStr.c_str());

				std::string playerControllerDebugStr = "PlayerController: " + utilFunction::AddressForImGui((uintptr_t)ue->PlayerController);
				ImGui::Text(playerControllerDebugStr.c_str());

				std::string playerCameraManagerDebugStr = "PlayerCameraManager: " + utilFunction::AddressForImGui((uintptr_t)ue->PlayerCameraManager);
				ImGui::Text(playerCameraManagerDebugStr.c_str());

				std::string myPawnDebugStr = "myPawn: " + utilFunction::AddressForImGui((uintptr_t)ue->myPawn);
				ImGui::Text(myPawnDebugStr.c_str());

				std::string myCharacterDebugStr = "myCharacter: " + utilFunction::AddressForImGui((uintptr_t)ue->myCharacter);
				ImGui::Text(myCharacterDebugStr.c_str());

				ImGui::Text("--------------------------------");

				SDK::FRotator cameraAngleDebug = ue->PlayerCameraManager->GetCameraRotation();
				std::string cameraAngleDebugStr = "Camera angles: " + std::to_string(cameraAngleDebug.Pitch) + " " + std::to_string(cameraAngleDebug.Roll) + " " + std::to_string(cameraAngleDebug.Yaw);
				ImGui::Text(cameraAngleDebugStr.c_str());

				SDK::FVector cameraLocationDebug = ue->PlayerCameraManager->GetCameraLocation();
				std::string cameraLocationDebugStr = "Camera Location: " + std::to_string(cameraLocationDebug.X) + " " + std::to_string(cameraLocationDebug.Y) + " " + std::to_string(cameraLocationDebug.Z);
				ImGui::Text(cameraLocationDebugStr.c_str());

				ImGui::Text("--------------------------------");

				ImGui::Checkbox("All actors", &CFG.debugAllActors);
				if (CFG.debugAllActors)
				{
					ImGui::SliderFloat("Distance debug actor", &CFG.debugDistanceMax, 0.f, 1000.f);
					ImGui::Checkbox("Only Characters", &CFG.debugAllActorsOnlyCharacter);
					if (CFG.debugAllActorsOnlyCharacter)
					{
						ImGui::Checkbox("Only Characters Bones", &CFG.debugAllActorsOnlyCharacterBones);
					}
				}
			}

			ImGui::End();
		}
	}
}