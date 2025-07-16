#pragma once


#include "CoreMinimal.h"
#include "ConnectionDrawingPolicy.h"
#include "PathEdGraphNode.h"
#include "EdGraph/EdGraphSchema.h"
#include "PathEdGraphSchema.generated.h"


class UPathEdGraph;

class FPathEdGraphConnectionDrawingPolicy : public FConnectionDrawingPolicy
{
public:

	FPathEdGraphConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID,
	float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements)
	: FConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements)
	{}

	virtual void DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom, const FConnectionParams& Params) override;
	
};

template<typename NodeType>
struct TPathEdGraphSchemaAction : public FEdGraphSchemaAction
{
	TPathEdGraphSchemaAction(
	const FText& InNodeCategory,
	const FText& InMenuDesc,
	const FText& InToolTip,
	int32 InGrouping
	)
	: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping)
	{}
	
	TFunction<NodeType*(UEdGraph*, UEdGraphPin*, const FVector2D&)> Action;

	virtual UEdGraphNode* PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode = true) override
	{
		if (Action)
		{
			return Action(ParentGraph, FromPin, Location);
		}
		return nullptr;
	}
};

UCLASS()
class UPathEdGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:

	UPathEdGraphSchema(const FObjectInitializer& ObjectInitializer);
	
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	}

	virtual FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(
		int32 InBackLayerID,
		int32 InFrontLayerID,
		float InZoomFactor,
		const FSlateRect& InClippingRect,
		class FSlateWindowElementList& InDrawElements,
		class UEdGraph* InGraphObj) const override;

	static UPathEdGraphNode* AddPathEdGraphNodeToGraph(UPathEdGraph* Graph, const FVector2D& Position);
};