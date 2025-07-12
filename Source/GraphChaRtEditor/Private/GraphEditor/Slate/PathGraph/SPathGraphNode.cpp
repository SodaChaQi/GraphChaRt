
#include "GraphEditor/Slate/PathGraph/SPathGraphNode.h"


void SPathGraphNode::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	// SGraphNode::Construct(SGraphNode::FArguments(), InNode);
}

void SPathGraphNode::UpdateGraphNode()
{
	// 清除默认内容
	ContentScale.Bind(this, &SGraphNode::GetContentScale);
	GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Graph.Node.Body"))
			.Padding(10)
			[
				CreateNodeContentArea()
			]
		];
}

TSharedRef<SWidget> SPathGraphNode::CreateNodeContentArea()
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(this, &SPathGraphNode::GetEditableNodeTitleAsText)
			.TextStyle(FAppStyle::Get(), "Graph.Node.NodeTitle")
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0, 5, 0, 0)
		[
			SAssignNew(PinBox, SVerticalBox)
		];
}

void SPathGraphNode::CreatePinWidgets()
{
	PinBox->ClearChildren();
    
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

TSharedPtr<SGraphPin> SPathGraphNode::CreatePinWidget(UEdGraphPin* Pin) const
{
	return SNew(SPathGraphPin, Pin);
}

void SPathGraphNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	if (PinBox.IsValid())
	{
		PinBox->AddSlot()
		.AutoHeight()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(5, 2)
		[
			PinToAdd
		];
	}
	else
	{
		// 处理错误情况或回退到基类实现
		SGraphNode::AddPin(PinToAdd);
	}
}
