// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PathGraphSubsystem.generated.h"

class UPathGraph;
/**
 * 
 */

USTRUCT()
struct FPathGraphs
{
	GENERATED_BODY()

	UPROPERTY()
	TSet<UPathGraph*> Graphs;
};

UCLASS()
class GRAPHCHARTRUNTIME_API UPathGraphSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

	UPROPERTY()
	TMap<ULevel*, FPathGraphs> PathGraphs;
	
	void OnLevelLoaded(ULevel* Level, UWorld* World);
	void OnLevelUnloaded(ULevel* Level, UWorld* World);
};
