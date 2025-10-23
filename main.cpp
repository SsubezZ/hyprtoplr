#define WLR_USE_UNSTABLE

#include <unistd.h>

#include <hyprland/src/includes.hpp>
#include <sstream>

#define private public
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/desktop/Window.hpp>
#include <hyprland/src/config/ConfigManager.hpp>
#include <hyprland/src/render/Renderer.hpp>
#include <hyprland/src/managers/KeybindManager.hpp>
#undef private

#include <hyprutils/string/VarList.hpp>
using namespace Hyprutils::String;

#include "globals.hpp"

// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

//

static bool            g_forceTopLayersVisible = false;

static SDispatchResult showTopLayers(std::string) {
    if (!g_pCompositor->m_lastMonitor || !g_pCompositor->m_lastMonitor->m_activeWorkspace) {
        return SDispatchResult{.success = false, .error = "No active monitor or workspace"};
    }

    auto monitor = g_pCompositor->m_lastMonitor.lock();
    if (!monitor) {
        return SDispatchResult{.success = false, .error = "Monitor no longer valid"};
    }

    g_forceTopLayersVisible = true;

    for (const auto& ls : monitor->m_layerSurfaceLayers[2]) {
        *ls->m_alpha = 1.f;
    }

    g_pHyprRenderer->damageMonitor(monitor);

    return SDispatchResult{};
}

static SDispatchResult hideTopLayers(std::string) {
    if (!g_pCompositor->m_lastMonitor || !g_pCompositor->m_lastMonitor->m_activeWorkspace) {
        return SDispatchResult{.success = false, .error = "No active monitor or workspace"};
    }

    auto monitor = g_pCompositor->m_lastMonitor.lock();
    if (!monitor) {
        return SDispatchResult{.success = false, .error = "Monitor no longer valid"};
    }

    g_forceTopLayersVisible = false;

    const auto pWorkspace = monitor->m_activeWorkspace;
    for (const auto& ls : monitor->m_layerSurfaceLayers[2]) {
        *ls->m_alpha = pWorkspace->m_hasFullscreenWindow && pWorkspace->m_fullscreenMode == FSMODE_FULLSCREEN ? 0.f : 1.f;
    }

    g_pHyprRenderer->damageMonitor(monitor);

    return SDispatchResult{};
}

static SDispatchResult toggleTopLayers(std::string) {
    if (!g_pCompositor->m_lastMonitor || !g_pCompositor->m_lastMonitor->m_activeWorkspace) {
        return SDispatchResult{.success = false, .error = "No active monitor or workspace"};
    }

    auto monitor = g_pCompositor->m_lastMonitor.lock();
    if (!monitor) {
        return SDispatchResult{.success = false, .error = "Monitor no longer valid"};
    }

    g_forceTopLayersVisible = !g_forceTopLayersVisible;

    PHLWORKSPACE pWorkspace = nullptr;

    if (monitor->m_activeSpecialWorkspace) {
        pWorkspace = monitor->m_activeSpecialWorkspace;
    } else {
        pWorkspace = monitor->m_activeWorkspace;
    }

    for (const auto& ls : monitor->m_layerSurfaceLayers[2]) {
        if (g_forceTopLayersVisible) {
            *ls->m_alpha = 1.f;
        } else {
            *ls->m_alpha = (pWorkspace->m_hasFullscreenWindow && pWorkspace->m_fullscreenMode == FSMODE_FULLSCREEN) ? 0.f : 1.f;
        }
    }

    g_pHyprRenderer->damageMonitor(monitor);
    return SDispatchResult{};
}

static SDispatchResult justToggleTopLayers(std::string) {
    if (!g_pCompositor->m_lastMonitor || !g_pCompositor->m_lastMonitor->m_activeWorkspace) {
        return SDispatchResult{.success = false, .error = "No active monitor or workspace"};
    }

    auto monitor = g_pCompositor->m_lastMonitor.lock();
    if (!monitor) {
        return SDispatchResult{.success = false, .error = "Monitor no longer valid"};
    }

    g_forceTopLayersVisible = !g_forceTopLayersVisible;

    for (const auto& ls : monitor->m_layerSurfaceLayers[2]) {
        *ls->m_alpha = g_forceTopLayersVisible ? 1.f : 0.f;
    }

    g_pHyprRenderer->damageMonitor(monitor);
    return SDispatchResult{};
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE = handle;

    const std::string COMPOSITOR_HASH = __hyprland_api_get_hash();
    const std::string CLIENT_HASH = __hyprland_api_get_client_hash();

    if (COMPOSITOR_HASH != CLIENT_HASH) {
        HyprlandAPI::addNotification(PHANDLE, "[hyprtoplr] Failure in initialization: Version mismatch (headers ver is not equal to running hyprland ver)",
                                     CHyprColor{1.0, 0.2, 0.2, 1.0}, 5000);
        throw std::runtime_error("[hyprtoplr] Version mismatch");
    }

    bool success = true;
    success      = success && HyprlandAPI::addDispatcherV2(PHANDLE, "plugin:hytop:show", ::showTopLayers);
    success      = success && HyprlandAPI::addDispatcherV2(PHANDLE, "plugin:hytop:hide", ::hideTopLayers);
    success      = success && HyprlandAPI::addDispatcherV2(PHANDLE, "plugin:hytop:toggle", ::toggleTopLayers);
    success      = success && HyprlandAPI::addDispatcherV2(PHANDLE, "plugin:hytop:justtoggle", ::justToggleTopLayers);

    if (success)
        HyprlandAPI::addNotification(PHANDLE, "[hyprtoplr] Initialized successfully!", CHyprColor{0.2, 1.0, 0.2, 1.0}, 5000);
    else {
        HyprlandAPI::addNotification(PHANDLE, "[hyprtoplr] Failure in initialization: failed to register dispatchers", CHyprColor{1.0, 0.2, 0.2, 1.0}, 5000);
        throw std::runtime_error("[hyprtoplr] Dispatchers failed");
    }

    return {"hyprtoplr", "A plugin that adds some top layer visibility dispatchers to Hyprland", " subez ", " 1.0 "};
}

APICALL EXPORT void PLUGIN_EXIT() {
    ;
}
