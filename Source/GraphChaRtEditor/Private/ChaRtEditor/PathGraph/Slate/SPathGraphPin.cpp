
#include "ChaRtEditor/PathGraph/Slate/SPathGraphPin.h"



void SPathGraphPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	this->SetCursor(EMouseCursor::Default);
	bShowLabel = true;

	SGraphPin::Construct(SGraphPin::FArguments(), InPin);
}

TSharedRef<SWidget> SPathGraphPin::GetDefaultValueWidget()
{
	// if (GetPinObj() && GetPinObj()->PinType.PinCategory == "exec")
	// {
	// 	return SNullWidget::NullWidget;
	// }
    
	return SNew(STextBlock).Text(FText::GetEmpty());
}

const FSlateBrush* SPathGraphPin::GetPinIcon() const
{
	return FAppStyle::GetBrush("Graph.Pin.Connected");
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
