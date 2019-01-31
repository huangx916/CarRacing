// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "MyVehicle.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USoundCue;
class UAudioComponent;
class UWheeledVehicleMovementComponent4W;
class AVehicleGameMode;

/**
 * 
 */
UCLASS()
class TESTCAR_API AMyVehicle : public AWheeledVehicle
{
	GENERATED_BODY()

public:
	AMyVehicle();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "CameraComponent")
		UCameraComponent* ExternalCamera;
	UPROPERTY(VisibleAnywhere, Category = "CameraComponent")
		UCameraComponent* InternalCamera;
	UPROPERTY(VisibleAnywhere, Category = "CameraComponent")
		USceneComponent* InternalCameraBase;

	FVector InternalCameraOrigin;

	UPROPERTY(VisibleAnywhere, Category = "SpringArmComponent")
		USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundCue* EngineSound;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SteerSound;
	UPROPERTY(VisibleAnywhere, Category = "Sound")
		UAudioComponent* SteerAudioComp;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void OnHandbrakePressed();
	void OnHandbrakeReleased();
	void OnToggleCamera();

	virtual void BeginPlay() override;

	void PlayEngineSound();

	bool bInCarCameraActive;

	bool bHandbraking;

	UWheeledVehicleMovementComponent4W* Vehicle4W;

	void OnSkid();

	TArray<int32> GearSpeedArray;

	float FrontSteerAngle;

	AVehicleGameMode* VehicleGameMode;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	float CurrentSpeedOnDisplay;
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	int CurrentGear;
};
