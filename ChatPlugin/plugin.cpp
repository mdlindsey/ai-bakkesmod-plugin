#include "pch.h"
#include "cvar.h"
#include "plugin.h"
#include "global.h"
#include "logging.h"
#include "version.h"
#include <imgui_stdlib.h> // ImGui::InputText

using namespace std;

const char * PluginName = "AI Chat"; // To change DLL filename use <TargetName> in *.vcxproj
constexpr auto PluginVersion = stringify(V_MAJOR) "." stringify(V_MINOR) "." stringify(V_PATCH) "." stringify(V_BUILD);
BAKKESMOD_PLUGIN(ChatPlugin, PluginName, PluginVersion, PLUGINTYPE_FREEPLAY);

void ChatPlugin::onLoad()
{
	Global::GameWrapper = gameWrapper;
	Global::CvarManager = cvarManager;

	Log::SetPrintLevel(Log::Level::Info);
	Log::SetWriteLevel(Log::Level::Info);
	Log::Info("AI Chat plugin loaded");

	Cvar::Create("ai_enabled", true);
	Cvar::Create("ai_provider", "openai");
    Cvar::Create("ai_model", "gpt-5-mini-2025-08-07");
    Cvar::Create("ai_api_key", "");
    Cvar::Create("ai_system_prompt", "You are a washed Rocket League player who always replies in 100 chars or less using sPoNgEbOb cAsInG.");
	
	// List of commonly used hooks can be found here:
	// https://wiki.ChatPlugins.com/functions/commonly_hooked_functions/
	map<string, void (ChatPlugin::*)(ServerWrapper c, void *p, string e)> ListenerMap =
	{
		{ "Function ProjectX.GameViewportClient_X.EventFocusChanged", &ChatPlugin::onFocusChange },
	};
	for (auto const& [eventName, eventListener] : ListenerMap)
	{
		gameWrapper->HookEventWithCaller<ServerWrapper>(eventName, bind(
			eventListener, this, placeholders::_1, placeholders::_2, placeholders::_3
		));
	}
}

void ChatPlugin::onUnload()
{
	// Clean up all cvars to prevent memory leaks
	Cvar::CleanupAll();
	Log::Info("AI chat plugin unloaded");
}

// When the plugin is loaded (manually or after build) these settings
// will show in BakkesMod (F2 while in-game) under Plugins > ChatPlugin
void ChatPlugin::RenderSettings()
{
	// Obviously you can delete this spam
	ImGui::Spacing();
	
	// Rendering settings for cvars is quick and easy...
	Cvar::Get("ai_enabled")->RenderCheckbox("Enabled");
	ImGui::Spacing();
	Cvar::Get("ai_api_key")->RenderLargeInput("OpenAI API Key ", 128);
	ImGui::Spacing();
	Cvar::Get("ai_system_prompt")->RenderTextArea("System Prompt", 512);
	
	if (ImGui::Button("Send Message"))
	{
		Log::Info("Sending message");
		this->handleChatMessage("did you see that reset?", "Dan");
	}
	
	// Padding in case things get close to the bottom and become difficult to read...
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();
}
// More settings rendering boilerplate
string ChatPlugin::GetPluginName() { return string(PluginName); }
void ChatPlugin::SetImGuiContext(uintptr_t ctx) { ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx)); }

// Event listeners
void ChatPlugin::onFocusChange(ServerWrapper caller, void* params, string eventName)
{
	bool focus = *reinterpret_cast<bool*>(params);
	Log::Info("RL window is " + string(focus ? "focused" : "blurred"));
}

// Chat Functionality
struct ChatMessageParams
{
    void* InPRI;
    uint8_t Message[0x10];
    uint8_t ChatChannel; // see EChatChannel
    bool bPreset; // i don't use this, maybe it's whether it's a quickchat or custom chat?
};
enum class EChatChannel : uint8_t
{
    EChatChannel_Match = 0,
    EChatChannel_Team = 1,
    EChatChannel_Party = 2,
    EChatChannel_Individual = 3,
    EChatChannel_END = 4
};
void ChatPlugin::onChatMessage(ServerWrapper caller, void* params, string eventName)
{
	Log::Info("CatFacts::onChatMessage");
	if (!params) return;
    ChatMessageParams* chatParams = reinterpret_cast<ChatMessageParams*>(params);
	PriWrapper pri(reinterpret_cast<uintptr_t>(chatParams->InPRI));
	UnrealStringWrapper message = UnrealStringWrapper(reinterpret_cast<uintptr_t>(&chatParams->Message));
    cvarManager->log("Author: " + pri.GetPlayerName().ToString());
    cvarManager->log("Message: " + message.ToString());
}

// LLM Functionality
void ChatPlugin::handleChatMessage(string message, string author)
{
    cvarManager->log("[CHAT] " + author + ": " + message);
	// CurlRequest req; req.url = url;
	// HttpWrapper::SendCurlRequest(req, [callback](int code, string result) { callback(result); });
}