#pragma once

#include "SGraphNode.h"
#include "GraphEditor/Slate/PathGraph/SPathGraphPin.h"

class SPathGraphNode : public SGraphNode
{
	
public:

	SLATE_BEGIN_ARGS(SPathGraphNode) {}
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);


	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;

private:

	virtual TSharedRef<SWidget> CreateNodeContentArea() override;

	TSharedPtr<SVerticalBox> PinBox;
};
