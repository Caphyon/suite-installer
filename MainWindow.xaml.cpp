// Copyright (c) Caphyon.

#include "stdafx.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "App.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Input;

using namespace Windows::Foundation;
using namespace Windows::Graphics;

using namespace Windows::UI::Core;
using namespace Microsoft::UI::Xaml::Navigation;

namespace
{

}  // namespace

namespace winrt::SuiteInstaller::implementation
{
MainWindow::MainWindow()
  : mContext(App::AppService().RuntimeContext())
{
  InitializeComponent();
  InitializeAppBarComponents();
  AppWindow().Resize({ 2000, 1300 });
  SetWindowSubclass(GetMainWindow(), &MainWindow::SubclassProc, 0, 0);

  SetAppTheme();

  AppTitleBar().DataContext(mContext);

  rootGrid().PointerPressed({ this, &MainWindow::OnPointerPressed });

  LoadingOverlay().Visibility(mContext.InstallStateLoaded() ? Visibility::Collapsed
                                                            : Visibility::Visible);

  news().Visibility(mContext.UpdatesLoaded() ? Visibility::Visible : Visibility::Collapsed);

  SmokeGrid().DataContext(mContext);
  LoadingOverlay().DataContext(mContext);

  MainFrame().Navigated(
    [this](const winrt::Windows::Foundation::IInspectable &,
           const winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs & args)
    {
      auto currentPageType = args.SourcePageType();
      auto navigationMode  = args.NavigationMode();

      if (navigationMode == NavigationMode::Back)
      {
        if (currentPageType.Name == L"SuiteInstaller.HomePage")
        {
          navigationView().SelectedItem(navigationView().MenuItems().GetAt(0));
          AnimatedIcon::SetState(libraryIcon(), L"Normal");
          AnimatedIcon::SetState(homeIcon(), L"Selected");

          homeText().Visibility(Visibility::Collapsed);
          libraryText().Visibility(Visibility::Visible);
        }
        else if (currentPageType.Name == L"SuiteInstaller.LibraryPage")
        {
          navigationView().SelectedItem(navigationView().FooterMenuItems().GetAt(1));

          AnimatedIcon::SetState(homeIcon(), L"Normal");
          AnimatedIcon::SetState(libraryIcon(), L"Selected");

          libraryText().Visibility(Visibility::Collapsed);
          homeText().Visibility(Visibility::Visible);
        }
      }

      navigationView().IsBackEnabled(MainFrame().CanGoBack());
    });

  // taskbar icon
  // auto windowId  = this->AppWindow().Id();
  // auto appWindow = winrt::Microsoft::UI::Windowing::AppWindow::GetFromWindowId(windowId);

  // appWindow.SetIcon(L"assets\\test1.jpg");
}

void MainWindow::AppTitleBar_Loaded(winrt::Windows::Foundation::IInspectable const & /*sender*/,
                                    winrt::Microsoft::UI::Xaml::RoutedEventArgs const & /*e*/)
{
  if (ExtendsContentIntoTitleBar())
  {
    SetRegionsForCustomTitleBar();
  }
}

void MainWindow::AppTitleBar_SizeChanged(
  winrt::Windows::Foundation::IInspectable const & /*sender*/,
  winrt::Microsoft::UI::Xaml::RoutedEventArgs const & /*e*/)
{
  if (ExtendsContentIntoTitleBar())
  {
    SetRegionsForCustomTitleBar();
  }
}

void MainWindow::SetRegionsForCustomTitleBar()
{
  // Specify the interactive regions of the title bar.

  double scaleAdjustment = AppTitleBar().XamlRoot().RasterizationScale();

  RightPaddingColumn().Width(
    GridLengthHelper::FromPixels(AppWindow().TitleBar().RightInset() / scaleAdjustment));
  LeftPaddingColumn().Width(
    GridLengthHelper::FromPixels(AppWindow().TitleBar().LeftInset() / scaleAdjustment));

  // Search bar and account icon
  auto transform = SearchBox().TransformToVisual(nullptr);
  Rect bounds    = transform.TransformBounds({ 0, 0, static_cast<float>(SearchBox().ActualWidth()),
                                               static_cast<float>(SearchBox().ActualHeight()) });
  Windows::Graphics::RectInt32 SearchBoxRect = GetRect(bounds, scaleAdjustment);

  transform = ThemeButton().TransformToVisual(nullptr);
  bounds    = transform.TransformBounds({ 0, 0, static_cast<float>(ThemeButton().ActualWidth()),
                                          static_cast<float>(ThemeButton().ActualHeight()) });
  Windows::Graphics::RectInt32 themeButtonBoxRect = GetRect(bounds, scaleAdjustment);

  std::array<Windows::Graphics::RectInt32, 2> rectArray = { SearchBoxRect, themeButtonBoxRect };

  auto nonClientInputSrc = InputNonClientPointerSource::GetForWindowId(AppWindow().Id());
  nonClientInputSrc.SetRegionRects(NonClientRegionKind::Passthrough, rectArray);
}

Windows::Graphics::RectInt32 MainWindow::GetRect(const Rect & bounds, double scale)
{
  return { static_cast<int32_t>(std::round(bounds.X * scale)),
           static_cast<int32_t>(std::round(bounds.Y * scale)),
           static_cast<int32_t>(std::round(bounds.Width * scale)),
           static_cast<int32_t>(std::round(bounds.Height * scale)) };
}

LRESULT CALLBACK MainWindow::SubclassProc(HWND   hWnd,
                                          UINT   message,
                                          WPARAM wParam,
                                          LPARAM lParam,
                                          UINT_PTR /*uIdSubclass*/,
                                          DWORD_PTR /*dwRefData*/)
{
  switch (message)
  {
  case WM_GETMINMAXINFO:
  {
    MINMAXINFO * lpMinMaxInfo      = (MINMAXINFO *)lParam;
    lpMinMaxInfo->ptMinTrackSize.x = 1000;  // Minimum width
    lpMinMaxInfo->ptMinTrackSize.y = 500;   // Minimum height
    return 0;
  }
  default:
    return DefSubclassProc(hWnd, message, wParam, lParam);
  }
}

void MainWindow::InitializeAppBarComponents()
{
  this->SetTitleBar(AppTitleBar());
  AppTitleBar().Loaded({ this, &MainWindow::AppTitleBar_Loaded });
  AppTitleBar().SizeChanged({ this, &MainWindow::AppTitleBar_SizeChanged });
  ExtendsContentIntoTitleBar(true);
}

HWND MainWindow::GetMainWindow()
{
  auto windowNative{ this->try_as<::IWindowNative>() };
  if (!windowNative)
    return 0;

  HWND hWnd{ 0 };
  windowNative->get_WindowHandle(&hWnd);

  return hWnd;
}

void MainWindow::SetAppBarButtonsColor(Microsoft::UI::Xaml::ElementTheme aTheme)
{
  auto buttonsTitleBar = AppWindow().TitleBar();
  if (aTheme == ElementTheme::Dark)
  {
    buttonsTitleBar.ButtonForegroundColor(Windows::UI::Colors::White());
    buttonsTitleBar.ButtonHoverForegroundColor(Windows::UI::Colors::White());
    buttonsTitleBar.ButtonHoverBackgroundColor(Windows::UI::ColorHelper::FromArgb(255, 45, 45, 45));
  }
  else if (aTheme == ElementTheme::Light)
  {
    buttonsTitleBar.ButtonForegroundColor(Windows::UI::Colors::Black());
    buttonsTitleBar.ButtonHoverForegroundColor(Windows::UI::Colors::Black());
    buttonsTitleBar.ButtonHoverBackgroundColor(
      Windows::UI::ColorHelper::FromArgb(255, 227, 227, 227));
  }
}

void MainWindow::SetAppTheme()
{
  // until user change it we use the Dark one
  auto rootElement = this->Content().as<FrameworkElement>();

  if (LoadThemeSetting())
  {
    rootElement.RequestedTheme(ElementTheme::Light);
    SetAppBarButtonsColor(ElementTheme::Light);
    AnimatedIcon::SetState(Moon(), L"Dark");
  }
  else
  {
    rootElement.RequestedTheme(ElementTheme::Dark);
    SetAppBarButtonsColor(ElementTheme::Dark);

    AnimatedIcon::SetState(Moon(), L"Light");
  }
}

void MainWindow::ThemeButton_Click(IInspectable const &, RoutedEventArgs const &)
{
  auto rootElement = this->Content().as<FrameworkElement>();

  if (rootElement != nullptr)
  {
    auto currTheme = rootElement.RequestedTheme();
    if (currTheme == ElementTheme::Dark)
    {
      rootElement.RequestedTheme(ElementTheme::Light);

      SetAppBarButtonsColor(ElementTheme::Light);
      AnimatedIcon::SetState(Moon(), L"Dark");

      SaveThemeSetting(true);
    }
    else
    {
      rootElement.RequestedTheme(ElementTheme::Dark);

      SetAppBarButtonsColor(ElementTheme::Dark);
      AnimatedIcon::SetState(Moon(), L"Light");

      SaveThemeSetting(false);
    }
  }
}

void MainWindow::SaveThemeSetting(bool isLight)
{
  HKEY hKey;
  LONG result = RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\SuiteInstaller\\Settings", 0, NULL,
                               REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);

