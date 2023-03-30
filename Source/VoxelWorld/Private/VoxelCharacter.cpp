// Fill out your copyright notice in the Description page of Project Settings.


#include "VoxelCharacter.h"

#include "AGameWorld.h"

// Sets default values
AVoxelCharacter::AVoxelCharacter()
{
  // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVoxelCharacter::BeginPlay()
{
  Super::BeginPlay();
}

// Called every frame
void AVoxelCharacter::Tick(float deltaTime)
{
  Super::Tick(deltaTime);
}

// Called to bind functionality to input
void AVoxelCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
  Super::SetupPlayerInputComponent(playerInputComponent);

  playerInputComponent->BindAction("Dig", IE_Pressed, this, &AVoxelCharacter::Dig);
  playerInputComponent->BindAction("Save", IE_Pressed, this, &AVoxelCharacter::Save);
  playerInputComponent->BindAction("Load", IE_Pressed, this, &AVoxelCharacter::Load);
}

void AVoxelCharacter::Dig()
{
  // raycast to get the voxel
  FHitResult hitResult;
  GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(),
                                       GetActorLocation() + GetActorForwardVector() * 1000, ECC_Visibility);


  // (- Normal) -> inside the block; now we are sure that we are inside the block
  FVector voxelLocation = hitResult.Location - hitResult.Normal * 50;
  if (voxelLocation.X < 0) voxelLocation.X -= 100;
  if (voxelLocation.Y >= 0) voxelLocation.Y += 100;

  if (hitResult.bBlockingHit)
  {
    OnDig.Broadcast(voxelLocation);
  }
}

void AVoxelCharacter::Save()
{
  OnGameSave.Broadcast();
}

void AVoxelCharacter::Load()
{
  OnGameLoad.Broadcast();
}
