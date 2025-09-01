#include "stdafx.h"
#include "PrerequisitePage.xaml.h"
#if __has_include("PrerequisitePage.g.cpp")
#include "PrerequisitePage.g.cpp"
#endif

#include "App.xaml.h"
#include "Prerequisite.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::SuiteInstaller::implementation
{

PrerequisitePage::PrerequisitePage()
  : mContext(App::AppService().RuntimeContext())
{
}

void PrerequisitePage::GoBack()
{
  if (Frame() && Frame().CanGoBack())
  {
    Frame().GoBack();
  }
}

void PrerequisitePage::Image_Tapped(winrt::Windows::Foundation::IInspectable const & sender,
                                    Input::TappedRoutedEventArgs const & /*e*/)
{
  auto image      = sender.as<Microsoft::UI::Xaml::Controls::Image>();
  auto tappedItem = image.DataContext();

  PopupFlipView().SelectedItem(tappedItem);
  ImagePopup().IsOpen(true);
  Overlay().Visibility(Microsoft::UI::Xaml::Visibility::Visible);
}

void PrerequisitePage::HidePopup(IInspectable const &, RoutedEventArgs const &)
{
  ImagePopup().IsOpen(false);
  Overlay().Visibility(Visibility::Collapsed);
}

void PrerequisitePage::OnNavigatedTo(Navigation::NavigationEventArgs const & e)
{
  mPrereq = e.Parameter().try_as<SuiteInstaller::Prerequisite>();

  if (mPrereq)
  {
    this->DataContext(mPrereq);

    auto animationService =
      Microsoft::UI::Xaml::Media::Animation::ConnectedAnimationService::GetForCurrentView();

    auto gridAnimation = animationService.GetAnimation(L"ForwardConnectedGridAnimation");
    if (gridAnimation)
    {
      gridAnimation.TryStart(animatedGrid());
    }

    auto iconAnimation = animationService.GetAnimation(L"ForwardConnectedAnimation");
    if (iconAnimation)
    {
      iconAnimation.TryStart(Icon());
    }

    auto titleAnimation = animationService.GetAnimation(L"ForwardConnectedTitleAnimation");
    if (titleAnimation)
    {
      titleAnimation.TryStart(Title());
    }
  }
}

void PrerequisitePage::InstallButton_Click(IInspectable const &, RoutedEventArgs const &)
{
  auto installmodel = App::AppService().InstallingPrereqs();
  auto prereq       = this->DataContext().as<SuiteInstaller::Prerequisite>();

  installmodel.AddPrerequisite(prereq);
  prereq.WaitingForInstall(true);
}

void PrerequisitePage::OpenButton_Click(IInspectable const &, RoutedEventArgs const &)
{
  std::thread(
    [this]()
    {
      STARTUPINFO         startupInfo = { sizeof(STARTUPINFO) };
      PROCESS_INFORMATION processInfo;
      std::wstring        command = mPrereq.OpenPath().c_str();

      CreateProcess(nullptr, command.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr,
                    &startupInfo, &processInfo);
    })
    .detach();
}

}  // namespace winrt::SuiteInstaller::implementation
