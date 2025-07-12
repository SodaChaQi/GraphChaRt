#pragma once


#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "PathGraphSchema.generated.h"



UCLASS()
class UPathGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:

	UPathGraphSchema(const FObjectInitializer& ObjectInitializer);
	
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override {}
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
	}
};