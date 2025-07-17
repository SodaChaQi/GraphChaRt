
#include "ChaRtEditor/PathGraph/EdGraph/PathEdGraphSchema.h"

#include "ChaRtEditor/PathGraph/EdGraph/PathEdGraph.h"


void FPathEdGraphConnectionDrawingPolicy::Draw(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries,
	FArrangedChildren& ArrangedNodes)
{
	FConnectionDrawingPolicy::Draw(InPinGeometries, ArrangedNodes);

	UE_LOG(LogTemp, Warning, TEXT("InPinGeometries Num: %d"), InPinGeometries.Num());
	// PinGeometries = &InPinGeometries;
	//
	// BuildPinToPinWidgetMap(InPinGeometries);
	// DrawPinGeometries(InPinGeometries, ArrangedNodes);
}

void FPathEdGraphConnectionDrawingPolicy::DrawPinGeometries(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries,
	FArrangedChildren& ArrangedNodes)
{
	UE_LOG(LogTemp, Warning, TEXT("PinToPinWidgetMap Num: %d"), PinToPinWidgetMap.Num());
	for (const auto& Pair : PinToPinWidgetMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("1"));
		UEdGraphPin* OutputPin = Pair.Key; // ← 通常你的 Pin 映射来自这里
		if (!OutputPin || OutputPin->Direction != EGPD_Output)
			continue;

		UE_LOG(LogTemp, Warning, TEXT("2"));

		for (UEdGraphPin* LinkedPin : OutputPin->LinkedTo)
		{
			if (!LinkedPin) continue;

			UE_LOG(LogTemp, Warning, TEXT("3"));

			// 获取目标 pin 的几何
			TSharedRef<SWidget> TargetWidget = PinToPinWidgetMap.FindChecked(LinkedPin).ToSharedRef();
			const FArrangedWidget* StartGeom = InPinGeometries.Find(Pair.Value.ToSharedRef());
			const FArrangedWidget* EndGeom   = InPinGeometries.Find(TargetWidget);

			if (!StartGeom || !EndGeom) continue;

			const FVector2D StartAbs = StartGeom->Geometry.LocalToAbsolute(FGeometryHelper::CenterOf(StartGeom->Geometry));
			const FVector2D EndAbs = EndGeom->Geometry.LocalToAbsolute(FGeometryHelper::CenterOf(EndGeom->Geometry));

			FConnectionParams Params;
			DetermineWiringStyle(OutputPin, LinkedPin, Params);

			UE_LOG(LogTemp, Warning, TEXT("Start: (%.1f, %.1f)"), StartAbs.X, StartAbs.Y);
			UE_LOG(LogTemp, Warning, TEXT("End: (%.1f, %.1f)"), EndAbs.X, EndAbs.Y);
			// 传递绝对坐标而非局部几何体
			DrawSplineWithArrow(StartGeom->Geometry, EndGeom->Geometry, Params);
		}
	}
}

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

