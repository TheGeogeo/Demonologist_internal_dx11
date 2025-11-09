#pragma once

#define M_PI 3.14159265358979323846

struct BonePair {
	std::string bone1;
	std::string bone2;
};

struct vec3
{
	float x, y, z;
};

class bones
{
public:
	static inline const std::string bottom = "Root";
	static inline const std::string pelvis = "pelvis";
	static inline const std::string middleBody = "spine_02";
	static inline const std::string upBody = "spine_03";
	static inline const std::string leftElbow = "lowerarm_l";
	static inline const std::string leftHand = "hand_l";
	static inline const std::string leftShoulder = "upperarm_l";
	static inline const std::string rightShoulder = "upperarm_r";
	static inline const std::string rightElbow = "lowerarm_r";
	static inline const std::string rightHand = "hand_r";
	static inline const std::string neck = "neck_01";
	static inline const std::string head = "Head";
	static inline const std::string leftHip = "thigh_l";
	static inline const std::string leftKnee = "calf_l";
	static inline const std::string leftFeet = "foot_l";
	static inline const std::string rightHip = "thigh_r";
	static inline const std::string rightKnee = "calf_r";
	static inline const std::string rightFeet = "foot_r";
};

namespace offset
{
	const inline uint64_t G_WORLD = SDK::Offsets::GWorld;
	const inline uint64_t G_NAME = SDK::Offsets::GNames;
	const inline uint64_t G_OBJECT_ARRAY = SDK::Offsets::GObjects;
}

class UnrealEngine
{
public:
	static inline SDK::UEngine* Engine = nullptr;
	static inline SDK::UWorld** GWorld = nullptr;
	static inline SDK::UWorld* World = nullptr;
	static inline SDK::ULevel* level = nullptr;
	static inline SDK::AGameStateBase* GameState = nullptr;
	static inline SDK::AGameModeBase* authorityGamemode = nullptr;
	static inline SDK::UBP_ShiversGameInstance_C* gameInstance = nullptr;
	static inline SDK::ULocalPlayer* LocalPlayer = nullptr;
	static inline SDK::ABP_ShiversController_C* PlayerController = nullptr;
	static inline SDK::APlayerCameraManager* PlayerCameraManager = nullptr;
	static inline SDK::ABP_PlayerCharacter_C* myCharacter = nullptr;
	static inline SDK::ABP_LobbyCharacter_C* myCharacterLobby = nullptr;
	static inline SDK::ABP_SpectatorCharacter_C* myCharacterSpectator = nullptr;
	static inline SDK::ABP_Shivers_InGamePlayerState_C* playerStateInGame = nullptr;
	static inline SDK::ABP_Shivers_LobbyPlayerState_C* playerStateLobby = nullptr;
	static inline SDK::APawn* myPawn = nullptr;
	static inline SDK::FVector myLoc;
	static inline SDK::FRotator myRot;
	//-------
	static inline SDK::UCanvas* Canvas = nullptr;
	static inline SDK::FMinimalViewInfo* Camera = nullptr;
	static inline SDK::UKismetMathLibrary* MathLib = nullptr;
	static inline SDK::UKismetSystemLibrary* KismetSystemLib = nullptr;
	static inline SDK::UGameplayStatics* GPStatics = nullptr;
	static inline SDK::UFont* Font = nullptr;
	//-------
	static inline SDK::FVector2D emptyToCompare2D = SDK::FVector2D();
	static inline SDK::FVector kUp;
	//-------
	static inline SDK::UClass* staticPlayerAssets = SDK::UBP_PlayerAssets_C::StaticClass();
	static inline SDK::UClass* staticLobbyPlayerState = SDK::ABP_Shivers_LobbyPlayerState_C::StaticClass();
	static inline SDK::UClass* staticInGamePlayerAssets = SDK::ABP_Shivers_InGamePlayerState_C::StaticClass();
	static inline SDK::UClass* staticLobbyCharacter = SDK::ABP_LobbyCharacter_C::StaticClass();
	static inline SDK::UClass* staticInGameCharacter = SDK::ABP_PlayerCharacter_C::StaticClass();
	static inline SDK::UClass* staticSpectatorCharacter = SDK::ABP_SpectatorCharacter_C::StaticClass();
	static inline SDK::UClass* staticToolFlashlight = SDK::ABP_Flashlight_C::StaticClass();

