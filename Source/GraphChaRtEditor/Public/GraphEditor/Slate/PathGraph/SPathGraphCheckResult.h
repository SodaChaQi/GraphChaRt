#pragma once

enum class EGraphMessageType : uint8
{
	None,
	Info,
	Warning,
	Error
};

struct FGraphCheckMessage
{
	EGraphMessageType Type;
	FText Message;

	FGraphCheckMessage() : Type(EGraphMessageType::None) {}

	FGraphCheckMessage(EGraphMessageType Type, const FText& Message) : Type(Type), Message(Message) {}
};


class SPathGraphCheckResult : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPathGraphCheckResult) {}
		
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetMessages(const TArray<FGraphCheckMessage>& InMessages);

private:

	TSharedPtr<SListView<TSharedPtr<FGraphCheckMessage>>> MessageListView;
	TArray<TSharedPtr<FGraphCheckMessage>> Messages;

	TSharedRef<ITableRow> OnGenerateRow(TSharedPtr<FGraphCheckMessage> Item, const TSharedRef<STableViewBase>& Owner);
};
