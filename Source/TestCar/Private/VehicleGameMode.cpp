// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleGameMode.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "ConstructorHelpers.h"

AVehicleGameMode::AVehicleGameMode()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> CTSC(TEXT("/Game/Sound/CutDown/UI_Countdown_Cue"));
	CutTimeSoundCue = CTSC.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> BPSC(TEXT("/Game/Sound/CutDown/UI_CountdownFinish_Cue"));
	BeginPlaySoundCue = BPSC.Object;
	CountTime = 3;
	CurrentState = EGameState::EWait;
}

void AVehicleGameMode::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_TimeCount, this, &AVehicleGameMode::CutTime, 1.0f, true, 1.0f);
}

void AVehicleGameMode::CutTime()
{
	if (CountTime == 0)
	{
		CurrentState = EGameState::EPlaying;
		GetWorldTimerManager().ClearTimer(TimerHandle_TimeCount);
		if(BeginPlaySoundCue)
			UGameplayStatics::PlaySoundAtLocation(this, BeginPlaySoundCue, FVector(0, 0, 0));
	}
	else
	{
		if(CutTimeSoundCue)
			UGameplayStatics::PlaySoundAtLocation(this, CutTimeSoundCue, FVector(0, 0, 0));
		CountTime--;
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(CountTime));
}
