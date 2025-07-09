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

void AEditorPathNode::BeginPlay()
{
	Super::BeginPlay();

	if (!GIsEditor) Destroy();
}