void FPathEdGraphConnectionDrawingPolicy::DrawSplineWithArrow(const FVector2D& StartPoint, const FVector2D& EndPoint,
	const FConnectionParams& Params)
{
	// 1. 直接使用绝对坐标点
	TArray<FVector2D> LinePoints;
	LinePoints.Add(StartPoint);
	LinePoints.Add(EndPoint);
	
	// 2. 使用正确的构造函数创建线段几何
	FPaintGeometry LinePaintGeometry = FPaintGeometry(
		FVector2D::ZeroVector,   // DrawPosition
		FVector2D(1, 1),         // DrawSize
		ZoomFactor               // DrawScale
	);
	
	// 3. 绘制线段（修复裁剪）
	FSlateDrawElement::MakeLines(
		DrawElementsList,
		WireLayerID,
		LinePaintGeometry,
		LinePoints,
		ESlateDrawEffect::None,
		Params.WireColor,
		true,
		Params.WireThickness
	);
	
	// 4. 计算箭头方向
	const FVector2D LineDelta = (EndPoint - StartPoint).GetSafeNormal();
	const float ArrowSize = 8.f * ZoomFactor; // 关键修复：考虑缩放因子
	
	// 5. 计算箭头位置（修复缩放）
	const FVector2D ArrowCenter = EndPoint - LineDelta * ArrowSize;
	
	// 6. 箭头旋转角度
	const float Angle = FMath::Atan2(LineDelta.Y, LineDelta.X);
	
	// 7. 获取箭头资源
	const FSlateBrush* ArrowBrush = FAppStyle::GetBrush("Graph.Arrow");
	
	// 8. 使用正确的构造函数创建箭头几何
	FPaintGeometry ArrowPaintGeometry = FPaintGeometry(
		ArrowCenter,                         // DrawPosition
		ArrowBrush->ImageSize * ZoomFactor,  // DrawSize
		ZoomFactor                          // DrawScale
	);
	
	// 9. 绘制箭头
	FSlateDrawElement::MakeRotatedBox(
		DrawElementsList,
		ArrowLayerID,
		ArrowPaintGeometry,
		ArrowBrush,
		ESlateDrawEffect::None,
		Angle,
		TOptional<FVector2D>(),
		FSlateDrawElement::RelativeToElement,
		Params.WireColor
	);

	// // 调试日志
	// UE_LOG(LogTemp, Warning, TEXT("DrawSplineWithArrow: From (%.1f, %.1f) to (%.1f, %.1f)"), 
	// 	StartPoint.X, StartPoint.Y, EndPoint.X, EndPoint.Y);
 //    
	// // 1. 创建绘制参数
	// const FLinearColor LineColor = FLinearColor::Green; // 强制使用绿色
	// const float LineThickness = 5.0f; // 强制使用粗线
 //    
	// // 2. 使用更可靠的直线绘制方法
	// FSlateDrawElement::MakeLines(
	// 	DrawElementsList,
	// 	WireLayerID,
	// 	FPaintGeometry(), // 使用空几何体，依赖点坐标
	// 	TArray<FVector2D>({StartPoint, EndPoint}),
	// 	ESlateDrawEffect::None,
	// 	LineColor,
	// 	true, // 抗锯齿
	// 	LineThickness
	// );
 //    
	// // 3. 箭头绘制 - 简化版
	// const FVector2D Direction = (EndPoint - StartPoint).GetSafeNormal();
	// const FVector2D ArrowBase = EndPoint - Direction * 15.0f * ZoomFactor;
 //    
	// // 创建箭头三角形
	// TArray<FVector2D> ArrowPoints;
	// const FVector2D Perp = FVector2D(-Direction.Y, Direction.X) * 8.0f * ZoomFactor;
 //    
	// ArrowPoints.Add(EndPoint);
	// ArrowPoints.Add(ArrowBase + Perp);
	// ArrowPoints.Add(ArrowBase - Perp);
	// ArrowPoints.Add(EndPoint); // 闭合三角形
 //    
	// // 绘制实心箭头
	// FSlateDrawElement::MakeLines(
	// 	DrawElementsList,
	// 	ArrowLayerID,
	// 	FPaintGeometry(),
	// 	ArrowPoints,
	// 	ESlateDrawEffect::None,
	// 	LineColor,
	// 	true, // 抗锯齿
	// 	LineThickness
	// );
 //    
	// // 填充箭头
	// // FSlateDrawElement::MakeRotatedBox(
	// // 	DrawElementsList,
	// // 	ArrowLayerID,
	// // 	FPaintGeometry(),
	// // 	ArrowPoints,
	// // 	FSlateColor(LineColor),
	// // 	ESlateDrawEffect::None
	// // );
}

void FPathEdGraphConnectionDrawingPolicy::DetermineWiringStyle(UEdGraphPin* OutputPin, UEdGraphPin* InputPin,
                                                               FConnectionParams& Params)
{
	// 设置颜色、线宽、风格等
	Params.AssociatedPin1 = OutputPin;
	Params.AssociatedPin2 = InputPin;
	Params.WireThickness = 2.0f;
	Params.WireColor = FLinearColor::White;
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
