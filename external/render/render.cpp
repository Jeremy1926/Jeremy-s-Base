#pragma once

#include <core/features/visuals/visuals.hpp>
#include <dwmapi.h>
#include <thread>
#include <string>
#include <random>
#include <external/includes/includes.hpp>
#include <external/menu/menu.hpp>
#include <external/settings/settings.hpp>
#include <external/render/render.hpp>

ID3D11Device* D3DDevice;
ID3D11DeviceContext* D3DDeviceContext;
IDXGISwapChain* D3DSwapChain;
ID3D11RenderTargetView* D3DRenderTarget;
D3DPRESENT_PARAMETERS D3DPresentParams;

HWND WindowHandle;

auto SetWindowToTarget() -> void
{
    while (true)
    {
        SetWindowPos(bWindowHandle, WindowHandle, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}

auto Render::CreateOverlay() -> bool
{
    MARGINS Margin = { -1 };

    std::thread(((SetWindowToTarget))).detach();

    WNDCLASS WindowClass = { 0 };
    WindowClass.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hInstance = GetModuleHandle(NULL);
    WindowClass.lpfnWndProc = DefWindowProcA;
    std::wstring ClassName = L"Jeremy's Base";
    std::wstring WindowName = L"Jeremy's Base";
    WindowClass.lpszClassName = ClassName.c_str();
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClass(&WindowClass))
    {
        std::cout << (_("[Jeremy's Base] Overlay didnt init."));
    }

    WindowHandle = CreateWindow(ClassName.c_str(),
        WindowName.c_str(),
        WS_POPUP,
        0,
        0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),
        NULL,
        NULL,
        WindowClass.hInstance,
        NULL);


    ShowWindow(WindowHandle, SW_SHOW);
    DwmExtendFrameIntoClientArea(WindowHandle, &Margin);
    SetWindowLong(WindowHandle, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
    UpdateWindow(WindowHandle);

    DXGI_SWAP_CHAIN_DESC SwapChainDescription;
    ZeroMemory(&SwapChainDescription, sizeof(SwapChainDescription));
    SwapChainDescription.BufferCount = 2;
    SwapChainDescription.BufferDesc.Width = 0;
    SwapChainDescription.BufferDesc.Height = 0;
    SwapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    SwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDescription.OutputWindow = WindowHandle;
    SwapChainDescription.SampleDesc.Count = 1;
    SwapChainDescription.SampleDesc.Quality = 0;
    SwapChainDescription.Windowed = 1;
    SwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL d3d_feature_lvl;
    const D3D_FEATURE_LEVEL d3d_feature_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, d3d_feature_array, 2, D3D11_SDK_VERSION,
        &SwapChainDescription, &D3DSwapChain, &D3DDevice, &d3d_feature_lvl, &D3DDeviceContext);

    if (FAILED(hr))
        return false;

    ID3D11Texture2D* pBackBuffer;
    hr = D3DSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (FAILED(hr))
        return false;

    hr = D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &D3DRenderTarget);
    pBackBuffer->Release();
    if (FAILED(hr))
        return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    io.Fonts->AddFontFromFileTTF(_("C:\\Windows\\Fonts\\calibrib.ttf"), 13.0f);

    if (!ImGui_ImplWin32_Init(WindowHandle) || !ImGui_ImplDX11_Init(D3DDevice, D3DDeviceContext))
        return false;

    return true;
}

auto Render::RenderThread() -> void
{
    ImVec4 vClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    const float fClearColor[4] = { vClearColor.x * vClearColor.w, vClearColor.y * vClearColor.w, vClearColor.z * vClearColor.w, vClearColor.w };
    auto& io = ImGui::GetIO();

    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

    uintptr_t pOffset = 0x0;

    for (;; )
    {
        MSG msg;
        while (PeekMessage(&msg, WindowHandle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                return;
        }

        io.DeltaTime = 1.0f / 165.f;
        io.ImeWindowHandle = WindowHandle;

        POINT p_cursor;
        GetCursorPos(&p_cursor);
        io.MousePos.x = p_cursor.x;
        io.MousePos.y = p_cursor.y;

        io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        io.MouseClicked[0] = io.MouseDown[0];

        io.WantCaptureMouse = io.WantCaptureKeyboard = io.WantCaptureMouse || io.WantCaptureKeyboard;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();
        {
            Visuals_.LoopPlayers();
            Menu_.RenderMenu();

            if (jtx::RenderFOV)
            {
                ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Width / 2, Height / 2), jtx::AimFOV, ImColor(255, 255, 255), 200, 1.0f);
            }
        }

        ImGui::Render();
        D3DDeviceContext->OMSetRenderTargets(1, &D3DRenderTarget, nullptr);
        D3DDeviceContext->ClearRenderTargetView(D3DRenderTarget, fClearColor);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        D3DSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    if (D3DRenderTarget)
        D3DRenderTarget->Release();

    if (D3DSwapChain)
        D3DSwapChain->Release();

    if (D3DDeviceContext)
        D3DDeviceContext->Release();

    DestroyWindow(WindowHandle);
}