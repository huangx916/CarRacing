// Fill out your copyright notice in the Description page of Project Settings.

#include "MyWheeledVehicle.h"
#include "Camera/CameraComponent.h"

AMyWheeledVehicle::AMyWheeledVehicle()
{
	ExternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ExternalCamera"));
	ExternalCamera->SetupAttachment(RootComponent);
}