  if (result == ERROR_SUCCESS)
  {
    DWORD themeValue = isLight ? 1 : 0;  // Save 1 for Light, 0 for Dark
    RegSetValueEx(hKey, L"Theme", 0, REG_DWORD, reinterpret_cast<const BYTE *>(&themeValue),
                  sizeof(themeValue));
    RegCloseKey(hKey);
  }
}

bool MainWindow::LoadThemeSetting()
{
  HKEY  hKey;
  DWORD lightTheme = 0;
  LONG  result =
    RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\SuiteInstaller\\Settings", 0, KEY_READ, &hKey);

  if (result == ERROR_SUCCESS)
  {
    DWORD dataSize = sizeof(lightTheme);
    RegQueryValueEx(hKey, L"Theme", NULL, NULL, reinterpret_cast<BYTE *>(&lightTheme), &dataSize);
    RegCloseKey(hKey);
  }

  return lightTheme == 1;
}

void MainWindow::GetHelp_Tapped(IInspectable const &, Input::TappedRoutedEventArgs const &)
{
  if (mContext.HelpLink().empty())
    return;

  winrt::Windows::System::Launcher::LaunchUriAsync(
    winrt::Windows::Foundation::Uri(mContext.HelpLink()));
}

void MainWindow::Feedback_Tapped(IInspectable const &, Input::TappedRoutedEventArgs const &)
{
  if (mContext.FeedbackLink().empty())
    return;

  winrt::Windows::System::Launcher::LaunchUriAsync(
    winrt::Windows::Foundation::Uri(mContext.FeedbackLink()));
}

