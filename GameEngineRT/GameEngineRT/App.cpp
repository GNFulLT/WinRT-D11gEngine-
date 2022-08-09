#include "pch.h"
#include "DirectXManager.h"
#include "Graphics/GraphicsMain.h"
#include <winrt/Windows.UI.Popups.h>
using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Input;
using namespace GNF::Graphics;

struct App : winrt::implements<App, IFrameworkViewSource, IFrameworkView>
{
    bool m_closed = false;
    bool m_sizeChanging = false;
    bool m_pointerPressing = false;
    std::unique_ptr<GraphicsMain> m_graphicsMain;
    IFrameworkView CreateView()
    {
        return *this;
    }
    void OnPointerMove(IInspectable const&, PointerEventArgs const& args)
    {
        int a = 6;
    }

    void Load(winrt::hstring const& /* entryPoint */)
    {

    }

    void Initialize(CoreApplicationView const& applicationView)
    {
        applicationView.Activated({ this, &App::OnActivated });
        
        CoreApplication::Suspending({ this, &App::OnSuspending });

        CoreApplication::Resuming({ this, &App::OnResuming });

        m_graphicsMain.reset(new GraphicsMain());
    }
    //Render Loop
    void Run()
    {
        m_graphicsMain->Run();
       
    }
    void SetWindow(CoreWindow const& window)
    {
        window.PointerCursor(CoreCursor(CoreCursorType::Arrow, 0));

        PointerVisualizationSettings visualizationSettings{ PointerVisualizationSettings::GetForCurrentView() };
        visualizationSettings.IsContactFeedbackEnabled(false);
        visualizationSettings.IsBarrelButtonFeedbackEnabled(false);

        m_graphicsMain->SetWindow(window);

        window.Activated({ this, &App::OnWindowActivationChanged });
        window.Closed({ this, &App::OnWindowClosed });
        window.VisibilityChanged({ this, &App::OnVisibilityChanged });
        window.PointerPressed({ this,&App::OnPointerPressed });
        window.PointerMoved({ this,&App::OnPointerMove });
        window.ResizeStarted({ this,&App::OnResizeStarted });
        window.ResizeStarted({ this,&App::OnResizeCompleted });
        window.SizeChanged({ this,&App::OnWindowSizeChanged });
        window.PointerReleased({ this,&App::OnPointerReleased });
    }

    void OnWindowSizeChanged(CoreWindow const& sender, WindowSizeChangedEventArgs const& args)
    {
        if (m_sizeChanging)
            return;
        m_graphicsMain->OnWindowSizeChanged(args.Size());

    }
    void OnResizeStarted(CoreWindow const& sender, Windows::Foundation::IInspectable const& args)
    {
        m_sizeChanging = true;
    }

    void OnResizeCompleted(CoreWindow const& sender, Windows::Foundation::IInspectable const& args)
    {

        m_sizeChanging = false;
    }

    void OnActivated(CoreApplicationView const& /* applicationView */, IActivatedEventArgs const& /* args */)
    {
        CoreWindow window = CoreWindow::GetForCurrentThread();
        window.Activate();
    }

    void OnVisibilityChanged(CoreWindow const& /* sender */, VisibilityChangedEventArgs const& args)
    {
    }
    void App::OnPointerReleased(IInspectable const&, PointerEventArgs const& args)
    {
        m_pointerPressing = false;
    }
    void App::OnPointerPressed(IInspectable const&, PointerEventArgs const& args)
    {
        m_pointerPressing = true;
    }
    void App::OnWindowActivationChanged(CoreWindow const& /* sender */, WindowActivatedEventArgs const& args)
    {
        int a = 5;
    }
    void Uninitialize()
    {
    }
    void OnSuspending(IInspectable const& /* sender */, SuspendingEventArgs const& args)
    {

        // Save app state asynchronously after requesting a deferral. Holding a deferral
        // indicates that the application is busy performing suspending operations. Be
        // aware that a deferral may not be held indefinitely. After about five seconds,
        // the app will be forced to exit.

    }

    void OnResuming(IInspectable const& /* sender */, IInspectable const& /* args */)
    {
        // Restore any data or state that was unloaded on suspend. By default, data
        // and state are persisted when resuming from suspend. Note that this event
        // does not occur if the app was previously terminated.
    }


   
    void OnWindowClosed(CoreWindow const& /* sender */, CoreWindowEventArgs const& /* args */)
    {
        winrt::hstring Title = L"Hello";
        winrt::hstring Content = L"Some cool content!";
        winrt::Windows::UI::Popups::MessageDialog dialog(Content, Title);
        dialog.ShowAsync();
        m_closed = true;
    }



private:

};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    CoreApplication::Run(winrt::make<App>());
}