// Fill out your copyright notice in the Description page of Project Settings.


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

#include "KeepThePuppyDry.h"

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
}

// Called when the game starts or when spawned
void APPlayer::BeginPlay()
{
	Super::BeginPlay();
	PPlayerController = Cast<APPlayerController>(Controller);

	// Subscribe all delegates:
	UPSwipeDelegates::DoubleTapDelegate.AddUObject(this, &APPlayer::OnDoubleTap);
	UPSwipeDelegates::SwipeUpDelegate.AddUObject(this, &APPlayer::OnSwipeUp);
	UPSwipeDelegates::SwipeDownDelegate.AddUObject(this, &APPlayer::OnSwipeDown);
	UPSwipeDelegates::PostBeginPlayDelegate.AddUObject(this, &APPlayer::PostBeginPlay);

	UPSwipeDelegates::TouchBeganDelegate.AddUObject(this, &APPlayer::OnTouchBegin);
	UPSwipeDelegates::TouchEndedDelegate.AddUObject(this, &APPlayer::OnTouchEnd);

	if (UmbrellaClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Umbrella = (APUmbrella*)GetWorld()->SpawnActor<APUmbrella>(UmbrellaClass, UmbrellaSpawnLoc, FRotator(0,0,0), SpawnParams);
		if (Umbrella) {
			Umbrella->SetMPC(MPC);
			Umbrella->Initialize(UTouchPosition, UReleasePosition, PPlayerController);
		}
	}
}

void APPlayer::PostBeginPlay()
{
}

// Called every frame
void APPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	if (B_LEVEL_SIMPLE) return;

	if (PUserWidget) {
		PUserWidget->SetTouchDragPosition(TouchLocIn);
	}

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

	if (Umbrella) {
		Umbrella->MoveToPosition(UTouchPosition);
	}
	this->OnUTouchBegin();
}

void APPlayer::OnTouchEnd()
{
	bScreenTouched = false;
	if (Umbrella) {
		Umbrella->MoveToPosition(UReleasePosition);
	}
	this->OnUTouchEnd();
}

void APPlayer::Zoom(float AxisValue)
{
}
