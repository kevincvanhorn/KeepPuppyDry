// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "PPlayerController.h"
#include "PSwipeDelegates.h"
#include "Engine/World.h"
#include "PUmbrella.h"
#include "Materials/MaterialParameterCollection.h"
#include "Public/PUserWidget.h"
#include "PPlayerState.h"
#include "Components/StaticMeshComponent.h"

#include "KeepThePuppyDry.h"
#include "PSwipeDelegates.h"
#include "PCustomizationManager.h"
#include "PMainMenuWidget.h"
#include "TimerManager.h"
#include "PPuppyCharacter.h"
#include "PPreviewPuppy.h"

// Sets default values
APPlayer::APPlayer()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	if (SpringArm) {
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->bEnableCameraLag = false;
	}
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	if (Camera) {
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	}

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bScreenTouched = false;
	ClampZNeg = ClampZNeg = UReleasePosition.Z;
	UOverlapTime = 0;
	bTryUpdateTouchEvents = true;
	//bUOverlapping = false;
}

void APPlayer::SetMainMenuWidget(UPMainMenuWidget* WidgetIn)
{
	PMenuWidget = WidgetIn;
}

void APPlayer::Initialize(APPlayerState* PlayerStateIn)
{
	PPlayerState = PlayerStateIn;
	if (CustomizationManager) {
		CustomizationManager->SetPPlayerState(PPlayerState);
	}
}

FVector APPlayer::GetUmbrellaLocation() const
{
	if (Umbrella) {
		return Umbrella->GetActorLocation();
	}
	return FVector::ZeroVector;
}

void APPlayer::ZoomIn()
{
	TargetZoomFactor = InZoomFactor;
	GetWorldTimerManager().SetTimer(ZoomHandle, this, &APPlayer::UpdateCameraZoom, 0.01f, true, 0.0f);
}

void APPlayer::ZoomOut()
{
	TargetZoomFactor = OutZoomFactor;
	GetWorldTimerManager().SetTimer(ZoomHandle, this, &APPlayer::UpdateCameraZoom, 0.01f, true, 0.0f);
}

void APPlayer::OnGameTutorial() {
	bTryUpdateTouchEvents = false;
	if (PUserWidget && PPlayerState) {
		PUserWidget->UpdateHealth(PPlayerState->GetHealth());
		PUserWidget->UpdateScore(PPlayerState->GetScore());
	}

	SpawnPuppy();

	this->SetActorTickEnabled(false);
}

void APPlayer::OnGameStart()
{
	if (!bTryUpdateTouchEvents) {
		bTryUpdateTouchEvents = true;
		// Tutorial just finished
		OnTouchBegin();
	}
	bTryUpdateTouchEvents = true;
	if (PUserWidget && PPlayerState) {
		PUserWidget->UpdateHealth(PPlayerState->GetHealth());
		//PUserWidget->UpdateScore(PPlayerState->GetScore());
	}
	this->SetActorTickEnabled(true);
}

void APPlayer::OnGameOver()
{
	this->SetActorTickEnabled(false);
	bTryUpdateTouchEvents = false;
}

// Called when the game starts or when spawned
void APPlayer::BeginPlay()
{
	Super::BeginPlay();
	PPlayerController = Cast<APPlayerController>(Controller);

	// Subscribe all delegates:
	//UPSwipeDelegates::DoubleTapDelegate.AddUObject(this, &APPlayer::OnDoubleTap);
	//UPSwipeDelegates::SwipeUpDelegate.AddUObject(this, &APPlayer::OnSwipeUp);
	//UPSwipeDelegates::SwipeDownDelegate.AddUObject(this, &APPlayer::OnSwipeDown);
	UPSwipeDelegates::PostBeginPlayDelegate.AddUObject(this, &APPlayer::PostBeginPlay);

	UPSwipeDelegates::TouchBeganDelegate.AddUObject(this, &APPlayer::OnTouchBegin);
	UPSwipeDelegates::TouchEndedDelegate.AddUObject(this, &APPlayer::OnTouchEnd);
	UPSwipeDelegates::GameOverDelegate.AddUObject(this, &APPlayer::OnGameOver);
	UPSwipeDelegates::GameStartDelegate.AddUObject(this, &APPlayer::OnGameStart);
	UPSwipeDelegates::GameTutorialDelegate.AddUObject(this, &APPlayer::OnGameTutorial);

	if (UmbrellaClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Umbrella = (APUmbrella*)GetWorld()->SpawnActor<APUmbrella>(UmbrellaClass, UmbrellaSpawnLoc, FRotator(0,0,0), SpawnParams);
		if (Umbrella) {
			Umbrella->SetMPC(MPC_Umbrella);
			Umbrella->Initialize(this, UTouchPosition, UReleasePosition, PPlayerController);
			Umbrella->SetClampZValues(ClampZPos, ClampZNeg);
		}
	}

	if (CustomizationManagerClass) {
		CustomizationManager = GetWorld()->SpawnActor<APCustomizationManager>(CustomizationManagerClass);
		if (CustomizationManager) {
			UStaticMeshComponent* UIMesh = nullptr;
			USkeletalMeshComponent* UIPuppy = nullptr;
			if (UmbrellaMeshUIActor) {
				UIMesh = UmbrellaMeshUIActor->GetStaticMeshComponent();
			}
			if (PreviewPuppy) {
				UIPuppy = PreviewPuppy->GetSkeletalMeshComponent();
			}
			CustomizationManager->PInit(Umbrella, UIMesh, UIPuppy);
		}
	}

	if (SpringArm) {
		InZoomFactor = SpringArm->TargetArmLength;
	}

	//SpawnPuppy();
}

