// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GraphChaRtSettings.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FPathGraphPaths
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GraphSettings")
	TSet<FSoftObjectPath> PathGraphPaths;
};


UCLASS(Config = GraphChaRt, DefaultConfig)
class GRAPHCHARTCORE_API UGraphChaRtSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	FORCEINLINE virtual FName GetContainerName() const override { return FName("project"); }
	
	FORCEINLINE virtual FName GetCategoryName() const override { return FName("GraphChaRt"); }
	
	FORCEINLINE virtual FName GetSectionName() const override { return FName("GraphChaRtSettings"); }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LogSettings")
	bool bEnableLogging = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GraphSettings")
	FString GraphAssetPath = FPaths::EnginePluginsDir() + TEXT("GraphChaRt/Content");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GraphSettings")
	TMap<FSoftObjectPath, FPathGraphPaths> LevelPathGraphPaths;

	virtual void PostSaveRoot(FObjectPostSaveRootContext ObjectSaveContext) override;
};
