// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VehicleGameMode.generated.h"

class USoundCue;

enum class EGameState : short
{
	EWait,
	EPlaying,
};

/**
 * 
 */
UCLASS()
class TESTCAR_API AVehicleGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AVehicleGameMode();
	virtual void StartPlay() override;

	EGameState CurrentState;
	
protected:
	void CutTime();

	FTimerHandle TimerHandle_TimeCount;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	int CountTime;

	USoundCue* CutTimeSoundCue;
	USoundCue* BeginPlaySoundCue;
};
