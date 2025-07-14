
#include "GraphEditor/EdGraph/PathGraph/PathEdGraphSchema.h"

#include "SAdvancedTransformInputBox.h"
#include "GraphEditor/EdGraph/PathGraph/PathEdGraph.h"



UPathEdGraphSchema::UPathEdGraphSchema(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPathEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	Super::GetGraphContextActions(ContextMenuBuilder);

	if (!ContextMenuBuilder.CurrentGraph) return;

	if (ContextMenuBuilder.FromPin == nullptr)
	{
		using TPathEdGraphScTPathEdGraphSchemaAction = TPathEdGraphSchemaAction<UPathEdGraphNode>;
		TSharedPtr<TPathEdGraphScTPathEdGraphSchemaAction> NewSchemaAction = MakeShared<TPathEdGraphScTPathEdGraphSchemaAction>(
			FText::FromString("Path EdGraph Nodes"),
			FText::FromString("Add Path EdGraph Node"),
			FText::FromString("Add a new Path EdGraph Node."),
			0
			);

		NewSchemaAction->Action = [](UEdGraph* Graph, UEdGraphPin* Pin, const FVector2D& Location) -> UPathEdGraphNode*
		{
			return UPathEdGraphSchema::AddPathEdGraphNodeToGraph(Cast<UPathEdGraph>(Graph), Location);
		};

		ContextMenuBuilder.AddAction(NewSchemaAction);
	}
}

UPathEdGraphNode* UPathEdGraphSchema::AddPathEdGraphNodeToGraph(UPathEdGraph* Graph, const FVector2D& Position)
{
	if (!Graph) return nullptr;

	const FScopedTransaction Transaction(NSLOCTEXT("PathEdGraphSchema", "AddPathEdGraphNodeToGraph", "AddPathEdGraphSchema"));
	Graph->Modify();

	UPathEdGraphNode* NewNode = NewObject<UPathEdGraphNode>(Graph);
	Graph->AddNode(NewNode, true, false);

	NewNode->NodePosX = Position.X;
	NewNode->NodePosY = Position.Y;

	NewNode->AllocateDefaultPins();

	NewNode->AutowireNewNode(nullptr);

	return NewNode;
}