	void ReDefineAttribut()
	{
		//set MathLib for math functions
		if (!this->MathLib) this->MathLib = (SDK::UKismetMathLibrary*)SDK::UKismetMathLibrary::GetDefaultObj();

		//set SystemLib for math functions
		if (!this->KismetSystemLib) this->KismetSystemLib = (SDK::UKismetSystemLibrary*)SDK::UKismetSystemLibrary::GetDefaultObj();

		//Set GPStatics for game functions
		if (!this->GPStatics) this->GPStatics = (SDK::UGameplayStatics*)SDK::UGameplayStatics::GetDefaultObj();

		this->Engine = SDK::UEngine::GetEngine();

		this->GWorld = (SDK::UWorld**)((DWORD64)GetModuleHandleW(NULL) + offset::G_WORLD);
		if (this->GWorld && this->KismetSystemLib->IsValid(*this->GWorld)) this->World = *this->GWorld;
		else this->World = nullptr;

		if (this->World && this->KismetSystemLib->IsValid(this->World->PersistentLevel)) this->level = this->World->PersistentLevel;
		else this->level = nullptr;

		if (this->World && this->KismetSystemLib->IsValid(this->World->AuthorityGameMode)) this->authorityGamemode = this->World->AuthorityGameMode;
		else this->authorityGamemode = nullptr;

		if (this->World && this->KismetSystemLib->IsValid(this->World->GameState)) this->GameState = this->World->GameState;
		else this->GameState = nullptr;

		if (this->World && this->KismetSystemLib->IsValid(this->World->OwningGameInstance)) this->gameInstance = reinterpret_cast<SDK::UBP_ShiversGameInstance_C*>(this->World->OwningGameInstance);
		else this->gameInstance = nullptr;

		if (this->gameInstance && this->gameInstance->LocalPlayers.Num() > 0 && this->KismetSystemLib->IsValid(this->gameInstance->LocalPlayers[0])) this->LocalPlayer = this->gameInstance->LocalPlayers[0];
		else this->LocalPlayer = nullptr;

		if (this->LocalPlayer && this->KismetSystemLib->IsValid(this->LocalPlayer->PlayerController)) this->PlayerController = reinterpret_cast<SDK::ABP_ShiversController_C*>(this->LocalPlayer->PlayerController);
		else this->PlayerController = nullptr;

		if (this->PlayerController && this->KismetSystemLib->IsValid(this->PlayerController->PlayerCameraManager)) this->PlayerCameraManager = this->PlayerController->PlayerCameraManager;
		else this->PlayerCameraManager = nullptr;

		if (this->PlayerController && this->KismetSystemLib->IsValid(this->PlayerController->K2_GetPawn())) this->myPawn = this->PlayerController->K2_GetPawn();
		else this->myPawn = nullptr;

		if (this->PlayerController && this->KismetSystemLib->IsValid(this->PlayerController->Character))
		{
			if (this->PlayerController->Character->IsA(this->staticInGameCharacter))
			{
				this->myCharacter = reinterpret_cast<SDK::ABP_PlayerCharacter_C*>(this->PlayerController->Character);
				this->myCharacterLobby = nullptr;
				this->myCharacterSpectator = nullptr;
			}
			else if (this->PlayerController->Character->IsA(this->staticLobbyCharacter))
			{
				this->myCharacterLobby = reinterpret_cast<SDK::ABP_LobbyCharacter_C*>(this->PlayerController->Character);
				this->myCharacter = nullptr;
				this->myCharacterSpectator = nullptr;
			}
			else if (this->PlayerController->Character->IsA(this->staticSpectatorCharacter))
			{
				this->myCharacterSpectator = reinterpret_cast<SDK::ABP_SpectatorCharacter_C*>(this->PlayerController->Character);
				this->myCharacterLobby = nullptr;
				this->myCharacter = nullptr;
			}
			else
			{
				this->myCharacter = nullptr;
				this->myCharacterLobby = nullptr;
				this->myCharacterSpectator = nullptr;
			}
		}
		else
		{
			this->myCharacter = nullptr;
			this->myCharacterLobby = nullptr;
			this->myCharacterSpectator = nullptr;
		}

		if (this->myCharacter && this->KismetSystemLib->IsValid(this->myCharacter->PlayerState))
		{
			if (this->myCharacter->PlayerState->IsA(this->staticLobbyPlayerState))
			{
				this->playerStateLobby = reinterpret_cast<SDK::ABP_Shivers_LobbyPlayerState_C*>(this->myCharacter->PlayerState);
				this->playerStateInGame = nullptr;
			}
			else if (this->myCharacter->PlayerState->IsA(this->staticInGamePlayerAssets))
			{
				this->playerStateInGame = reinterpret_cast<SDK::ABP_Shivers_InGamePlayerState_C*>(this->myCharacter->PlayerState);
				this->playerStateLobby = nullptr;
			}
			else
			{
				this->playerStateInGame = nullptr;
				this->playerStateLobby = nullptr;
			}
		}
		else
		{
			this->playerStateInGame = nullptr;
			this->playerStateLobby = nullptr;
		}

		if (this->PlayerCameraManager) this->myLoc = this->PlayerCameraManager->GetCameraLocation();
		else this->myLoc = SDK::FVector();

		if (this->PlayerCameraManager) this->myRot = this->PlayerCameraManager->GetCameraRotation();
		else this->myRot = SDK::FRotator();
	}

