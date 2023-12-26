// Fill out your copyright notice in the Description page of Project Settings.
#include "Model/TerrianGenerationMode.h"


ATerrianGenerationMode::ATerrianGenerationMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;
	

}

void ATerrianGenerationMode::BeginPlay()
{
	Super::BeginPlay();
	//BBlocks = GetWorld()->SpawnActor<AInstBlock>(FVector::ZeroVector, FRotator::ZeroRotator);
}

void ATerrianGenerationMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//GameEnd = true;
}

void ATerrianGenerationMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}



