#pragma once

class UPathGraph;

class SPathGraphEditor : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPathGraphEditor) {}
		SLATE_ARGUMENT(TWeakObjectPtr<UPathGraph>, EditedPathGraph);
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	TSharedRef<SWidget> CreateToolBar();
	TSharedRef<SGraphEditor> CreateGraphEditor();

	
	FText GetStatusText() const;

	void BindCommands();
	
	void AddNewNode();
	void SelectAllNodes();
	void DeleteSelectedNodes();
	void CopySelectedNodes();
	void PasteNodes();

	void OnNodeSelected(const FGraphPanelSelectionSet& Element);
	void OnNodesPasted(const TSet<UObject*>& PastedNodes);
	
private:
	
	TWeakObjectPtr<UPathGraph> EditedPathGraph;

	TSharedPtr<SGraphEditor> GraphEditor;
	TSharedPtr<FUICommandList> CommandList;
	
};
