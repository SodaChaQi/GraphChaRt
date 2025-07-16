
#include "ChaRtEditor/PathGraph/Slate/SPathGraphNode.h"

#include "SGraphPin.h"
#include "ChaRtEditor/PathGraph/EdGraph/PathEdGraphNode.h"
#include "ChaRtEditor/PathGraph/Slate/SPathGraphPin.h"
#include "Widgets/Layout/SConstraintCanvas.h"


void SPathGraphNode::Construct(const FArguments& InArgs, UPathEdGraphNode* InNode)
{
	GraphNode = Cast<UEdGraphNode>(InNode);

	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}

void SPathGraphNode::UpdateGraphNode()
{
	// 清除默认内容
	ContentScale.Bind(this, &SGraphNode::GetContentScale);
	GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
			.Padding(5.f)
			[
				CreateNodeContentArea()
			]
		];

	CreatePinWidgets();
}

TSharedPtr<SGraphPin> SPathGraphNode::CreatePinWidget(UEdGraphPin* Pin) const
{
	return SNew(SPathGraphPin, Pin);
}

TSharedRef<SWidget> SPathGraphNode::CreateNodeContentArea()
{
	// SAssignNew(LeftNodeBox, SVerticalBox);
	// SAssignNew(RightNodeBox, SVerticalBox);
	// SAssignNew(TopNodeBox, SHorizontalBox);
	// SAssignNew(BottomNodeBox, SHorizontalBox);
	//
	// return SNew(SOverlay)
	//
	// 	+ SOverlay::Slot()
	// 	[
	// 		SNew(SBorder)
	// 		.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
	// 		.Padding(10)
	// 		[
	// 			SNew(SVerticalBox)
	// 			+ SVerticalBox::Slot()
	// 			.AutoHeight()
	// 			[
	// 				SNew(STextBlock)
	// 				.Text(this, &SPathGraphNode::GetEditableNodeTitleAsText)
	// 			]
	// 			+ SVerticalBox::Slot()
	// 			.AutoHeight()
	// 			[
	// 				SNew(STextBlock).Text(FText::FromString("Node Body"))
	// 			]
	// 		]
	// 	]
	//
	// 	+SOverlay::Slot()
	// 	.HAlign(HAlign_Center)
	// 	.VAlign(VAlign_Top)
	// 	[
	// 		TopNodeBox.ToSharedRef()
	// 	]
	//
	// 	+SOverlay::Slot()
	// 	.HAlign(HAlign_Center)
	// 	.VAlign(VAlign_Bottom)
	// 	[
	// 		BottomNodeBox.ToSharedRef()
	// 	]
	//
	// 	// Left pins
	// 	+ SOverlay::Slot()
	// 		.HAlign(HAlign_Left)
	// 		.VAlign(VAlign_Center)
	// 	[
	// 		LeftNodeBox.ToSharedRef()
	// 	]
	//
	// 	// Right pins
	// 	+ SOverlay::Slot()
	// 		.HAlign(HAlign_Right)
	// 		.VAlign(VAlign_Center)
	// 	[
	// 		RightNodeBox.ToSharedRef()
	// 	];

	SAssignNew(Canvas, SConstraintCanvas);

	Canvas->AddSlot()
	.Anchors(FAnchors(.5f, .5f))
	.AutoSize(true)
	
	[
		SNew(STextBlock).Text(this, &SPathGraphNode::GetEditableNodeTitleAsText)
	];

	return Canvas.ToSharedRef();
}

void SPathGraphNode::CreatePinWidgets()
{
    
	// 为每个UEdGraphPin创建对应的Slate控件
	for (UEdGraphPin* Pin : GetNodeObj()->Pins)
	{
		if (Pin->Direction == EEdGraphPinDirection::EGPD_Input)
		{
			TSharedPtr<SGraphPin> NewPin = CreatePinWidget(Pin);
			AddPin(NewPin.ToSharedRef());
		}
	}
    
	// 处理输出引脚（如果需要）
	for (UEdGraphPin* Pin : GetNodeObj()->Pins)
	{
		if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			TSharedPtr<SGraphPin> NewPin = CreatePinWidget(Pin);
			AddPin(NewPin.ToSharedRef());
		}
	}
}

void SPathGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	// if (PinBox.IsValid())
	// {
	// 	PinBox->AddSlot()
	// 	.AutoHeight()
	// 	.HAlign(HAlign_Left)
	// 	.VAlign(VAlign_Center)
	// 	.Padding(5, 2)
	// 	[
	// 		PinToAdd
	// 	];
	// }
	// else
	// {
	// 	// 处理错误情况或回退到基类实现
	// 	SGraphNode::AddPin(PinToAdd);
	// }

	if (!Canvas.IsValid()) return;

	FVector2D InitPos = FVector2D(50, 50);

	Canvas->AddSlot()
	.Offset(FMargin(InitPos.X, InitPos.Y, 0, 0))
	.AutoSize(true)
	[
		PinToAdd
	];
}
