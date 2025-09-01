#include "stdafx.h"
#include "InstallPane.xaml.h"
#if __has_include("InstallPane.g.cpp")
#include "InstallPane.g.cpp"
#endif

#include "App.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::SuiteInstaller::implementation
{
InstallPane::InstallPane()
  : mContext(App::AppService().RuntimeContext())
{
  InitializeComponent();
}

}  // namespace winrt::SuiteInstaller::implementation

void winrt::SuiteInstaller::implementation::InstallPane::CancelButton_Click(
  winrt::Windows::Foundation::IInspectable const & sender,
  winrt::Microsoft::UI::Xaml::RoutedEventArgs const &)
{
  auto installmodel = App::AppService().InstallingPrereqs();

  auto button = sender.try_as<Microsoft::UI::Xaml::Controls::Button>();
  auto prereq = button.DataContext().try_as<SuiteInstaller::Prerequisite>();

  installmodel.RemovePrerequisite(prereq);
}

void winrt::SuiteInstaller::implementation::InstallPane::RemoveAll_Click(
  winrt::Windows::Foundation::IInspectable const &,
  winrt::Microsoft::UI::Xaml::RoutedEventArgs const &)
{
  App::AppService().InstallingPrereqs().RemoveAllPrerequisites();
}

void winrt::SuiteInstaller::implementation::InstallPane::Install_Click(
  winrt::Windows::Foundation::IInspectable const &,
  winrt::Microsoft::UI::Xaml::RoutedEventArgs const &)
{
  mContext.InstallPackages(DispatcherQueue());
}
