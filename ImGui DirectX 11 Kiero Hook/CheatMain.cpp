#include "includes.h"

#pragma once

namespace CheatMain
{
	void Cheat()
	{
		// Test for few things
		if (CFG.debugAllActors && offset::G_WORLD && ue->World)
		{
			for (int i = 0; i < CFG.v_allActorsDebug.size(); i++)
			{
				SDK::AActor* actor = reinterpret_cast<SDK::AActor*>(CFG.v_allActorsDebug[i]);
				if (!actor) continue;
				actor->IsA(SDK::AActor::StaticClass());

				SDK::FVector postionActor = actor->K2_GetActorLocation();
				SDK::FVector2D relativePosToScreen = ue->w2s(postionActor);
				float distanceActor = ue->Distance(postionActor);

				if (CFG.debugDistanceMax > distanceActor)
				{
					if (!CFG.debugAllActorsOnlyCharacterBones)
					{
						overlay::DrawString({ (float)relativePosToScreen.X, (float)relativePosToScreen.Y }, actor->GetName(), ImColor{ 255,255,255 });
					}

					if (CFG.debugAllActorsOnlyCharacter && CFG.debugAllActorsOnlyCharacterBones)
					{
						SDK::ACharacter* characterActor = reinterpret_cast<SDK::ACharacter*>(actor);
						std::string nameActorStr = "Actor name: " + actor->GetName();
						ImGui::Text(nameActorStr.c_str());
						for (int j = 0; j < characterActor->mesh->GetNumBones(); j++)
						{
							SDK::FName boneSocketName = characterActor->mesh->GetBoneName(j);
							SDK::FVector boneSocketLocation = characterActor->mesh->GetSocketLocation(boneSocketName);
							SDK::FVector2D boneSocketLocation2D = ue->w2s(boneSocketLocation);

							overlay::DrawString({ (float)boneSocketLocation2D.X, (float)boneSocketLocation2D.Y }, boneSocketName.ToString(), ImColor{ 255, 255, 255 });
							ImGui::Text("%s (%d)", boneSocketName.ToString().c_str(), j);
						}
						ImGui::Text("--------------------------------");
					}
				}
			}
		}

		if (ue->World && ue->myCharacter && ue->level && ue->PlayerController)
		{
			/*for (size_t i = 0; i < CFG.v_allRoomIdentifiers.size(); i++)
			{
				SDK::ABP_RoomIdentifier_C* roomIdentifier = reinterpret_cast<SDK::ABP_RoomIdentifier_C*>(CFG.v_allRoomIdentifiers[i]);
				if (!roomIdentifier) continue;

				SDK::FVector2D roomIdentifierPos2D = ue->w2s(roomIdentifier->K2_GetActorLocation());
				overlay::DrawString({ (float)roomIdentifierPos2D.X, (float)roomIdentifierPos2D.Y }, roomIdentifier->GetName(), ImColor{ 0, 255, 255 });
			}*/

			float marginPlayerX = 50.f;
			float indentPlayerText = 100.f;
			for (size_t i = 0; i < CFG.v_allPlayers.size(); i++)
			{
				SDK::ABP_PlayerCharacter_C* player = reinterpret_cast<SDK::ABP_PlayerCharacter_C*>(CFG.v_allPlayers[i]);
				if (!player) continue;
				if (!ue->KismetSystemLib->IsValid(player)) continue;

				//-- troll
				if (player->PlayerState && ue->KismetSystemLib->IsValid(player->PlayerState))
				{
					SDK::ABP_Shivers_PlayerStateBase_C* playerStateBase = reinterpret_cast<SDK::ABP_Shivers_PlayerStateBase_C*>(player->PlayerState);
					std::string playerName = playerStateBase->PlayerNamePrivate.ToString();

					//-- force flashlight
					if (auto ForceFlashlight = CFG.map_forceFlashlightByName.find(playerName); ForceFlashlight != CFG.map_forceFlashlightByName.end()) {
						bool bForceFlashlight = ForceFlashlight->second;
						if (bForceFlashlight)
						{
							player->IsFlashlightActivated = true;
						}
					}
					else {
						CFG.map_forceFlashlightByName[playerName] = false;
					}
					//-- end force flashlight
				}
				//-- end troll

				if (CFG.b_playerInformation)
				{
					if (player->PlayerState && ue->KismetSystemLib->IsValid(player->PlayerState))
					{
						SDK::ABP_Shivers_PlayerStateBase_C* playerStateBase = reinterpret_cast<SDK::ABP_Shivers_PlayerStateBase_C*>(player->PlayerState);
						std::string playerName = playerStateBase->PlayerNamePrivate.ToString();

						overlay::DrawString({ marginPlayerX, indentPlayerText }, playerName, ImColor{ 255, 255, 255 });
						indentPlayerText += 13.f;
					}

					std::string sanityStr = "Sanity: " + utilFunction::to_str2(player->PlayerSanity);
					overlay::DrawString({ marginPlayerX, indentPlayerText }, sanityStr, ImColor{ 255, 255, 255 });
					indentPlayerText += 13.f;

					std::string isHiddingStr = player->bIsHiding ? "Hidden" : "Not hidden";
					overlay::DrawString({ marginPlayerX, indentPlayerText }, isHiddingStr, ImColor{ 255, 255, 255 });
					indentPlayerText += 13.f;

					indentPlayerText += 60.f;
				}

				if (player == ue->myCharacter) continue;

				SDK::FVector playerLocation = player->K2_GetActorLocation();
				SDK::FVector2D playerPos2D = ue->w2s(playerLocation);

				SDK::FVector BottomCharacter = player->mesh->GetSocketLocation(player->mesh->GetBoneName(0));
				SDK::FVector TopCharacter = SDK::FVector();

				TopCharacter = player->mesh->GetSocketLocation(ue->getBoneByName(player, bones::head));
				TopCharacter.Z = TopCharacter.Z + 25.0;

				//---------------------- pawn to screen
				//Bone Calculations
				const SDK::FVector BoneLoc1 = TopCharacter;
				const SDK::FVector BoneLoc2 = player->mesh->GetSocketLocation(ue->getBoneByName(player, bones::leftFeet));
				const SDK::FVector BoneLoc3 = BottomCharacter;

				SDK::FVector WorldHead = BoneLoc1;
				SDK::FVector WorldBottom = BoneLoc2;
				SDK::FVector WorldRoot = BoneLoc3;
				WorldRoot.Z = WorldRoot.Z - 10.0;

				SDK::FVector2D head = ue->w2s(WorldHead);
				SDK::FVector2D Top = ue->w2s({ BoneLoc1.X, BoneLoc1.Y, BoneLoc1.Z + 20 });
				SDK::FVector2D Bottom = ue->w2s(WorldBottom);
				SDK::FVector2D Root = ue->w2s(WorldRoot);

				//Calculations for ESP Positioning

				float BoxHeight = (float)(Root.Y - head.Y);
				float BoxWidth = BoxHeight * 0.60f;

				float BottomLeftX = (float)head.X - BoxWidth / 2;
				float BottomLeftY = (float)Root.Y;

				float BottomRightX = (float)head.X + BoxWidth / 2;
				float BottomRightY = (float)Root.Y;

				float TopRightX = (float)head.X + BoxWidth / 2;
				float TopRightY = (float)head.Y;

				float TopLeftX = (float)head.X - BoxWidth / 2;
				float TopLeftY = (float)head.Y;
				//-------------------------

				if (CFG.b_playerRect)
				{
					if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
					{
						overlay::RectNotFilled({ BottomLeftX, BottomLeftY }, { BottomRightX, BottomRightY }, { TopRightX, TopRightY }, { TopLeftX, TopLeftY }, CFG.boxVisiblePlayerColor, 1.f);
					}
				}

				if (CFG.b_playerName)
				{
					if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
					{
						if (player->PlayerState && ue->KismetSystemLib->IsValid(player->PlayerState))
						{
							SDK::ABP_Shivers_PlayerStateBase_C* playerStateBase = reinterpret_cast<SDK::ABP_Shivers_PlayerStateBase_C*>(player->PlayerState);
							std::string playerName = playerStateBase->PlayerNamePrivate.ToString();
							overlay::DrawString({ TopLeftX, TopLeftY - 15.f }, playerName, ImColor{ 255, 255, 255 });
						}
					}
				}

				if (CFG.b_player3dBox && ue->emptyToCompare2D != playerPos2D && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, playerPos2D))
				{
					overlay::Draw3dBox(player, CFG.boxVisiblePlayerColor, true);
				}

				if (CFG.b_playerSkeleton)
				{
					if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
					{
						std::vector<BonePair> bpair;

						bpair.push_back({ bones::head, bones::upBody });

						bpair.push_back({ bones::upBody, bones::leftShoulder });
						bpair.push_back({ bones::leftShoulder, bones::leftElbow });
						bpair.push_back({ bones::leftElbow, bones::leftHand });

						bpair.push_back({ bones::upBody, bones::rightShoulder });
						bpair.push_back({ bones::rightShoulder, bones::rightElbow });
						bpair.push_back({ bones::rightElbow, bones::rightHand });

						bpair.push_back({ bones::upBody, bones::pelvis });

						bpair.push_back({ bones::pelvis, bones::leftHip });
						bpair.push_back({ bones::leftHip, bones::leftKnee });
						bpair.push_back({ bones::leftKnee, bones::leftFeet });

						bpair.push_back({ bones::pelvis, bones::rightHip });
						bpair.push_back({ bones::rightHip, bones::rightKnee });
						bpair.push_back({ bones::rightKnee, bones::rightFeet });

						for (int y = 0; y < bpair.size(); y++)
						{
							SDK::FVector2D b1 = ue->w2s(player->mesh->GetSocketLocation(ue->getBoneByName(player, bpair[y].bone1)));
							SDK::FVector2D b2 = ue->w2s(player->mesh->GetSocketLocation(ue->getBoneByName(player, bpair[y].bone2)));
							if (b1 == SDK::FVector2D()) continue;
							if (b2 == SDK::FVector2D()) continue;
							overlay::DrawLine({ (float)b1.X,(float)b1.Y }, { (float)b2.X,(float)b2.Y }, CFG.skeletonPlayerColor, 1);
						}
					}
				}
			}

			if (CFG.b_playerShowDead)
			{
				for (size_t i = 0; i < CFG.v_allSpectatorPlayers.size(); i++)
				{
					SDK::ABP_SpectatorCharacter_C* player = reinterpret_cast<SDK::ABP_SpectatorCharacter_C*>(CFG.v_allSpectatorPlayers[i]);
					if (!player) continue;
					if (!ue->KismetSystemLib->IsValid(player)) continue;
					if (player == ue->myCharacterSpectator) continue;

					SDK::FVector playerLocation = player->K2_GetActorLocation();
					SDK::FVector2D playerPos2D = ue->w2s(playerLocation);

					SDK::FVector BottomCharacter = player->mesh->GetSocketLocation(player->mesh->GetBoneName(0));
					SDK::FVector TopCharacter = SDK::FVector();

					TopCharacter = player->mesh->GetSocketLocation(ue->getBoneByName(player, bones::head));
					TopCharacter.Z = TopCharacter.Z + 25.0;

					//---------------------- pawn to screen
					//Bone Calculations
					const SDK::FVector BoneLoc1 = TopCharacter;
					const SDK::FVector BoneLoc2 = player->mesh->GetSocketLocation(ue->getBoneByName(player, bones::leftFeet));
					const SDK::FVector BoneLoc3 = BottomCharacter;

					SDK::FVector WorldHead = BoneLoc1;
					SDK::FVector WorldBottom = BoneLoc2;
					SDK::FVector WorldRoot = BoneLoc3;
					WorldRoot.Z = WorldRoot.Z - 10.0;

					SDK::FVector2D head = ue->w2s(WorldHead);
					SDK::FVector2D Top = ue->w2s({ BoneLoc1.X, BoneLoc1.Y, BoneLoc1.Z + 20 });
					SDK::FVector2D Bottom = ue->w2s(WorldBottom);
					SDK::FVector2D Root = ue->w2s(WorldRoot);

					//Calculations for ESP Positioning

					float BoxHeight = (float)(Root.Y - head.Y);
					float BoxWidth = BoxHeight * 0.60f;

					float BottomLeftX = (float)head.X - BoxWidth / 2;
					float BottomLeftY = (float)Root.Y;

					float BottomRightX = (float)head.X + BoxWidth / 2;
					float BottomRightY = (float)Root.Y;

					float TopRightX = (float)head.X + BoxWidth / 2;
					float TopRightY = (float)head.Y;

					float TopLeftX = (float)head.X - BoxWidth / 2;
					float TopLeftY = (float)head.Y;
					//-------------------------

					if (CFG.b_playerRect)
					{
						if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
						{
							overlay::RectNotFilled({ BottomLeftX, BottomLeftY }, { BottomRightX, BottomRightY }, { TopRightX, TopRightY }, { TopLeftX, TopLeftY }, CFG.boxVisiblePlayerColor, 1.f);
						}
					}

					if (CFG.b_playerName)
					{
						if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
						{
							if (player->PlayerState && ue->KismetSystemLib->IsValid(player->PlayerState))
							{
								SDK::ABP_Shivers_PlayerStateBase_C* playerStateBase = reinterpret_cast<SDK::ABP_Shivers_PlayerStateBase_C*>(player->PlayerState);
								std::string playerName = playerStateBase->PlayerNamePrivate.ToString();
								overlay::DrawString({ TopLeftX, TopLeftY - 15.f }, playerName, ImColor{ 255, 255, 255 });
							}
						}
					}

					if (CFG.b_player3dBox && ue->emptyToCompare2D != playerPos2D && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, playerPos2D))
					{
						overlay::Draw3dBox(player, CFG.boxVisiblePlayerColor, true);
					}

					if (CFG.b_playerSkeleton)
					{
						if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
						{
							std::vector<BonePair> bpair;

							bpair.push_back({ bones::head, bones::upBody });

							bpair.push_back({ bones::upBody, bones::leftShoulder });
							bpair.push_back({ bones::leftShoulder, bones::leftElbow });
							bpair.push_back({ bones::leftElbow, bones::leftHand });

							bpair.push_back({ bones::upBody, bones::rightShoulder });
							bpair.push_back({ bones::rightShoulder, bones::rightElbow });
							bpair.push_back({ bones::rightElbow, bones::rightHand });

							bpair.push_back({ bones::upBody, bones::pelvis });

							bpair.push_back({ bones::pelvis, bones::leftHip });
							bpair.push_back({ bones::leftHip, bones::leftKnee });
							bpair.push_back({ bones::leftKnee, bones::leftFeet });

							bpair.push_back({ bones::pelvis, bones::rightHip });
							bpair.push_back({ bones::rightHip, bones::rightKnee });
							bpair.push_back({ bones::rightKnee, bones::rightFeet });

							for (int y = 0; y < bpair.size(); y++)
							{
								SDK::FVector2D b1 = ue->w2s(player->mesh->GetSocketLocation(ue->getBoneByName(player, bpair[y].bone1)));
								SDK::FVector2D b2 = ue->w2s(player->mesh->GetSocketLocation(ue->getBoneByName(player, bpair[y].bone2)));
								if (b1 == SDK::FVector2D()) continue;
								if (b2 == SDK::FVector2D()) continue;
								overlay::DrawLine({ (float)b1.X,(float)b1.Y }, { (float)b2.X,(float)b2.Y }, CFG.skeletonPlayerColor, 1);
							}
						}
					}
				}
			}

