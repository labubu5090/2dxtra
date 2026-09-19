#include <mutex>
#include <MinHook.h>
#include "renderer_hook.h"
#include "../game.h"
#include "../gui/gui.h"
#include "../util/code_patch.h"

namespace iidxtra::renderer_hook
{
    std::once_flag init_once;

    void** original_vft = nullptr;
    std::unique_ptr<std::uintptr_t[]> replacement_vft;

    IDirect3DDevice9* device_ptr = nullptr;
    IDirect3DSwapChain9* swapchain_ptr = nullptr;

    HRESULT (*original_present_fn) (IDirect3DSwapChain9*, const RECT*, const RECT*, HWND, const RGNDATA*, DWORD) = nullptr;
    LRESULT (*original_wndproc_fn) (void*, HWND, UINT, WPARAM, LPARAM) = nullptr;

    auto present_hook_fn(IDirect3DSwapChain9* swapchain, const RECT* src, const RECT* dst, HWND dst_wnd, const RGNDATA* dirty_region, DWORD flags) -> HRESULT
    {
        std::call_once(init_once, gui::init);

        if (SUCCEEDED(device_ptr->BeginScene()))
        {
            gui::begin();
            gui::render();
            gui::end();

            device_ptr->EndScene();
        }

        return original_present_fn(swapchain, src, dst, dst_wnd, dirty_region, flags);
    }

    auto wndproc_hook_fn(void* a1, HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) -> LRESULT
    {
        auto result = original_wndproc_fn(a1, hwnd, msg, wparam, lparam);
        gui::wndproc(hwnd, msg, wparam, lparam);
        return result;
    }

	auto set_renderer_freeze(bool frozen) -> void
	{
		if (bm2dx::addr->RENDERER_PATCH == nullptr)
			return;

		auto static patch = util::code_patch { bm2dx::addr->RENDERER_PATCH,
			{ 0x90, 0x90, 0x90, 0x90, 0x90 } };

		frozen ? patch.enable(): patch.disable();

		Sleep(25);
	}

    // __try needs a leaf function with only POD locals: any destructor-bearing
    // temporary (e.g. unique_ptr) inside the block forces object unwinding and
    // trips C2712, so the raw vtable pointer is handed back to install_hook.
    static auto armed_install() -> std::uintptr_t*
    {
        __try
        {
            // The D3D9_DEVICE cell may be an unverified candidate on some
            // builds; validate it (and the swapchain vtable) before touching
            // anything the game depends on.
            auto const cell = reinterpret_cast<IDirect3DDevice9* const*>(bm2dx::addr->D3D9_DEVICE);

            auto mbi = MEMORY_BASIC_INFORMATION {};
            if (!cell || VirtualQuery(cell, &mbi, sizeof mbi) == 0 || mbi.State != MEM_COMMIT)
                return nullptr;

            device_ptr = *cell;
            if (!device_ptr || device_ptr->GetSwapChain(0, &swapchain_ptr) != D3D_OK)
                return nullptr;

            auto* const old_vft = *reinterpret_cast<void***>(swapchain_ptr);
            auto count = 0;

            while (old_vft && old_vft[count])
                ++count;

            if (count < 4)
                return nullptr;

            auto* const new_vft = new std::uintptr_t[count];
            std::memcpy(new_vft, old_vft, count * sizeof(void*));

            new_vft[3] = std::uintptr_t(present_hook_fn);
            original_present_fn = reinterpret_cast<decltype(original_present_fn)>(old_vft[3]);
            original_vft = old_vft;

            *reinterpret_cast<void**>(swapchain_ptr) = new_vft;

            // Hook WndProc to capture keyboard/mouse input.
            MH_CreateHook(bm2dx::addr->WNDPROC_FN, reinterpret_cast<LPVOID>(wndproc_hook_fn), (void**) &original_wndproc_fn);
            MH_EnableHook(bm2dx::addr->WNDPROC_FN);

            return new_vft;
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            return nullptr;
        }
    }

    auto install_hook() -> bool
    {
        // Already hooked (e.g. mdata_load and boot thread both try this).
        if (replacement_vft)
            return true;

		#ifndef NDEBUG
			set_renderer_freeze(true);
        #endif

        auto vft = std::unique_ptr<std::uintptr_t[]>(armed_install());
        if (!vft)
            return false;

        replacement_vft = std::move(vft);

		#ifndef NDEBUG
			set_renderer_freeze(false);
		#endif

        return true;
    }

    auto uninstall_hook() -> void
    {
		// Write the original virtual table pointer back.
		set_renderer_freeze(true);
        *reinterpret_cast<void**>(swapchain_ptr) = original_vft;
		set_renderer_freeze(false);
    }
}