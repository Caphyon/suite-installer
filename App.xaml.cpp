// Copyright (c) Caphyon.

#include "stdafx.h"
#include "App.xaml.h"
#include "MainWindow.xaml.h"
#include <util/Utils.h>

using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Controls;
using namespace winrt::Microsoft::UI::Xaml::Navigation;
using namespace winrt::SuiteInstaller;
using namespace winrt::SuiteInstaller::implementation;

//----------------------------------------------------------------------------

const CString APPLICATION_NAME = L"Suite Installer";

//----------------------------------------------------------------------------

App::App()
{
  InitializeComponent();

  std::filesystem::path configFilePath = GetModulePath().replace_filename(L"SuiteInstaller.ini");
  auto externalUiPath = GetIniValue(configFilePath.c_str(), L"Properties", L"ExternalUiPath");
  mAppIcon            = ExtractLargestIconFromExe(externalUiPath.c_str());

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
  UnhandledException(
    [this](IInspectable const &, UnhandledExceptionEventArgs const & e)
    {
      if (IsDebuggerPresent())
      {
        auto errorMessage = e.Message();
        __debugbreak();
      }
    });
#endif
}

winrt::SuiteInstaller::AppService App::AppService()
{
  return winrt::SuiteInstaller::AppService::GetInstance();
}

winrt::fire_and_forget App::SetTitleBarIcon()
{
  Microsoft::UI::Xaml::Media::Imaging::BitmapImage bmpImage =
    co_await ConvertHIconToBitmapImageAsync(mAppIcon);

  App::AppService().RuntimeContext().AppIcon(bmpImage);
}

void winrt::SuiteInstaller::implementation::App::SetTaskBarIcon()
{
  auto windowNative{ mWindow.try_as<::IWindowNative>() };
  if (windowNative)
  {
    HWND hWnd{ 0 };
    windowNative->get_WindowHandle(&hWnd);

    if (hWnd)
    {
      SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)mAppIcon);
      SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)mAppIcon);
    }
  }
}

void App::OnLaunched(LaunchActivatedEventArgs const &)
{
  mWindow = make<MainWindow>();

  auto windowId  = mWindow.AppWindow().Id();
  auto appWindow = winrt::Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);

  SetTitleBarIcon();
  SetTaskBarIcon();

  if (appWindow)
  {
    auto displayArea = winrt::Microsoft::UI::Windowing::DisplayArea::GetFromWindowId(
      windowId, winrt::Microsoft::UI::Windowing::DisplayAreaFallback::Nearest);

    if (displayArea)
    {
      auto centeredPosition = appWindow.Position();
      centeredPosition.X    = (displayArea.WorkArea().Width - appWindow.Size().Width) / 2;
      centeredPosition.Y    = (displayArea.WorkArea().Height - appWindow.Size().Height) / 2;
      appWindow.Move(centeredPosition);
    }
  }
  mWindow.Activate();
}