			for (size_t i = 0; i < CFG.v_allTools.size(); i++)
			{
				SDK::ABP_Tool_C* tool = reinterpret_cast<SDK::ABP_Tool_C*>(CFG.v_allTools[i]);
				if (!tool) continue;
				if (!ue->KismetSystemLib->IsValid(tool)) continue;

				// --- name tool on var: secondPart
				std::string typeOfToolStr = tool->ItemTag.TagName.ToString();
				std::string secondPart;

				const std::string& s = typeOfToolStr;
				std::size_t firstDot = s.find('.');
				if (firstDot != std::string::npos) {
					std::size_t start = firstDot + 1;
					std::size_t nextDot = s.find('.', start);
					if (nextDot == std::string::npos) nextDot = s.size();

					if (start < nextDot) {
						secondPart = s.substr(start, nextDot - start);
					}
				}
				if (secondPart == "")
				{
					secondPart = tool->GetName();
				}
				// ---

				if (CFG.b_objectToolsPickup && CFG.b_MenuShow)
				{
					ImGui::Begin("Pickup tool");

					if (tool->CanPlayerInteract(ue->myCharacter))
					{
						std::string itemBtnStr = secondPart + "##tool_grab_" + std::to_string(i);
						if (ImGui::Button(itemBtnStr.c_str()))
						{
							tool->PickTool(ue->myCharacter);
						}
					}
					else
					{
						ImGui::Text("You carry too many tool");
					}

					ImGui::End();
				}

				if (CFG.b_objectTools)
				{
					SDK::FVector2D toolPos2D = ue->w2s(tool->K2_GetActorLocation());
					if (ue->emptyToCompare2D != toolPos2D && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, toolPos2D))
					{
						overlay::DrawFilledCircle({ (float)toolPos2D.X, (float)toolPos2D.Y }, 2.f, CFG.toolColor);
						overlay::DrawString({ (float)toolPos2D.X + 10, (float)toolPos2D.Y }, secondPart, CFG.toolColor);
					}
				}