	bool bFov = false;
	float Fov = 0.f;
	float Smoothing = 1.f;
	SDK::FVector2D w2s(SDK::FVector location)
	{
		SDK::FVector2D screen;

		if (this->PlayerController)
		{
			this->PlayerController->ProjectWorldLocationToScreen(location, &screen, false);
			return screen;
		}

		return { 0,0 };
	}

	inline void SetAim(SDK::APlayerCameraManager* MyCamera, SDK::ACharacter* Target, int bone)
	{
		if (this->PlayerController)
		{
			SDK::FVector CamLocation = MyCamera->GetCameraLocation();
			SDK::FRotator CamRotation = MyCamera->GetCameraRotation();
			SDK::FVector AimLocation;
			SDK::FRotator AimRotation, CurrentRotation;

			AimLocation = Target->mesh->GetSocketLocation(Target->mesh->GetBoneName(bone));

			//get aim rotation
			AimRotation = MathLib->FindLookAtRotation(CamLocation, AimLocation);

			//set aim
			this->PlayerController->SetControlRotation(AimRotation);
		}
	}

	float deg_2_rad(float degrees) {
		float radians;
		radians = degrees * (M_PI / 180);
		return radians;
	}

	void angle_vectors(const SDK::FVector& angles, SDK::FVector* forward) {
		float sp, sy, cp, cy;
		sy = sin(deg_2_rad(angles.Y));
		cy = cos(deg_2_rad(angles.Y));
		sp = sin(deg_2_rad(angles.X));
		cp = cos(deg_2_rad(angles.X));
		forward->X = cp * cy;
		forward->Y = cp * sy;
		forward->Z = -sp;
	}

	void __forceinline AnsiToWide(char* inAnsi, wchar_t* outWide)
	{
		int i = 0;
		for (; inAnsi[i] != '\0'; i++)
			outWide[i] = (wchar_t)(inAnsi)[i];
		outWide[i] = L'\0';
	}

