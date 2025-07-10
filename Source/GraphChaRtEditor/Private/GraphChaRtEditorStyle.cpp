// Copyright Epic Games, Inc. All Rights Reserved.

#include "GraphChaRtEditorStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FGraphChaRtEditorStyle::StyleInstance = nullptr;

void FGraphChaRtEditorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FGraphChaRtEditorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FGraphChaRtEditorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("GraphChaRtEditorStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FGraphChaRtEditorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("GraphChaRtEditorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("GraphChaRt")->GetBaseDir() / TEXT("Resources"));

	Style->Set("GraphChaRtEditor.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	Style->Set("ClassIcon.PathGraph", new IMAGE_BRUSH_SVG(TEXT("PathGraph"), Icon20x20));

	return Style;
}

void FGraphChaRtEditorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FGraphChaRtEditorStyle::Get()
{
	return *StyleInstance;
}
