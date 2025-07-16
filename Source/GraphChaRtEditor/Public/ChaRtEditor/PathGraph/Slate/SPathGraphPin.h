#pragma once


#include "SGraphPin.h"


class SPathGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SPathGraphPin){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin);

	virtual TSharedRef<SWidget> GetDefaultValueWidget() override;

	virtual const FSlateBrush* GetPinIcon() const override;
	virtual FSlateColor GetPinColor() const override;
};