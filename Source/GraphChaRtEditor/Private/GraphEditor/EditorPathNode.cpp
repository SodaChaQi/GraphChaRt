// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphEditor/EditorPathNode.h"




AEditorPathNode::AEditorPathNode()
	: NodeName(NAME_None)
{
	PrimaryActorTick.bCanEverTick = false;
	bIsEditorOnlyActor = true;

	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	SetRootComponent(PreviewMesh);

	if (GIsEditor)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshFinder(
			TEXT("/Engine/EditorMeshes/Axis_Guide"));
		if (MeshFinder.Succeeded()) PreviewMesh->SetStaticMesh(MeshFinder.Object);
	}

}

#if WITH_EDITOR

void AEditorPathNode::PostActorCreated()
{
	Super::PostActorCreated();

	if (GetWorld() && GetWorld()->WorldType != EWorldType::EditorPreview)
	{
		// if (UGraphEditorSubsystem* GraphEditorSubsystem = GEditor->GetEditorSubsystem<UGraphEditorSubsystem>())
		// {
		// 	
		// }
	}
}

void AEditorPathNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void AEditorPathNode::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);
}

#endif

void AEditorPathNode::BeginPlay()
{
	Super::BeginPlay();

	if (!GIsEditor) Destroy();
}