				/*if (tool->IsA(ue->staticToolFlashlight))
				{
					SDK::ABP_Flashlight_C* flashlight = reinterpret_cast<SDK::ABP_Flashlight_C*>(tool);
					if (!flashlight) continue;

					std::string flashlightBtnStr = "Flashlight intensity##flashlight_settings_" + std::to_string(i);
					float testFloat = (float)flashlight->TargetIntensity;
					ImGui::SliderFloat(flashlightBtnStr.c_str(), &testFloat, 0.f, 10000.f);
					flashlight->TargetIntensity = (double)testFloat;

					std::string flashlightBtn2Str = "Flashlight multiplier##flashlight_multiplier_" + std::to_string(i);
					testFloat = (float)flashlight->Multiplier;
					ImGui::SliderFloat(flashlightBtn2Str.c_str(), &testFloat, 0.f, 10000.f);
					flashlight->Multiplier = (double)testFloat;

					std::string flashlightBtn3Str = "Flashlight CustomIntensityMultiplier##flashlight_custom_intensity_multiplier_" + std::to_string(i);
					testFloat = (float)flashlight->CustomIntensityMultiplier;
					ImGui::SliderFloat(flashlightBtn3Str.c_str(), &testFloat, 0.f, 10000.f);
					flashlight->CustomIntensityMultiplier = (double)testFloat;
				}*/
			}

			for (size_t i = 0; i < CFG.v_allHidingSpotVisuals.size(); i++)
			{
				SDK::ABP_HidingSpotVisual_C* hidingSpotVisual = reinterpret_cast<SDK::ABP_HidingSpotVisual_C*>(CFG.v_allHidingSpotVisuals[i]);
				if (!hidingSpotVisual) continue;
				if (!ue->KismetSystemLib->IsValid(hidingSpotVisual)) continue;

				if (CFG.b_objectHiddingSpotVisual)
				{
					SDK::FVector2D toolPos2D = ue->w2s(hidingSpotVisual->K2_GetActorLocation());
					if (ue->emptyToCompare2D != toolPos2D && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, toolPos2D))
					{
						overlay::DrawFilledCircle({ (float)toolPos2D.X, (float)toolPos2D.Y }, 2.f, CFG.hidingSpotVisualColor);
						overlay::DrawString({ (float)toolPos2D.X + 10, (float)toolPos2D.Y }, "Closet", CFG.hidingSpotVisualColor);
					}
				}

				if (CFG.b_objectHiddingSpotVisual3D)
				{
					overlay::Draw3dBox(hidingSpotVisual, CFG.hidingSpotVisualColor, false);
				}
			}

			for (size_t i = 0; i < CFG.v_allEntities.size(); i++)
			{
				SDK::ABP_Entity_C* entity = reinterpret_cast<SDK::ABP_Entity_C*>(CFG.v_allEntities[i]);
				if (!entity) continue;
				if (!ue->KismetSystemLib->IsValid(entity)) continue;

				if (CFG.b_ghostEvent)
				{
					entity->GhostEventBehaviour();

					CFG.b_ghostEvent = false;
				}

				if (CFG.b_ghostThrowItemEvent)
				{
					bool succesThrowItem = false;

					entity->ThrowItemBehavior(&succesThrowItem);

					CFG.b_ghostThrowItemEvent = false;
				}

				if (CFG.b_ghostTeleportToFavouriteRoom)
				{
					entity->TeleportAiInFavouriteRoom();

					CFG.b_ghostTeleportToFavouriteRoom = false;
				}

				if (CFG.b_ghostHuntEvent)
				{
					//entity->SetAiCurrentStatus(SDK::E_CurrentStatusAI::NewEnumerator2); // NewEnumerator2 hunting
					entity->HuntBehaviour();

					CFG.b_ghostHuntEvent = false;
				}

				if (CFG.b_ghostInformation)
				{
					int marginX = 250;
					float indentText = 100.f;

					std::string typeOfGhostStr = "Entity: " + entity->EntityIdentity.ToString();
					overlay::DrawString({ (float)CFG.ScreenWidth - marginX, indentText }, typeOfGhostStr, ImColor{ 255, 255, 255 });
					indentText += 13.f;

					std::string entityNameStr = "Entity name: " + entity->EntityName.ToString();
					overlay::DrawString({ (float)CFG.ScreenWidth - marginX, indentText }, entityNameStr, ImColor{ 255, 255, 255 });
					indentText += 13.f;

					std::string entityAgeStr = "Entity age: " + std::to_string(entity->GhostAge);
					overlay::DrawString({ (float)CFG.ScreenWidth - marginX, indentText }, entityAgeStr, ImColor{ 255, 255, 255 });
					indentText += 13.f;

					std::string isShyStr = entity->IsShy ? "Is shy" : "Is not shy";
					overlay::DrawString({ (float)CFG.ScreenWidth - marginX, indentText }, isShyStr, ImColor{ 255, 255, 255 });
					indentText += 13.f;

					std::string isHuntingStr = entity->IsHunting ? "Is hunting" : "Is not hunting";
					overlay::DrawString({ (float)CFG.ScreenWidth - marginX, indentText }, isHuntingStr, entity->IsHunting ? ImColor{ 255, 0, 0 } : ImColor{ 0, 255, 0 });
					indentText += 13.f;

					std::string huntChanceStr = "Hunt chance: " + std::to_string((int)entity->GetHuntChance()) + "%";
					overlay::DrawString({ (float)CFG.ScreenWidth - marginX, indentText }, huntChanceStr, ImColor{ 255, 255, 255 });
					indentText += 13.f;
				}

				SDK::FVector entityLocation = entity->K2_GetActorLocation();
				SDK::FVector2D entityPos2D = ue->w2s(entityLocation);

				if (ue->emptyToCompare2D != entityPos2D && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, entityPos2D))
				{
					if (CFG.b_ghostCircle)
					{
						overlay::DrawFilledCircle({ (float)entityPos2D.X, (float)entityPos2D.Y }, 2.f, CFG.boxVisibleGhostColor);
						overlay::DrawString({ (float)entityPos2D.X + 10, (float)entityPos2D.Y }, "Entity", CFG.boxVisibleGhostColor);
					}

					if (CFG.b_ghost3dBox)
					{
						overlay::Draw3dBox(entity, CFG.boxVisibleGhostColor, true);
					}
				}

				if (CFG.b_ghostFavouriteRoom && entity->FavouriteRoom)
				{
					SDK::ABP_FavouriteRoom_C* favouriteRoom = entity->FavouriteRoom;
					if (ue->KismetSystemLib->IsValid(favouriteRoom))
					{
						SDK::FVector favouriteRoomLocation = favouriteRoom->K2_GetActorLocation();
						SDK::FVector2D favouriteRoomPos2D = ue->w2s(favouriteRoomLocation);

						if (ue->emptyToCompare2D != favouriteRoomPos2D && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, favouriteRoomPos2D))
						{
							overlay::DrawFilledCircle({ (float)favouriteRoomPos2D.X, (float)favouriteRoomPos2D.Y }, 2.f, CFG.boxVisibleGhostColor);
							overlay::DrawString({ (float)favouriteRoomPos2D.X + 10, (float)favouriteRoomPos2D.Y }, "Favourite room", CFG.boxVisibleGhostColor);
						}
					}
				}

				if (CFG.b_ghostShowSelf)
				{
					entity->GhostShowSelf();
					CFG.b_ghostShowSelf = false;
				}
			}

			if (CFG.b_miscInfStamina)
			{
				if (ue->myCharacter->BP_SprintComponent && ue->KismetSystemLib->IsValid(ue->myCharacter->BP_SprintComponent))
				{
					if (ue->myCharacter->BP_SprintComponent->Stamina != ue->myCharacter->BP_SprintComponent->MaximumStamina)
						ue->myCharacter->BP_SprintComponent->Stamina = ue->myCharacter->BP_SprintComponent->MaximumStamina;
				}
			}

			if (CFG.b_miscNoClip)
			{
				ue->ToggleNoClip(CFG.b_miscNoClip);
				ue->NoclipTick();
			}

			ue->SetLightShadow(CFG.b_miscFullbright);
		}

		if (ue->gameInstance && ue->KismetSystemLib->IsValid(ue->gameInstance))
		{
			SDK::UBP_PlayerAssets_C* playerAssets = nullptr;

			for (SDK::TPair<SDK::E_SaveGames, SDK::USaveGame*>& kv : ue->gameInstance->RegisteredSaveGameFiles)
			{
				SDK::USaveGame* saveGame = kv.Value();
				if (!saveGame) continue;
				if (saveGame->IsA(ue->staticPlayerAssets)) playerAssets = reinterpret_cast<SDK::UBP_PlayerAssets_C*>(saveGame);
			}

			if (playerAssets && ue->KismetSystemLib->IsValid(playerAssets))
			{
				if (CFG.b_saveMoney)
				{
					playerAssets->Money = CFG.saveMoneyValue;
					CFG.b_saveMoney = false;
				}

				if (CFG.b_saveLevel)
				{
					playerAssets->PlayerSafeData.Level_3_BF7479E24F2224F0DD0F5880BD0BE237 = CFG.saveLevelValue;
					CFG.b_saveLevel = false;
				}
			}
		}

		if (ue->World && ue->level && ue->myCharacterLobby && ue->PlayerController)
		{
			for (size_t i = 0; i < CFG.v_allLobbyPlayers.size(); i++)
			{
				SDK::ABP_LobbyCharacter_C* player = reinterpret_cast<SDK::ABP_LobbyCharacter_C*>(CFG.v_allLobbyPlayers[i]);
				if (!player) continue;
				if (!ue->KismetSystemLib->IsValid(player)) continue;
				if (player == ue->myCharacterLobby) continue;

				SDK::FVector playerLocation = player->K2_GetActorLocation();
				SDK::FVector2D playerPos2D = ue->w2s(playerLocation);

				SDK::FVector BottomCharacter = player->mesh->GetSocketLocation(player->mesh->GetBoneName(0));
				SDK::FVector TopCharacter = SDK::FVector();

				TopCharacter = player->mesh->GetSocketLocation(ue->getBoneByName(player, bones::head));
				TopCharacter.Z = TopCharacter.Z + 25.0;

				//---------------------- pawn to screen
				//Bone Calculations
				const SDK::FVector BoneLoc1 = TopCharacter;
				const SDK::FVector BoneLoc2 = player->mesh->GetSocketLocation(ue->getBoneByName(player, bones::leftFeet));
				const SDK::FVector BoneLoc3 = BottomCharacter;

				SDK::FVector WorldHead = BoneLoc1;
				SDK::FVector WorldBottom = BoneLoc2;
				SDK::FVector WorldRoot = BoneLoc3;
				WorldRoot.Z = WorldRoot.Z - 10.0;

				SDK::FVector2D head = ue->w2s(WorldHead);
				SDK::FVector2D Top = ue->w2s({ BoneLoc1.X, BoneLoc1.Y, BoneLoc1.Z + 20 });
				SDK::FVector2D Bottom = ue->w2s(WorldBottom);
				SDK::FVector2D Root = ue->w2s(WorldRoot);

				//Calculations for ESP Positioning

				float BoxHeight = (float)(Root.Y - head.Y);
				float BoxWidth = BoxHeight * 0.60f;

				float BottomLeftX = (float)head.X - BoxWidth / 2;
				float BottomLeftY = (float)Root.Y;

				float BottomRightX = (float)head.X + BoxWidth / 2;
				float BottomRightY = (float)Root.Y;

				float TopRightX = (float)head.X + BoxWidth / 2;
				float TopRightY = (float)head.Y;

				float TopLeftX = (float)head.X - BoxWidth / 2;
				float TopLeftY = (float)head.Y;
				//-------------------------

				if (CFG.b_playerRect)
				{
					if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
					{
						overlay::RectNotFilled({ BottomLeftX, BottomLeftY }, { BottomRightX, BottomRightY }, { TopRightX, TopRightY }, { TopLeftX, TopLeftY }, CFG.boxVisiblePlayerColor, 1.f);
					}
				}

				if (CFG.b_playerName)
				{
					if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
					{
						if (player->PlayerState && ue->KismetSystemLib->IsValid(player->PlayerState))
						{
							SDK::ABP_Shivers_PlayerStateBase_C* playerStateBase = reinterpret_cast<SDK::ABP_Shivers_PlayerStateBase_C*>(player->PlayerState);
							std::string playerName = playerStateBase->PlayerNamePrivate.ToString();
							overlay::DrawString({ TopLeftX, TopLeftY - 15.f }, playerName, ImColor{ 255, 255, 255 });
						}
					}
				}

				if (CFG.b_player3dBox && ue->emptyToCompare2D != playerPos2D && !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, playerPos2D))
				{
					overlay::Draw3dBox(player, CFG.boxVisiblePlayerColor, true);
				}

				if (CFG.b_playerSkeleton)
				{
					if (head != SDK::FVector2D() && Root != SDK::FVector2D() && (!ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, head) || !ue->IsNotInScreen(CFG.ScreenWidth, CFG.ScreenHeight, Root)))
					{
						std::vector<BonePair> bpair;

						bpair.push_back({ bones::head, bones::upBody });

						bpair.push_back({ bones::upBody, bones::leftShoulder });
						bpair.push_back({ bones::leftShoulder, bones::leftElbow });
						bpair.push_back({ bones::leftElbow, bones::leftHand });

						bpair.push_back({ bones::upBody, bones::rightShoulder });
						bpair.push_back({ bones::rightShoulder, bones::rightElbow });
						bpair.push_back({ bones::rightElbow, bones::rightHand });

						bpair.push_back({ bones::upBody, bones::pelvis });

						bpair.push_back({ bones::pelvis, bones::leftHip });
						bpair.push_back({ bones::leftHip, bones::leftKnee });
						bpair.push_back({ bones::leftKnee, bones::leftFeet });

						bpair.push_back({ bones::pelvis, bones::rightHip });
						bpair.push_back({ bones::rightHip, bones::rightKnee });
						bpair.push_back({ bones::rightKnee, bones::rightFeet });

						for (int y = 0; y < bpair.size(); y++)
						{
							SDK::FVector2D b1 = ue->w2s(player->mesh->GetSocketLocation(ue->getBoneByName(player, bpair[y].bone1)));
							SDK::FVector2D b2 = ue->w2s(player->mesh->GetSocketLocation(ue->getBoneByName(player, bpair[y].bone2)));
							if (b1 == SDK::FVector2D()) continue;
							if (b2 == SDK::FVector2D()) continue;
							overlay::DrawLine({ (float)b1.X,(float)b1.Y }, { (float)b2.X,(float)b2.Y }, CFG.skeletonPlayerColor, 1);
						}
					}
				}
			}

			if (CFG.b_miscNoClip)
			{
				ue->ToggleNoClip(CFG.b_miscNoClip);
				ue->NoclipTick();
			}

			if (CFG.b_miscInfStamina)
			{
				if (ue->myCharacterLobby->BP_SprintComponent && ue->KismetSystemLib->IsValid(ue->myCharacterLobby->BP_SprintComponent))
				{
					if (ue->myCharacterLobby->BP_SprintComponent->Stamina != ue->myCharacterLobby->BP_SprintComponent->MaximumStamina)
						ue->myCharacterLobby->BP_SprintComponent->Stamina = ue->myCharacterLobby->BP_SprintComponent->MaximumStamina;
				}
			}
		}
	}
}