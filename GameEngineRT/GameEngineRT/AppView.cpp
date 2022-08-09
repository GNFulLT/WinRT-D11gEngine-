#include "pch.h"
#include "AppView.h"

using namespace winrt::Windows::ApplicationModel;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Core;


//Application Lifecycle

void AppView::OnResuming(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& args)
{
    // Restore any data or state that was unloaded on suspend. By default, data
    // and state are persisted when resuming from suspend. Note that this event
    // does not occur if the app was previously terminated.

    int a = 5;
    while (true)
    {
        throw std::exception("RESUMING");
        m_directXmanager->Render();
    }
    //
    // TODO: Insert code here to load your app state.
    //
}

void AppView::OnPointerPressed(IInspectable const&, PointerEventArgs const& args)
{
    int a = 5;
}

void AppView::AddVisual(float2 const point)
{
    Compositor compositor = m_visuals.Compositor();
    SpriteVisual visual = compositor.CreateSpriteVisual();

    static Color colors[] =
    {
        { 0xDC, 0x5B, 0x9B, 0xD5 },
        { 0xDC, 0xED, 0x7D, 0x31 },
        { 0xDC, 0x70, 0xAD, 0x47 },
        { 0xDC, 0xFF, 0xC0, 0x00 }
    };

    static unsigned last = 0;
    unsigned const next = ++last % _countof(colors);
    visual.Brush(compositor.CreateColorBrush(colors[next]));

    float const BlockSize = 100.0f;

    visual.Size(
        {
            BlockSize,
            BlockSize
        });

    visual.Offset(
        {
            point.x - BlockSize / 2.0f,
            point.y - BlockSize / 2.0f,
            0.0f,
        });

    m_visuals.InsertAtTop(visual);

    m_selected = visual;
    m_offset.x = -BlockSize / 2.0f;
    m_offset.y = -BlockSize / 2.0f;
}

void AppView::OnClosed(CoreWindow const&,CoreWindowEventArgs const& args)
{
    // Restore any data or state that was unloaded on suspend. By default, data
    // and state are persisted when resuming from suspend. Note that this event
    // does not occur if the app was previously terminated.
    auto t = args.Handled();
    throw std::exception("closedd");
    int a = 5;

    //
    // TODO: Insert code here to load your app state.
    //
}


//IFrameWork Methods


// Terminate events do not cause Uninitialize to be called. It will be called if your IFrameworkView
// class is torn down while the app is in the foreground, for example if the Run method exits.
void AppView::Uninitialize()
{
   
}


// This method is called after the window becomes active. It oversees the
// update, draw, and present loop, and it also oversees window message processing.
void AppView::Run()
{

    CoreWindow window = CoreWindow::GetForCurrentThread();
    window.Activate();

   
}

// Called when the CoreWindow object is created (or re-created).
void AppView::SetWindow(CoreWindow const& window)
{
    m_closedEventToken = window.Closed({ this,  &AppView::OnClosed });
    m_pointerPressedEventToken = window.PointerPressed({ this,&AppView::OnPointerPressed });
}

// The first method called when the IFrameworkView is being created.
void AppView::Initialize(CoreApplicationView const& applicationView)
{
    applicationView.Activated({ this, &AppView::OnActivated });

    // Register event handlers for app lifecycle.
    m_resumingEventToken = CoreApplication::Resuming({ this,&AppView::OnResuming });
}


void AppView::OnActivated(CoreApplicationView const& /* applicationView */, IActivatedEventArgs const& /* args */)
{

}

// The Load method can be used to initialize scene resources or to load a
// previously saved app state.
void AppView::Load(winrt::hstring const& entryPoint)
{
}




//IFrameWorkSource Methods

IFrameworkView AppViewSource::CreateView()
{
    IFrameworkView* t = &mainView;
    return mainView;
}

/*int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    CoreApplication::Run(winrt::make<AppViewSource>());
}*/


/*


#include "pch.h"
#include "DirectXManager.h"

using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
    winrt::event_token m_windowClosedEventToken;



    CompositionTarget m_target{ nullptr };
    VisualCollection m_visuals{ nullptr };
    Visual m_selected{ nullptr };
    float2 m_offset{};
    std::unique_ptr<DirectXManager> manager;
    bool m_windowIsClosed = false;


    IFrameworkView CreateView()
    {
        return *this;
    }

    void Initialize(CoreApplicationView const&)
    {

    }

    void Load(hstring const&)
    {
    }

    void Uninitialize()
    {
    }

    void Run()
    {
        CoreWindow window = CoreWindow::GetForCurrentThread();
        window.Activate();

        CoreDispatcher dispatcher = window.Dispatcher();
        dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);

        while (!m_windowIsClosed)
        {

        }
    }

    void SetWindow(CoreWindow const& window)
    {

         m_windowClosedEventToken = window.Closed({this,&App::OnWindowClosed});



        manager.reset(new DirectXManager(const_cast<CoreWindow*>(&window)));

        /*Compositor compositor;
        ContainerVisual root = compositor.CreateContainerVisual();
        m_target = compositor.CreateTargetForCurrentView();
        m_target.Root(root);
        m_visuals = root.Children();

        window.PointerPressed({ this, &App::OnPointerPressed });
        window.PointerMoved({ this, &App::OnPointerMoved });

        window.PointerReleased([&](auto && ...)
            {
                m_selected = nullptr;
            });
    }


    //Events
    void OnWindowClosed(CoreWindow const&, CoreWindowEventArgs const&)
    {
        manager->Render();
        int a = 5;
    }
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    CoreApplication::Run(make<App>());
}
*/