	void SetRotation(SDK::FRotator TargetRotation, bool bWithRotationInput, float Smooth)
	{
		if (this->PlayerController && this->PlayerCameraManager) {
			SDK::FRotator v11 = this->PlayerController->GetControlRotation();
			SDK::FRotator v10 = this->PlayerCameraManager->GetCameraRotation();

			if (!bWithRotationInput)
				v11 = v10;

			SDK::FRotator currentRotation = v11;

			SDK::FRotator smoothRotation;
			smoothRotation.Pitch = currentRotation.Pitch + (TargetRotation.Pitch - currentRotation.Pitch) / Smooth;
			smoothRotation.Yaw = currentRotation.Yaw + (TargetRotation.Yaw - currentRotation.Yaw) / Smooth;
			smoothRotation.Roll = currentRotation.Roll + (TargetRotation.Roll - currentRotation.Roll) / Smooth;

			PlayerController->SetControlRotation(smoothRotation);
		}
	}

	SDK::FVector AngleToFVector(SDK::FVector location, SDK::FRotator rotator, float distance) {
		//float pitch = -deg_2_rad(rotator.Pitch); // depent camera
		float pitch = deg_2_rad(rotator.Pitch);
		float yaw = deg_2_rad(rotator.Yaw);
		float x = cos(yaw) * cos(pitch);
		float y = sin(yaw) * cos(pitch);
		float z = sin(pitch);

		return location + SDK::FVector(x, y, z) * distance;
	}

	SDK::FRotator ToRotator(SDK::FVector local, SDK::FVector target) {
		SDK::FVector rotation = local - target;

		SDK::FRotator newViewAngle = { 0 };

		float hyp = sqrt(rotation.X * rotation.X + rotation.Y * rotation.Y);

		newViewAngle.Pitch = -atan(rotation.Z / hyp) * (180.f / M_PI);
		newViewAngle.Yaw = atan(rotation.Y / rotation.X) * (180.f / M_PI);
		newViewAngle.Roll = (float)0.f;

		if (rotation.X >= 0.f)
			newViewAngle.Yaw += 180.0f;

		return newViewAngle;
	}

	SDK::FVector GetPlayerVelocity(SDK::ACharacter* p)
	{
		if (p->RootComponent)
			return p->RootComponent->ComponentVelocity;

		return { 0.f, 0.f, 0.f };
	}

	SDK::FVector OriginalLocation = { 0, 0, 0 };
	SDK::FRotator OriginalRotation = { 0, 0, 0 };

	void Aimbot(SDK::ACharacter* Target, SDK::FName& aimingAt, float Smooth)
	{
		if (Target && this->PlayerController && this->PlayerCameraManager)
		{
			SDK::FVector AimDirection = Target->mesh->GetSocketLocation(aimingAt);
			SDK::FVector CameraCachePrivate_POV_Location = this->PlayerController->PlayerCameraManager->CameraCachePrivate.POV.Location;
			SDK::FRotator AimRotation = ToRotator(CameraCachePrivate_POV_Location, AimDirection);
			SetRotation(AimRotation, false, Smooth);
		}
	}

	static inline SDK::FVector RotateCorner(SDK::FVector origin, SDK::FVector corner, float theta)
	{
		float x = corner.X - origin.X;
		float y = corner.Y - origin.Y;

		return {
			origin.X + (x * MathLib->Cos(theta) - y * MathLib->Sin(theta)),
			origin.Y + (x * MathLib->Sin(theta) + y * MathLib->Cos(theta)),
			corner.Z
		};
	}

