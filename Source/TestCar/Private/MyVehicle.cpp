// Fill out your copyright notice in the Description page of Project Settings.

#include "MyVehicle.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "VehicleGameMode.h"
#include "Kismet/GameplayStatics.h"

AMyVehicle::AMyVehicle()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0, 0));
	SpringArm->TargetOffset = FVector(0, 0, 200.0f);
	SpringArm->TargetArmLength = 750.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.0f;

	ExternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ExternalCamera"));
	ExternalCamera->SetupAttachment(SpringArm);

	InternalCameraOrigin = FVector(-16, -23, 20);
	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetupAttachment(GetMesh());
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->SetupAttachment(InternalCameraBase);

	Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	check(Vehicle4W->WheelSetups.Num() == 4);
	Vehicle4W->WheelSetups[0].BoneName = FName("right_front_bone");
	Vehicle4W->WheelSetups[1].BoneName = FName("left_front_bone");
	Vehicle4W->WheelSetups[2].BoneName = FName("right_rear_bone");
	Vehicle4W->WheelSetups[3].BoneName = FName("left_rear_bone");

	bInCarCameraActive = false;

	bHandbraking = false;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);

	SteerAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("SteerAudioComp"));
	SteerAudioComp->SetupAttachment(RootComponent);

	GearSpeedArray = { 0, 18, 43, 62, 120, 151 };

	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Game/Sound/CanEngine_F_midhigh-register_mono_Cue"));
	EngineSound = SoundCue.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> SteerSoundCue(TEXT("/Game/Sound/Car_skid1_Cue"));
	SteerSound = SteerSoundCue.Object;

	VehicleGameMode = Cast<AVehicleGameMode>(UGameplayStatics::GetGameMode(this));
}

void AMyVehicle::BeginPlay()
{
	Super::BeginPlay();
	if (EngineSound->IsValidLowLevelFast())
	{
		EngineSound->PitchMultiplier = 0.1f;
		AudioComp->SetSound(EngineSound);
	}
	if (SteerSound->IsValidLowLevelFast())
	{
		SteerAudioComp->SetSound(SteerSound);
	}
}

void AMyVehicle::MoveForward(float Value)
{
	if (VehicleGameMode->CurrentState == EGameState::EPlaying)
	{
		GetVehicleMovementComponent()->SetThrottleInput(Value);
		PlayEngineSound();
	}
}

void AMyVehicle::MoveRight(float Value)
{
	if (VehicleGameMode->CurrentState == EGameState::EPlaying)
	{
		FrontSteerAngle = Value;
		GetVehicleMovementComponent()->SetSteeringInput(Value);
	}
}

void AMyVehicle::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
	bHandbraking = true;
}

void AMyVehicle::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
	bHandbraking = false;
}

void AMyVehicle::OnToggleCamera()
{
	if (bInCarCameraActive)
	{
		ExternalCamera->Activate();
		InternalCamera->Deactivate();
		bInCarCameraActive = false;
	}
	else
	{
		InternalCamera->Activate();
		ExternalCamera->Deactivate();
		bInCarCameraActive = true;
	}
}

void AMyVehicle::PlayEngineSound()
{
	int Index = 0;
	for (int i = 0; i < GearSpeedArray.Num()-2; i++)
	{
		if (GetVehicleMovement()->GetForwardSpeed()*0.036f >= GearSpeedArray[i])
		{
			Index = i;
		}
	}
	int MinSpeed = GearSpeedArray[Index];
	int MaxSpeed = GearSpeedArray[Index + 1];
	EngineSound->PitchMultiplier = 0.1f + (FMath::Abs(GetVehicleMovement()->GetForwardSpeed())*0.036f - MinSpeed) / (MaxSpeed - MinSpeed) * 0.8f;
	////EngineSound->PitchMultiplier = 0.2f + GetVehicleMovement()->GetForwardSpeed() * 0.036f / 152.0f;
	if (!AudioComp->IsPlaying())
	{
		AudioComp->Play();
	}
}

void AMyVehicle::OnSkid()
{
	//if (GetVehicleMovement()->GetForwardSpeed() * 0.036f > 40 && FMath::Abs(FrontSteerAngle) > 0.8f)
	if ((GetVehicleMovement()->GetForwardSpeed() * 0.036f > 40 && FMath::Abs(Vehicle4W->Wheels[0]->GetSteerAngle()) > 20.0f)
		||(GetVehicleMovement()->GetForwardSpeed() * 0.036f > 20 && FMath::Abs(Vehicle4W->Wheels[0]->GetSteerAngle()) > 10.0f && bHandbraking))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnSkid"));
		if (!SteerAudioComp->IsPlaying())
		{
			SteerAudioComp->Play();
		}
	}
	else
	{
		SteerAudioComp->Stop();
	}
}

void AMyVehicle::Tick(float DeltaTime)
{
	OnSkid();
	CurrentSpeedOnDisplay = FMath::Clamp(GetVehicleMovement()->GetForwardSpeed() * 0.036f * 270 / 140 - 180, -180.0f, 90.0f);
	CurrentGear = GetVehicleMovement()->GetCurrentGear();
}

void AMyVehicle::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyVehicle::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyVehicle::MoveRight);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AMyVehicle::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AMyVehicle::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &AMyVehicle::OnToggleCamera);
}


