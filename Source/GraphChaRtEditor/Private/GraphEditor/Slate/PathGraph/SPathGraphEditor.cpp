
#include "GraphEditor/Slate/PathGraph/SPathGraphEditor.h"

#include "Graph/Graphs.h"
#include "GraphEditor/PathGraphSchema.h"
#include "Framework/Commands/GenericCommands.h"
#include "EditorCommands/PathGraphEditorCommands.h"




#define LOCTEXT_NAMESPACE "PathGraphEditor"

void SPathGraphEditor::Construct(const FArguments& InArgs)
{
	EditedPathGraph = InArgs._EditedPathGraph;

	BindCommands();

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight()
		[
			CreateToolBar()
		]
		+ SVerticalBox::Slot().FillHeight(1.0f)
		[
			CreateGraphEditor()
		]
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SBorder)
			.Padding(4)
			[
				SNew(STextBlock)
				.Text(this, &SPathGraphEditor::GetStatusText)
			]
		]
	];
}

TSharedRef<SWidget> SPathGraphEditor::CreateToolBar()
{
	FToolBarBuilder ToolBarBuilder(CommandList, FMultiBoxCustomization::None);

	ToolBarBuilder.AddToolBarButton(
		FUIAction(FExecuteAction::CreateSP(this, &SPathGraphEditor::SelectAllNodes)),
		NAME_None,
		LOCTEXT("SelectAll", "Select All"),
		LOCTEXT("SelectAllTooltip", "Select All Nodes"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "GenericCommands.SelectAll")
		);

	ToolBarBuilder.AddSeparator();

	ToolBarBuilder.AddToolBarButton(
	FUIAction(FExecuteAction::CreateSP(this, &SPathGraphEditor::DeleteSelectedNodes)),
	NAME_None,
	LOCTEXT("Delete", "Delete"),
	LOCTEXT("DeleteTooltip", "Delete selected nodes"),
	FSlateIcon(FAppStyle::GetAppStyleSetName(), "GenericCommands.Delete")
		);
    
	ToolBarBuilder.AddToolBarButton(
		FUIAction(FExecuteAction::CreateSP(this, &SPathGraphEditor::CopySelectedNodes)),
		NAME_None,
		LOCTEXT("Copy", "Copy"),
		LOCTEXT("CopyTooltip", "Copy selected nodes"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "GenericCommands.Copy")
		);
    
	ToolBarBuilder.AddToolBarButton(
		FUIAction(FExecuteAction::CreateSP(this, &SPathGraphEditor::PasteNodes)),
		NAME_None,
		LOCTEXT("Paste", "Paste"),
		LOCTEXT("PasteTooltip", "Paste nodes"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "GenericCommands.Paste")
		);

	return ToolBarBuilder.MakeWidget();
}

TSharedRef<SGraphEditor> SPathGraphEditor::CreateGraphEditor()
{
	FGraphAppearanceInfo GraphAppearanceInfo;
	GraphAppearanceInfo.CornerText = LOCTEXT("PathGraphCornerText", "PATH GRAPH");

	SGraphEditor::FGraphEditorEvents GraphEditorEvents;
	GraphEditorEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this,
		&SPathGraphEditor::OnNodeSelected);
	// GraphEditorEvents.OnNodeSpawnedByKeymap = SGraphEditor::FOnNodePasted::CreateSP(this,)

	if (!EditedPathGraph.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SPathGraphEditor: EditedPathGraph is invalid!"));
		// 返回一个空控件，避免崩溃
		return SNew(SGraphEditor);
	}

	UEdGraph* EdGraphToUse = EditedPathGraph->GetEdGraph();
	if (!EdGraphToUse)
	{
		EdGraphToUse = NewObject<UEdGraph>(EditedPathGraph.Get(), UEdGraph::StaticClass(), NAME_None, RF_Transactional);
		EdGraphToUse->Schema = UPathGraphSchema::StaticClass(); // 可选
		EditedPathGraph->EdGraph = EdGraphToUse;
	}

	return SAssignNew(GraphEditor, SGraphEditor)
		.AdditionalCommands(CommandList)
		.Appearance(GraphAppearanceInfo)
		.GraphToEdit(EdGraphToUse)
		.GraphEvents(GraphEditorEvents);
}

FText SPathGraphEditor::GetStatusText() const
{
	if (GraphEditor.IsValid())
	{
		const FGraphPanelSelectionSet& SelectedNodes = GraphEditor->GetSelectedNodes();
		return FText::Format(LOCTEXT("StatusFormat", "Selected: {0} | Total: {1}"), 
			FText::AsNumber(SelectedNodes.Num()), 
			FText::AsNumber(GraphEditor->GetNumberOfSelectedNodes()));
	}
	return FText::GetEmpty();
}

void SPathGraphEditor::BindCommands()
{
	CommandList = MakeShareable(new FUICommandList);
    
	// 标准命令
	CommandList->MapAction(
		FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateSP(this, &SPathGraphEditor::SelectAllNodes)
	);
    
	CommandList->MapAction(
		FGenericCommands::Get().Delete,
		FExecuteAction::CreateSP(this, &SPathGraphEditor::DeleteSelectedNodes)
	);
    
	CommandList->MapAction(
		FGenericCommands::Get().Copy,
		FExecuteAction::CreateSP(this, &SPathGraphEditor::CopySelectedNodes)
	);
    
	CommandList->MapAction(
		FGenericCommands::Get().Paste,
		FExecuteAction::CreateSP(this, &SPathGraphEditor::PasteNodes)
	);
    
	// 自定义命令
	CommandList->MapAction(
		FPathGraphEditorCommands::Get().AddNode,
		FExecuteAction::CreateSP(this, &SPathGraphEditor::AddNewNode)
	);
}

void SPathGraphEditor::AddNewNode()
{
}

void SPathGraphEditor::SelectAllNodes()
{
}

void SPathGraphEditor::DeleteSelectedNodes()
{
}

void SPathGraphEditor::CopySelectedNodes()
{
}

void SPathGraphEditor::PasteNodes()
{
}

void SPathGraphEditor::OnNodeSelected(const FGraphPanelSelectionSet& Element)
{
	// 处理节点选择事件
	// 可以更新细节面板等
}

void SPathGraphEditor::OnNodesPasted(const TSet<UObject*>& PastedNodes)
{

}

#undef LOCTEXT_NAMESPACE
