

#include "ChaRtEditor/PathGraph/Slate/SPathGraphCheckResult.h"

void SPathGraphCheckResult::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SAssignNew(MessageListView, SListView<TSharedPtr<FGraphCheckMessage>>)
		.ListItemsSource(&Messages)
		.OnGenerateRow(this, &SPathGraphCheckResult::OnGenerateRow)
	];
}

void SPathGraphCheckResult::SetMessages(const TArray<FGraphCheckMessage>& InMessages)
{
	Messages.Empty();
	for (const FGraphCheckMessage& Msg : InMessages)
	{
		Messages.Add(MakeShared<FGraphCheckMessage>(Msg));
	}
	MessageListView->RequestListRefresh();
}

TSharedRef<ITableRow> SPathGraphCheckResult::OnGenerateRow(TSharedPtr<FGraphCheckMessage> Item,
	const TSharedRef<STableViewBase>& Owner)
{
	FSlateColor Color;
	switch (Item->Type)
	{
	case EGraphMessageType::None: break;
	case EGraphMessageType::Info:    Color = FSlateColor(FLinearColor::White); break;
	case EGraphMessageType::Warning: Color = FSlateColor(FLinearColor::Yellow); break;
	case EGraphMessageType::Error:   Color = FSlateColor(FLinearColor::Red); break;
	}

	return SNew(STableRow<TSharedPtr<FGraphCheckMessage>>, Owner)
	[
		SNew(SBox)
		.HeightOverride(28.f)
		.Padding(FMargin(4))
		[
			SNew(STextBlock)
			.Text(Item->Message)
			.ColorAndOpacity(Color)
		]
	];
}