	static BOOL IsNotInScreen(int SizeX, int SizeY, SDK::FVector2D Pos, int maxOutOfTheBox = -200) {
		if (
			((Pos.X <= maxOutOfTheBox or Pos.X > SizeX + abs(maxOutOfTheBox)) and (Pos.Y <= maxOutOfTheBox or Pos.Y > SizeY + abs(maxOutOfTheBox)))
			or
			((Pos.X <= maxOutOfTheBox or Pos.X > SizeX + abs(maxOutOfTheBox)) or (Pos.Y <= maxOutOfTheBox or Pos.Y > SizeY + abs(maxOutOfTheBox))))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	float Distance(SDK::FVector target)
	{
		float x = this->myLoc.X - target.X;
		float y = this->myLoc.Y - target.Y;
		float z = this->myLoc.Z - target.Z;

		return sqrtf((x * x) + (y * y) + (z * z)) * 0.03048f;
	}

	SDK::FName getBoneByName(SDK::ACharacter* pawn, std::string nameBone)
	{
		SDK::FName baseReturn;
		for (int y = 0; y < pawn->mesh->GetNumBones(); y++)
		{
			if (y == 0) baseReturn = pawn->mesh->GetBoneName(y);
			if (pawn->mesh->GetBoneName(y).ToString() != nameBone) continue;
			return pawn->mesh->GetBoneName(y);
		}
		return baseReturn;
	}

	void SetLightShadow(bool disable)
	{
		if (this->Engine && this->Engine->GameViewport)
		{
			uintptr_t* viewModeIndex = (uintptr_t*)((uintptr_t)this->Engine->GameViewport + 0xb0); // 0xb0 = viewModeIndex
			if (disable) {
				if (*viewModeIndex == (uintptr_t)SDK::EViewModeIndex::VMI_Lit) // 0x3
				{
					*viewModeIndex = (uintptr_t)SDK::EViewModeIndex::VMI_Wireframe; // 0x1
				}
			}
			else {
				if (*viewModeIndex == (uintptr_t)SDK::EViewModeIndex::VMI_Wireframe) // 0x1
				{
					*viewModeIndex = (uintptr_t)SDK::EViewModeIndex::VMI_Lit; // 0x3
				}
			}
		}
	}

	//bool VisibleTraceLineCameraToFVector(SDK::FVector endLocation, SDK::UClass* staticActorToTounch)
	//{
	//	if (this->World && this->PlayerCameraManager && this->KismetSystemLib)
	//	{
	//		// TArray(ArrayElementType* Data, uint32 NumElements) : Data(Data), NumElements(NumElements), MaxElements(NumElements) {}
	//		// Add this in TArray
	//		SDK::TArray<SDK::AActor*> ignoreActor(reinterpret_cast<SDK::AActor**>(&this->myCharacter), 1);

	//		SDK::FHitResult hitResult;

	//		bool visisibleSocket = this->KismetSystemLib->LineTraceSingle(this->World, this->PlayerCameraManager->GetCameraLocation(), endLocation,
	//			SDK::ETraceTypeQuery::TraceTypeQuery1, false, ignoreActor, SDK::EDrawDebugTrace::ForDuration, &hitResult, true,
	//			SDK::FLinearColor(255, 255, 255), SDK::FLinearColor(0, 0, 255), 0.1f);

	//		SDK::AActor* actor = hitResult.Actor.Get();
	//		if (actor)
	//		{
	//			//ImGui::Text(actor->GetName().c_str());
	//			if (actor->IsA(staticActorToTounch))
	//			{
	//				return true;
	//			}
	//		}
	//	}

	//	return false;
	//}

	void DrawSkeletonFrom(SDK::USkeletalMeshComponent* Mesh, SDK::FName StartBone, const std::vector<int32_t>& bonnesToSkip)
	{
		if (!Mesh) return;

		const int32_t boneCount = Mesh->GetNumBones();
		if (boneCount <= 0) return;

		// -> Set O(1) pour tester rapidement si un os est à skipper
		std::unordered_set<int32_t> skipSet(bonnesToSkip.begin(), bonnesToSkip.end());

		std::vector<int32_t> parentOf(boneCount, -1);
		std::vector<std::vector<int32_t>> children(boneCount);
		for (int32_t i = 0; i < boneCount; ++i)
		{
			const SDK::FName boneName = Mesh->GetBoneName(i);
			const SDK::FName parentName = Mesh->GetParentBone(boneName); // peut être "None"
			const int32_t pIdx = Mesh->GetBoneIndex(parentName);
			parentOf[i] = pIdx;                   // -1 si racine
			if (pIdx >= 0) children[pIdx].push_back(i);
		}

		auto BoneWorld = [&](int32_t idx)->SDK::FVector {
			return Mesh->GetSocketLocation(Mesh->GetBoneName(idx));
			};

		auto W2Sok = [&](const SDK::FVector& w, SDK::FVector2D& out)->bool {
			out = this->w2s(w);
			return std::isfinite(out.X) && std::isfinite(out.Y);
			};

		const int32_t start = Mesh->GetBoneIndex(StartBone);
		if (start < 0) return;

		std::vector<uint8_t> visited(boneCount, 0);
		std::queue<int32_t> q;
		q.push(start);
		visited[start] = 1;

		// Pour éviter de tracer deux fois la même arête (parent-enfant)
		auto edgeKey = [](int32_t a, int32_t b)->uint64_t {
			if (a > b) std::swap(a, b);
			return (uint64_t(uint32_t(a)) << 32) | uint32_t(b);
			};
		std::unordered_set<uint64_t> drawn;

		while (!q.empty())
		{
			const int32_t cur = q.front(); q.pop();

			std::vector<int32_t> neigh = children[cur];
			if (parentOf[cur] >= 0) neigh.push_back(parentOf[cur]);

			// On calculera la position écran de cur seulement si on a une arête à dessiner
			SDK::FVector2D curS;
			bool curSok = false;

			for (const int32_t nb : neigh)
			{
				const bool skipEdge = (skipSet.count(cur) > 0) || (skipSet.count(nb) > 0);

				if (!skipEdge)
				{
					const uint64_t key = edgeKey(cur, nb);
					if (drawn.insert(key).second)
					{
						// Calcul paresseux de curS
						if (!curSok)
						{
							const SDK::FVector curW = BoneWorld(cur);
							if (!W2Sok(curW, curS)) { curSok = false; }
							else { curSok = true; }
						}

						if (curSok)
						{
							const SDK::FVector nbW = BoneWorld(nb);
							SDK::FVector2D nbS;
							if (W2Sok(nbW, nbS))
							{
								overlay::DrawLine(
									{ (float)curS.X, (float)curS.Y },
									{ (float)nbS.X,  (float)nbS.Y },
									CFG.skeletonPlayerColor, 1.0f
								);
							}
						}
					}
				}

				if (!visited[nb]) { visited[nb] = 1; q.push(nb); }
			}
		}
	}

	static inline SDK::FQuat MakeQuatFromRotator(const SDK::FRotator& R)
	{
		const double DegToRad = 3.14159265358979323846 / 180.0;

		const double HR = R.Roll * DegToRad * 0.5;  // X
		const double HP = R.Pitch * DegToRad * 0.5;  // Y
		const double HY = R.Yaw * DegToRad * 0.5;  // Z

		const double SR = std::sin(HR), CR = std::cos(HR);
		const double SP = std::sin(HP), CP = std::cos(HP);
		const double SY = std::sin(HY), CY = std::cos(HY);

		SDK::FQuat Q{};
		// Ordre Unreal: Yaw(Z) * Pitch(Y) * Roll(X)
		Q.X = SR * CP * CY - CR * SP * SY;
		Q.Y = CR * SP * CY + SR * CP * SY;
		Q.Z = CR * CP * SY - SR * SP * CY;
		Q.W = CR * CP * CY + SR * SP * SY;
		return Q;
	}

	static inline float DegToRad(float deg) {
		return deg * (3.14159265358979323846f / 180.0f);
	}

	static inline bool NearlyZero(float v, float eps = 1e-4f)
	{
		return (v > -eps) && (v < eps);
	}

	static inline bool IsDown(int vk)
	{
		return (GetAsyncKeyState(vk) & 0x8000) != 0;
	}

	// Adapt to each project with showBaseInfo
	void ToggleNoClip(bool bNoclip, bool showBaseInfo = false)
	{
		SDK::APlayerController* PC = this->PlayerController;
		if (!PC) return;
		if (!this->KismetSystemLib->IsValid(PC)) return;

		SDK::ACharacter* Char = static_cast<SDK::ACharacter*>(PC->Pawn);
		if (!Char) return;
		if (!this->KismetSystemLib->IsValid(Char)) return;

		SDK::UCharacterMovementComponent* Move = Char->CharacterMovement;
		SDK::UCapsuleComponent* Capsule = Char->CapsuleComponent;
		if (!Move || !Capsule) return;
		if (!this->KismetSystemLib->IsValid(Move)) return;
		if (!this->KismetSystemLib->IsValid(Capsule)) return;

		if (showBaseInfo)
		{
			std::string movementModeStr = "Movement mode: " + std::to_string((int)Move->MovementMode);
			ImGui::Text(movementModeStr.c_str());

			std::string gravityScaleStr = "Gravity scale: " + std::to_string(Move->GravityScale);
			ImGui::Text(gravityScaleStr.c_str());

			std::string BrakingFrictionFactorStr = "BrakingFrictionFactor: " + std::to_string(Move->BrakingFrictionFactor);
			ImGui::Text(BrakingFrictionFactorStr.c_str());

			std::string BrakingDecelerationFlyingStr = "BrakingDecelerationFlying: " + std::to_string(Move->BrakingDecelerationFlying);
			ImGui::Text(BrakingDecelerationFlyingStr.c_str());
			return;
		}
		else
		{
			if (bNoclip)
			{
				Move->SetMovementMode(SDK::EMovementMode::MOVE_Flying, 0);
				Move->GravityScale = 0.f;
				Move->MaxFlySpeed = 300.f;
				Move->BrakingFrictionFactor = 0.f;
				Move->BrakingDecelerationFlying = 1e6f;   // stop quasi-instant si jamais velocity ≠ 0
				Move->MaxAcceleration = 1e9f;   // changement direction immédiat

				// *** clé pour le Z ***
				Move->SetPlaneConstraintEnabled(false);
				Move->bConstrainToPlane = false; // si dispo
				Move->SetPlaneConstraintNormal(SDK::FVector{ 0.f, 0.f, 0.f }); // neutralise toute normale
				// optionnel, mais évite des effets bizarres :
				Move->bUseControllerDesiredRotation = false;
				Move->bOrientRotationToMovement = false;

				Move->Velocity = { 0.f,0.f,0.f };

				Capsule->SetCollisionEnabled(SDK::ECollisionEnabled::NoCollision);
				Char->SetActorEnableCollision(false);
			}
			else
			{
				Char->SetActorEnableCollision(true);
				Capsule->SetCollisionEnabled(SDK::ECollisionEnabled::QueryAndPhysics);
				Move->GravityScale = 1.f;
				Move->SetMovementMode(SDK::EMovementMode::MOVE_Walking, 0);
				Move->BrakingFrictionFactor = 2.f;
				Move->BrakingDecelerationFlying = 2048.f;
			}
		}
	}

	// Helpers pour convertir FRotator -> vecteurs (pitch/yaw en degrés, roll ignoré)
	static inline SDK::FVector GetForwardFromRot(const SDK::FRotator& r)
	{
		const float pitch = r.Pitch * (3.1415926535f / 180.f);
		const float yaw = r.Yaw * (3.1415926535f / 180.f);

		const float cp = cosf(pitch), sp = sinf(pitch);
		const float cy = cosf(yaw), sy = sinf(yaw);

		// Forward: inclut la composante verticale via le pitch
		return SDK::FVector{ cp * cy, cp * sy, sp };
	}

	static inline SDK::FVector GetRightFromRot(const SDK::FRotator& r)
	{
		// Right (yaw uniquement, roll ignoré pour éviter des dérives bizarres)
		const float yaw = r.Yaw * (3.1415926535f / 180.f);
		const float sy = sinf(yaw), cy = cosf(yaw);
		return SDK::FVector{ -sy, cy, 0.f };
	}

	static inline bool NormalizeSafe(SDK::FVector& v, float eps = 1e-6f) {
		const float ls = v.X * v.X + v.Y * v.Y + v.Z * v.Z;
		if (ls < eps) return false;
		const float inv = 1.0f / std::sqrt(ls);
		v.X *= inv; v.Y *= inv; v.Z *= inv;
		return true;
	}

	static inline SDK::FVector ForwardFromRot(const SDK::FRotator& r) {
		const float P = r.Pitch * (3.1415926535f / 180.f);
		const float Y = r.Yaw * (3.1415926535f / 180.f);
		const float cp = std::cos(P), sp = std::sin(P);
		const float cy = std::cos(Y), sy = std::sin(Y);
		return { cp * cy, cp * sy, sp };           // inclut bien la composante Z (pitch)
	}

	static inline SDK::FVector RightFromRot(const SDK::FRotator& r) {
		const float Y = r.Yaw * (3.1415926535f / 180.f);
		const float sy = std::sin(Y), cy = std::cos(Y);
		return { -sy, cy, 0.f };               // roll ignoré
	}

	static inline float GetDeltaTime()
	{
		static LARGE_INTEGER freq = {};
		static LARGE_INTEGER prev = {};
		if (freq.QuadPart == 0) {
			QueryPerformanceFrequency(&freq);
			QueryPerformanceCounter(&prev);
			return 0.0f;
		}
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		const double dt = double(now.QuadPart - prev.QuadPart) / double(freq.QuadPart);
		prev = now;
		// clamp pour éviter spikes (pause, alt-tab, etc.)
		return (float)std::clamp(dt, 0.0, 0.05); // max 50 ms
	}

	void NoclipTick()
	{
		if (!CFG.b_miscNoClip) return;

		auto* PC = this->PlayerController;
		if (!PC)   return;
		if (!this->KismetSystemLib->IsValid(PC)) return;

		auto* Char = static_cast<SDK::ACharacter*>(PC->Pawn);
		if (!Char) return;
		if (!this->KismetSystemLib->IsValid(Char)) return;

		auto* Move = Char->CharacterMovement;
		if (!Move)   return;
		if (!this->KismetSystemLib->IsValid(Move)) return;

		// Inputs (AZERTY + fallback)
		int ix = 0, iy = 0;
		if (IsDown('D') || IsDown(VK_RIGHT)) ix += 1;
		if (IsDown('Q') || IsDown('A') || IsDown(VK_LEFT)) ix -= 1;
		if (IsDown('Z') || IsDown('W') || IsDown(VK_UP)) iy += 1;
		if (IsDown('S') || IsDown(VK_DOWN)) iy -= 1;

		if (ix == 0 && iy == 0) {
			if (Move) Move->Velocity = { 0.f,0.f,0.f };
			return;
		}

		// Rotation caméra (prends la vraie cam si possible)
		SDK::FRotator camRot = PC->ControlRotation;
		if (PC->PlayerCameraManager) camRot = PC->PlayerCameraManager->GetCameraRotation();

		// Forward/Right alignés UE (Z = -sin(Pitch))
		SDK::FVector fwd, rgt;
		if (this->MathLib) {
			fwd = this->MathLib->GetForwardVector(camRot);
			rgt = this->MathLib->GetRightVector(camRot);
		}
		else {
			const float P = DegToRad(camRot.Pitch);
			const float Y = DegToRad(camRot.Yaw);
			const float cp = std::cos(P), sp = std::sin(P);
			const float cy = std::cos(Y), sy = std::sin(Y);
			fwd = { cp * cy, cp * sy, -sp };   // <- signe correct
			rgt = { -sy,   cy,     0.f };
		}

		SDK::FVector dir = fwd * float(iy) + rgt * float(ix);
		if (!NormalizeSafe(dir)) return;

		float speed = (Move && Move->MaxFlySpeed > 0.f) ? Move->MaxFlySpeed : 2400.f;
		if (IsDown(VK_SHIFT)) speed *= 2.5f;
		if (IsDown(VK_MENU))  speed *= 0.15f;

		const float dt = GetDeltaTime();
		const SDK::FVector step = dir * (speed * dt);

		// Téléportation par offset monde = bypass total du MovementComponent
		Char->K2_AddActorWorldOffset(step, false, nullptr, true);

		// Pour rester cohérent avec le reste (FX, UI…)
		if (Move) Move->Velocity = dir * speed;
	}
};

inline UnrealEngine* ue = new UnrealEngine();