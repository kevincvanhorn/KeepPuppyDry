// Fill out your copyright notice in the Description page of Project Settings.


#include "PPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "PPlayerController.h"
#include "PSwipeDelegates.h"

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

	// Set Defaults:
	StartLoc = GetActorLocation();
	
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

void APPlayer::UpdateTouchLoc(FVector2D _TouchLoc)
{
	TouchLoc = _TouchLoc;
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

void APPlayer::Zoom(float AxisValue)
{
}
