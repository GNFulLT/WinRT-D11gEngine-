#pragma once
#include "DirectXManager.h"

using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;


struct AppView : public winrt::implements<AppView, winrt::Windows::ApplicationModel::Core::IFrameworkView>
{
    public:
        // IFrameworkView methods.
        void Initialize(winrt::Windows::ApplicationModel::Core::CoreApplicationView const& applicationView);
        void SetWindow(winrt::Windows::UI::Core::CoreWindow const& window);
        void Load(winrt::hstring const& entryPoint);
        void Run();
        void Uninitialize();
         
    protected:
        void OnResuming(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& args);
        void OnClosed(winrt::Windows::UI::Core::CoreWindow const& sender, winrt::Windows::UI::Core::CoreWindowEventArgs const& args);
        void OnPointerPressed(IInspectable const&, PointerEventArgs const& args);

        void OnActivated(CoreApplicationView const& /* applicationView */, Windows::ApplicationModel::Activation::IActivatedEventArgs const& /* args */);

        private:
        void AddVisual(float2 const point);
        private:
        std::unique_ptr<DirectXManager> m_directXmanager;

        winrt::event_token m_resumingEventToken;
        winrt::event_token m_pointerPressedEventToken;
        winrt::event_token  m_closedEventToken;

        CompositionTarget m_target{ nullptr };
        VisualCollection m_visuals{ nullptr };
        Visual m_selected{ nullptr };
        float2 m_offset{};
};

struct AppViewSource : public winrt::implements<AppViewSource, winrt::Windows::ApplicationModel::Core::IFrameworkViewSource>
{
public:
    // IFrameworkViewSource method.
    winrt::Windows::ApplicationModel::Core::IFrameworkView CreateView();
private:
    IFrameworkView mainView = winrt::make<AppView>();
};

