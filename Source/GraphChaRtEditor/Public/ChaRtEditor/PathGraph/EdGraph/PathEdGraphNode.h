// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "PathEdGraphNode.generated.h"

/**
 * 
 */

UCLASS()
class GRAPHCHARTEDITOR_API UPathEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UPathEdGraphNode();

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	// virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	//
	// virtual void OnPinRemoved(UEdGraphPin* InRemovedPin) override;

	// void AddPin(
	// 	EEdGraphPinDirection Direction,
	// 	const FName& PinName);
};
