// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IAssetTypeActions.h"
#include "Modules/ModuleManager.h"

class FGraphChaRtNodeFactory;
class FToolBarBuilder;
class FMenuBuilder;

class FGraphChaRtEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	TSharedPtr<FGraphChaRtNodeFactory> PathEdGraphNodeFactory;
	
	TSharedPtr<class FUICommandList> PluginCommands;

	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;

};
