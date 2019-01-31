// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "MyWheeledVehicle.generated.h"

class UCameraComponent;

/**
 * 
 */
UCLASS()
class TESTCAR_API AMyWheeledVehicle : public AWheeledVehicle
{
	GENERATED_BODY()
	
public:
	AMyWheeledVehicle();
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "CameraComponent")
		UCameraComponent* ExternalCamera;
	
};
