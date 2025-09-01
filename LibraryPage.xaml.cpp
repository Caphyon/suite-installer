#include "stdafx.h"
#include "LibraryPage.xaml.h"
#if __has_include("LibraryPage.g.cpp")
#include "LibraryPage.g.cpp"
#endif

#include "App.xaml.h"
#include "Prerequisite.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

namespace winrt::SuiteInstaller::implementation
{

LibraryPage::LibraryPage()
  : mContext(App::AppService().RuntimeContext())
{
  this->DataContext(mContext);
}

void LibraryPage::Prerequisite_Clicked(IInspectable const & /*sender*/,
                                       Controls::ItemClickEventArgs const & e)
{
  PrerequisiteList().PrepareConnectedAnimation(L"ForwardConnectedGridAnimation", e.ClickedItem(),
                                               L"ConnectedGrid");
  PrerequisiteList().PrepareConnectedAnimation(L"ForwardConnectedAnimation", e.ClickedItem(),
                                               L"ConnectedIcon");
  PrerequisiteList().PrepareConnectedAnimation(L"ForwardConnectedTitleAnimation", e.ClickedItem(),
                                               L"ConnectedTitle");

  Frame().Navigate(xaml_typename<SuiteInstaller::PrerequisitePage>(), e.ClickedItem());
}

}  // namespace winrt::SuiteInstaller::implementation

void winrt::SuiteInstaller::implementation::LibraryPage::InstallButton_Click(
  winrt::Windows::Foundation::IInspectable const &,
  winrt::Microsoft::UI::Xaml::RoutedEventArgs const & e)
{
  auto listItem = e.OriginalSource().try_as<FrameworkElement>().Parent().as<FrameworkElement>();
  auto prereq   = listItem.DataContext().try_as<SuiteInstaller::Prerequisite>();

  App::AppService().InstallingPrereqs().AddPrerequisite(prereq);
  prereq.WaitingForInstall(true);
}

void winrt::SuiteInstaller::implementation::LibraryPage::OpenButton_Click(
  winrt::Windows::Foundation::IInspectable const &,
  winrt::Microsoft::UI::Xaml::RoutedEventArgs const & e)
{
  auto listItem = e.OriginalSource().try_as<FrameworkElement>().Parent().as<FrameworkElement>();
  auto prereq   = listItem.DataContext().try_as<SuiteInstaller::Prerequisite>();

  std::thread(
    [this, prereq]()
    {
      STARTUPINFO         startupInfo = { sizeof(STARTUPINFO) };
      PROCESS_INFORMATION processInfo;
      std::wstring        command = prereq.OpenPath().c_str();

      CreateProcess(nullptr, command.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr,
                    &startupInfo, &processInfo);
    })
    .detach();
}

void winrt::SuiteInstaller::implementation::LibraryPage::InstalledPrerequisite_Clicked(
  winrt::Windows::Foundation::IInspectable const &,
  winrt::Microsoft::UI::Xaml::Controls::ItemClickEventArgs const & e)
{
  InstalledPrerequisiteList().PrepareConnectedAnimation(L"ForwardConnectedGridAnimation",
                                                        e.ClickedItem(), L"ConnectedGrid");
  InstalledPrerequisiteList().PrepareConnectedAnimation(L"ForwardConnectedAnimation",
                                                        e.ClickedItem(), L"ConnectedIcon");
  InstalledPrerequisiteList().PrepareConnectedAnimation(L"ForwardConnectedTitleAnimation",
                                                        e.ClickedItem(), L"ConnectedTitle");

  Frame().Navigate(xaml_typename<SuiteInstaller::PrerequisitePage>(), e.ClickedItem());
}
