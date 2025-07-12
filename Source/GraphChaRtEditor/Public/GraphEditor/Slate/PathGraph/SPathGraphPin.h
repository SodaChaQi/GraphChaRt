#pragma once

#include "SGraphPin.h"

class SPathGraphPin : public SGraphPin
{
public:
    SLATE_BEGIN_ARGS(SPathGraphPin) {}
    SLATE_END_ARGS()
    
    void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
    
protected:
    virtual TSharedRef<SWidget> GetDefaultValueWidget() override;
    virtual FSlateColor GetPinColor() const override;
};