
#include "GraphEditor/Slate/PathGraph/SPathGraphPin.h"



void SPathGraphPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

TSharedRef<SWidget> SPathGraphPin::GetDefaultValueWidget()
{
	if (GetPinObj() && GetPinObj()->PinType.PinCategory == "exec")
	{
		return SNullWidget::NullWidget;
	}
    
	return SGraphPin::GetDefaultValueWidget();
}

FSlateColor SPathGraphPin::GetPinColor() const
{
	if (GetPinObj() && GetPinObj()->PinType.PinCategory == "exec")
	{
		return FLinearColor::White;
	}
	else if (GetPinObj() && GetPinObj()->PinType.PinCategory == "exec")
	{
		return FLinearColor::Green;
	}
	else
	{
		return FLinearColor::Blue;
	}
}