#pragma once

#include "PrerequisitePage.g.h"

namespace winrt::SuiteInstaller::implementation
{
struct PrerequisitePage : PrerequisitePageT<PrerequisitePage>
{
  PrerequisitePage();

  void GoBack();

  void Image_Tapped(winrt::Windows::Foundation::IInspectable const &                 sender,
                    winrt::Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const & e);
  void HidePopup(IInspectable const & sender, Microsoft::UI::Xaml::RoutedEventArgs const & args);

  void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const & e);

  void InstallButton_Click(IInspectable const & sender,
                           Microsoft::UI::Xaml::RoutedEventArgs const &);
  void OpenButton_Click(winrt::Windows::Foundation::IInspectable const &    sender,
                        winrt::Microsoft::UI::Xaml::RoutedEventArgs const & e);

  SuiteInstaller::Prerequisite mPrereq{ nullptr };

private:
  SuiteInstaller::Context mContext{ nullptr };
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct PrerequisitePage : PrerequisitePageT<PrerequisitePage, implementation::PrerequisitePage>
{
};
}  // namespace winrt::SuiteInstaller::factory_implementation