SuiteInstaller::InstallModel MainWindow::GetInstallingPrereqModel()
{
  return App::AppService().InstallingPrereqs();
}

SuiteInstaller::Context MainWindow::Context()
{
  return mContext;
}

void MainWindow::HomeClick(IInspectable const & sender, RoutedEventArgs const &)
{
  auto navViewItem = sender.as<NavigationViewItem>();

  if (navViewItem.IsSelected())
  {
    AnimatedIcon::SetState(libraryIcon(), L"Normal");
    AnimatedIcon::SetState(homeIcon(), L"Selected");

    homeText().Visibility(Visibility::Collapsed);
    libraryText().Visibility(Visibility::Visible);

    auto navViewItemTag = unbox_value<hstring>(navViewItem.Tag());

    MainFrame().Navigate({ navViewItemTag }, *this);
  }
}

void MainWindow::LibraryClick(IInspectable const & sender, RoutedEventArgs const &)
{
  auto navViewItem = sender.as<NavigationViewItem>();

  if (navViewItem.IsSelected())
  {
    AnimatedIcon::SetState(homeIcon(), L"Normal");
    AnimatedIcon::SetState(libraryIcon(), L"Selected");

    libraryText().Visibility(Visibility::Collapsed);
    homeText().Visibility(Visibility::Visible);

    auto navViewItemTag = unbox_value<hstring>(navViewItem.Tag());
    MainFrame().Navigate({ navViewItemTag }, *this);
  }
}

