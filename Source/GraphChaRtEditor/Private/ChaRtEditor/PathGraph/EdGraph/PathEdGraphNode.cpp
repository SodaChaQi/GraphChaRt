// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaRtEditor/PathGraph/EdGraph/PathEdGraphNode.h"

UPathEdGraphNode::UPathEdGraphNode()
{
	NodeComment = TEXT("Path Node");
}

void UPathEdGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input,  TEXT("MultipleNodes"), FName(), nullptr, TEXT("In"));
	CreatePin(EGPD_Output, TEXT("MultipleNodes"), FName(), nullptr, TEXT("Out"));
}

FText UPathEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Path Node"));
}
