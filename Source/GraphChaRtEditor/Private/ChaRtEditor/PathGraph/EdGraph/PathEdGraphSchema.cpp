
#include "ChaRtEditor/PathGraph/EdGraph/PathEdGraphSchema.h"

#include "ChaRtEditor/PathGraph/EdGraph/PathEdGraph.h"


void FPathEdGraphConnectionDrawingPolicy::DrawSplineWithArrow(const FGeometry& StartGeom, const FGeometry& EndGeom,
                                                              const FConnectionParams& Params)
{
	// 1. 计算起点与终点中心
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter   = FGeometryHelper::CenterOf(EndGeom);

	// 2. 构造一条从 Start → End 的直线
	TArray<FVector2D> LinePoints;
	LinePoints.Add(StartCenter);
	LinePoints.Add(EndCenter);

	// 3. 绘制线段（在 BackLayer）
	FSlateDrawElement::MakeLines(
		DrawElementsList,
		WireLayerID,
		FPaintGeometry(),
		LinePoints,
		ESlateDrawEffect::None,
		Params.WireColor,
		true,
		Params.WireThickness
	);

	// 4. 箭头方向（End - Start）
	const FVector2D LineDelta = (EndCenter - StartCenter).GetSafeNormal();

	// 5. 箭头尺寸、旋转
	const float ArrowSize = 8.f;
	const FVector2D ArrowCenter = EndCenter - LineDelta * ArrowSize * 0.5f;
	const float Angle = FMath::Atan2(LineDelta.Y, LineDelta.X);

	// 6. 取箭头资源（Slate 默认箭头）
	const FSlateBrush* ArrowBrush = FAppStyle::GetBrush("Graph.Arrow");

	// 7. 画箭头（在 FrontLayer）
	FSlateDrawElement::MakeRotatedBox(
		DrawElementsList,
		ArrowLayerID,
		FPaintGeometry(
			ArrowCenter,
			ArrowBrush->ImageSize * ZoomFactor,
			ZoomFactor
		),
		ArrowBrush,
		ESlateDrawEffect::None,
		Angle,
		TOptional<FVector2D>(),
		FSlateDrawElement::RelativeToElement,
		Params.WireColor
	);
}

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

FConnectionDrawingPolicy* UPathEdGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID,
	float InZoomFactor, const FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements,
	class UEdGraph* InGraphObj) const
{
	return new FPathEdGraphConnectionDrawingPolicy(
		InBackLayerID,
		InFrontLayerID,
		InZoomFactor,
		InClippingRect,
		InDrawElements
		);
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
