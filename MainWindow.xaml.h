// Copyright (c) Caphyon

#pragma once

#include "MainWindow.g.h"

namespace winrt::SuiteInstaller::implementation
{
struct MainWindow : MainWindowT<MainWindow>
{
  MainWindow();

  void AppTitleBar_Loaded(winrt::Windows::Foundation::IInspectable const & /*sender*/,
                          winrt::Microsoft::UI::Xaml::RoutedEventArgs const & /*e*/);
  void AppTitleBar_SizeChanged(winrt::Windows::Foundation::IInspectable const & /*sender*/,
                               winrt::Microsoft::UI::Xaml::RoutedEventArgs const & /*e*/);
  void SetRegionsForCustomTitleBar();

  Windows::Graphics::RectInt32 GetRect(const Windows::Foundation::Rect & bounds, double scale);

  HWND GetMainWindow();

  static LRESULT CALLBACK SubclassProc(HWND   hWnd,
                                       UINT   message,
                                       WPARAM wParam,
                                       LPARAM lParam,
                                       UINT_PTR /*uIdSubclass*/,
                                       DWORD_PTR /*dwRefData*/);

  void InitializeAppBarComponents();
  void SetAppTheme();
  void SetAppBarButtonsColor(Microsoft::UI::Xaml::ElementTheme aTheme);
  bool LoadThemeSetting();
  void SaveThemeSetting(bool isLight);

  void ThemeButton_Click(winrt::Windows::Foundation::IInspectable const & sender,
                         winrt::Microsoft::UI::Xaml::RoutedEventArgs const &);

  void HomeClick(Windows::Foundation::IInspectable const &    sender,
                 Microsoft::UI::Xaml::RoutedEventArgs const & e);

  void LibraryClick(Windows::Foundation::IInspectable const &    sender,
                    Microsoft::UI::Xaml::RoutedEventArgs const & e);

  void NewsTapped(Windows::Foundation::IInspectable const &    sender,
                  Microsoft::UI::Xaml::RoutedEventArgs const & e);

  void CloseButtonNews_Click(Windows::Foundation::IInspectable const &    sender,
                             Microsoft::UI::Xaml::RoutedEventArgs const & e);

  void UpgradeButton_Click(Windows::Foundation::IInspectable const &    sender,
                           Microsoft::UI::Xaml::RoutedEventArgs const & e);

  void GetHelp_Tapped(IInspectable const & sender,
                      winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const & /*e*/);
  void Feedback_Tapped(IInspectable const & sender,
                       winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const & /*e*/);

  SuiteInstaller::InstallModel GetInstallingPrereqModel();

  SuiteInstaller::Context Context();

  void OnBackRequested(
    winrt::Microsoft::UI::Xaml::Controls::NavigationView const &                       sender,
    winrt::Microsoft::UI::Xaml::Controls::NavigationViewBackRequestedEventArgs const & args);

  void OnNavigationViewLoaded(winrt::Windows::Foundation::IInspectable const &    sender,
                              winrt::Microsoft::UI::Xaml::RoutedEventArgs const & e);

  void OnPointerPressed(IInspectable const &,
                        winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const & args);

  void OnNavigationViewDisplayModeChanged(
    winrt::Microsoft::UI::Xaml::Controls::NavigationView const &                            sender,
    winrt::Microsoft::UI::Xaml::Controls::NavigationViewDisplayModeChangedEventArgs const & args);

private:
  SuiteInstaller::Context mContext{ nullptr };

public:
  void OnSearchBoxTextChanged(
    winrt::Microsoft::UI::Xaml::Controls::AutoSuggestBox const &                     sender,
    winrt::Microsoft::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs const & args);
  void OnControlsSearchBoxQuerySubmitted(
    winrt::Microsoft::UI::Xaml::Controls::AutoSuggestBox const &                        sender,
    winrt::Microsoft::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs const & args);
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
{
};
}  // namespace winrt::SuiteInstaller::factory_implementation
