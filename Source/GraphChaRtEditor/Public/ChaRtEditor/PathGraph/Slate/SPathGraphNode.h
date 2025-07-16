#pragma once

#include "SGraphNode.h"


class SConstraintCanvas;
class UPathEdGraphNode;
class SGraphPin;

const FVector2D NormalizedSize = FVector2D(200, 100);

class SPathGraphNode : public SGraphNode
{
	
public:

	SLATE_BEGIN_ARGS(SPathGraphNode) {}
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPathEdGraphNode* InNode);


	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;

	FORCEINLINE virtual FVector2D ComputeDesiredSize(float) const override { return NormalizedSize; }

protected:

	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;

private:

	virtual TSharedRef<SWidget> CreateNodeContentArea() override;

	TSharedPtr<SConstraintCanvas> Canvas;
};
