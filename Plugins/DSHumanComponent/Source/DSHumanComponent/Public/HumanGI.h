// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DSLibrary.h"
#include "HumanGI.generated.h"

/**
 *
 */
UCLASS()
class DSHUMANCOMPONENT_API UHumanGI : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "DSGI")
		FString IP = TEXT("192.168.123.2");

	UPROPERTY(BlueprintReadWrite, Category = "DSGI")
		int32 Port = 8899;

	UPROPERTY(BlueprintReadWrite, Category = "DSGI")
		EDataOptions DataOptions = EDataOptions::monocular;

	UPROPERTY(BlueprintReadWrite, Category = "DSGI")
		float MinFloat = 0.3f;

	UPROPERTY(BlueprintReadWrite, Category = "DSGI")
		float MaxFloat = 0.5f;
	UPROPERTY(BlueprintReadWrite, Category = "DSGI")
		float FigMaxFloat = 0.5f;
};
