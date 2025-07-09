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

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PathNodeEditor")
	UStaticMeshComponent* PreviewMesh;
	
};
