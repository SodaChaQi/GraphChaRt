// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EditorPathNode.generated.h"

UCLASS()
class GRAPHCHARTEDITOR_API AEditorPathNode : public AActor
{
	GENERATED_BODY()
	
public:	

	AEditorPathNode();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graph|Editor")
	FName NodeName;

	
#if WITH_EDITOR
	
	virtual void PostActorCreated() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
	
#endif


protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Graph|Editor")
	UStaticMeshComponent* PreviewMesh;
	
};
