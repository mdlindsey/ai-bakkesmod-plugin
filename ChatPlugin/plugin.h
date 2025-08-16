#pragma once
#pragma warning(disable:4996)
// Above line prevents compilation error:
// 'PlayerReplicationInfoWrapper::GetUniqueId': Use GetUniqueIdWrapper instead

#include <string>
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

class ChatPlugin: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow
{
	// Inherited via PluginSettingsWindow
	string GetPluginName() override;
	void RenderSettings() override;
	void SetImGuiContext(uintptr_t ctx) override;

	// Plugin manager hooks
	virtual void onLoad();
	virtual void onUnload();

	// Game event hooks
	void onFocusChange(ServerWrapper caller, void* params, string eventName);
	void onChatMessage(ServerWrapper caller, void* params, string eventName);

	// LLM Functionality
	void handleChatMessage(string message, string author);
};