void MainWindow::NewsTapped(IInspectable const &, RoutedEventArgs const &)
{
  SmokeGrid().Visibility(Visibility::Visible);
}

void MainWindow::CloseButtonNews_Click(IInspectable const &, RoutedEventArgs const &)
{
  SmokeGrid().Visibility(Visibility::Collapsed);
}

void MainWindow::OnBackRequested(NavigationView const &,
                                 NavigationViewBackRequestedEventArgs const &)
{
  if (MainFrame().CanGoBack())
  {
    MainFrame().GoBack();
  }
}

void MainWindow::OnNavigationViewLoaded(IInspectable const &, RoutedEventArgs const &)
{
  for (auto const & item : navigationView().MenuItems())
  {
    if (auto navViewItem = item.try_as<NavigationViewItem>())
    {
      if (winrt::unbox_value<hstring>(navViewItem.Tag()) == L"SuiteInstaller.HomePage")
      {
        navigationView().SelectedItem(navViewItem);
        break;
      }
    }
  }

  MainFrame().Navigate({ L"SuiteInstaller.HomePage" }, *this);
}

void MainWindow::OnNavigationViewDisplayModeChanged(
  NavigationView const & sender, NavigationViewDisplayModeChangedEventArgs const &)
{
  auto titleBarMargin = AppTitleBar().Margin();

  titleBarMargin.Left = sender.CompactPaneLength();

  AppTitleBar().Margin(titleBarMargin);

  sender.UpdateLayout();
}

void MainWindow::OnPointerPressed(
  IInspectable const &, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const & args)
{
  auto pointerPoint = args.GetCurrentPoint(nullptr);

  if (pointerPoint.Properties().IsXButton1Pressed())
  {
    OnBackRequested(nullptr, nullptr);
    args.Handled(true);
  }
}

void MainWindow::OnSearchBoxTextChanged(AutoSuggestBox const &                     sender,
                                        AutoSuggestBoxTextChangedEventArgs const & args)
{
  if (args.Reason() == AutoSuggestionBoxTextChangeReason::UserInput)
  {
    auto suggestions = mContext.GetSearchList(sender.Text());

    if (suggestions.Size() != 0)
    {
      SearchBox().ItemsSource(suggestions);
    }
    else
    {
      auto noResults = winrt::single_threaded_observable_vector<hstring>();
      noResults.Append(L"No results found");
      sender.ItemsSource(noResults);
    }
  }
}

void MainWindow::OnControlsSearchBoxQuerySubmitted(
  AutoSuggestBox const &, AutoSuggestBoxQuerySubmittedEventArgs const & args)
{
  if (args.ChosenSuggestion() != nullptr)
  {
    auto prereq = args.ChosenSuggestion().try_as<SuiteInstaller::Prerequisite>();
    if (prereq)
    {
      MainFrame().Navigate(xaml_typename<SuiteInstaller::PrerequisitePage>(), prereq);
    }
  }
}

void MainWindow::UpgradeButton_Click(IInspectable const &, RoutedEventArgs const &)
{
  if (mContext)
  {
    mContext.LaunchUpdater();
  }
}

}  // namespace winrt::SuiteInstaller::implementation
