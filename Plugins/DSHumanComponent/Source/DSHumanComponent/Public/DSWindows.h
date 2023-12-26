// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widgets/SWindow.h"
#include "DSWindows.generated.h"

UCLASS()
class DSHUMANCOMPONENT_API ADSWindows : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADSWindows();

	TSharedPtr<SWindow> window = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "DSWindows")
		void CreateWindow(
			UUserWidget* ShowWidget, bool bCreateTitleBar = false,
			FString title = FString(TEXT("动捕设置界面")),
			FVector2D WindowPositon = FVector2D(200, 200),
			FVector2D WindowSize = FVector2D(1080, 720),
			bool bAutoCenter = true,
			bool bHasCloseButton = false,
			bool bHasMinimizeButton = false,
			bool bHasMaximizeButton = false);

	UFUNCTION(BlueprintCallable, Category = "DSWindows")
		void ShowWindow();

	UFUNCTION(BlueprintCallable, Category = "DSWindows")
		void HideWindow();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "DSWindows")
		bool IsVisible();

	UFUNCTION(BlueprintCallable, Category = "DSWindows")
		void Resize(FVector2D NewSize);

	UFUNCTION(BlueprintCallable, Category = "DSWindows")
		FVector2D GetSize()const;



};
