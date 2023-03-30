// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Delegates/Delegate.h"
#include "VoxelCharacter.generated.h"

UCLASS()
class VOXELWORLD_API AVoxelCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AVoxelCharacter();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector RaycastOrigin;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float deltaTime) override;
	void Save();
	void Load();
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;
	void Dig();
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDig, FVector, location);
	FOnDig OnDig;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameSave);
	FOnGameSave OnGameSave;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameLoad);
	FOnGameLoad OnGameLoad;
	
};