void APPlayer::PostBeginPlay()
{
	if (CustomizationManager && PPlayerState) {
		if (PPlayerState->LastSelected_UmbrellaPattern >= 0) { // -1 is none
			CustomizationManager->SelectUmbrellaPattern((EUmbrellaPattern)PPlayerState->LastSelected_UmbrellaPattern);
		}
		if (PPlayerState->LastSelected_Dog >= 0) {
			CustomizationManager->SelectDogChoice((EDogChoice)PPlayerState->LastSelected_Dog);
		}
	}
	if (PMenuWidget) {
		PMenuWidget->SetCustomizationManager(CustomizationManager);
	}
}

/*void APPlayer::OnDifficultyChangedInternal(int32 NewDifficulty)
{
	this->OnDifficultyChanged(NewDifficulty);
}*/

// Called every frame
void APPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUOverlapping) {
		UOverlapTime += DeltaTime;
		if (PPlayerState && PUserWidget && Umbrella) {
			//PUserWidget->UpdateScore(PPlayerState->ScoreFromTime(UOverlapTime));
			float fOverlap = 2.0f * (Umbrella->GetOverlapPercentage() - 0.5f);
			PUserWidget->UpdateHealth(PPlayerState->ChangeHealth(DeltaTime * fOverlap));
		}
	}
	else {
		if (PUserWidget) {
			PUserWidget->UpdateHealth(PPlayerState->SubtractHealth(DeltaTime));
		}
	}
}

// Called to bind functionality to input
void APPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//InputComponent->BindAction("Touch1", IE_Pressed);

	InputComponent->BindAxis("Pinch", this, &APPlayer::Zoom);
}

void APPlayer::OnTouchMoved(const FVector2D& DeltaTouch) {
	
}

void APPlayer::UpdateTouchLoc(FVector2D TouchLocIn)
{
	TouchLoc = TouchLocIn;

	if (!bTryUpdateTouchEvents) return;

	/*if (PUserWidget) {
		PUserWidget->SetTouchDragPosition(TouchLocIn);
	}*/

	if (Umbrella) {
		Umbrella->MoveFromScreenLoc(TouchLocIn);
	}
}

void APPlayer::OnSwipeUp()
{
}

void APPlayer::OnSwipeDown()
{
}

void APPlayer::OnDoubleTap()
{
}

void APPlayer::OnTouchBegin()
{
	bScreenTouched = true;
	if (!bTryUpdateTouchEvents) return;

	if (Umbrella) {
		if (B_LEVEL_SIMPLE) {
			Umbrella->LerpToPosition(UTouchPosition);
		}
		else {
			Umbrella->OnTouchBegin(TouchLoc);
		}
	}
	this->OnUTouchBegin();
}

void APPlayer::OnTouchEnd()
{
	bScreenTouched = false;

	if (!bTryUpdateTouchEvents) return;

	if (Umbrella) {
		Umbrella->OnTouchEnd();
	}
	this->OnUTouchEnd();
}

void APPlayer::Zoom(float AxisValue)
{
}

void APPlayer::OnUmbrellaOverlapBegin()
{
	bUOverlapping = true;
}

void APPlayer::OnUmbrellaOverlapEnd()
{
	bUOverlapping = false;
}

void APPlayer::UpdateCameraZoom()
{
	if (SpringArm) {
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetZoomFactor, 0.01f, ZoomSpeed);
		if (FMath::Abs(SpringArm->TargetArmLength - TargetZoomFactor) < 0.1f) {
			GetWorldTimerManager().ClearTimer(ZoomHandle);
		}
	}
}

void APPlayer::SpawnPuppy()
{
	if (CustomizationManager) {
		TSubclassOf<APPuppyCharacter> PuppyClass = CustomizationManager->GetDogChoice();
		APPuppyCharacter* SpawnedPuppy = nullptr;
		if (PuppyClass) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnedPuppy = (APPuppyCharacter*)GetWorld()->SpawnActor<APPuppyCharacter>(PuppyClass, PuppyTransform, SpawnParams);
		}
		if (SpawnedPuppy) {
			UPSwipeDelegates::PostSpawnPuppyDelegate.Broadcast();
		}
	}
}
