#pragma once

#include "LibraryPage.g.h"

namespace winrt::SuiteInstaller::implementation
{
struct LibraryPage : LibraryPageT<LibraryPage>
{
  LibraryPage();

  void Prerequisite_Clicked(IInspectable const & sender,
                            Microsoft::UI::Xaml::Controls::ItemClickEventArgs const & /*e*/);
  void InstalledPrerequisite_Clicked(
    winrt::Windows::Foundation::IInspectable const &                 sender,
    winrt::Microsoft::UI::Xaml::Controls::ItemClickEventArgs const & e);

private:
  SuiteInstaller::Context mContext{ nullptr };

public:
  void InstallButton_Click(winrt::Windows::Foundation::IInspectable const &    sender,
                           winrt::Microsoft::UI::Xaml::RoutedEventArgs const & e);
  void OpenButton_Click(winrt::Windows::Foundation::IInspectable const &    sender,
                        winrt::Microsoft::UI::Xaml::RoutedEventArgs const & e);
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct LibraryPage : LibraryPageT<LibraryPage, implementation::LibraryPage>
{
};
}  // namespace winrt::SuiteInstaller::factory_implementation
