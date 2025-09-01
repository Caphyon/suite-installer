#pragma once

#include "InstallPane.g.h"

namespace winrt::SuiteInstaller::implementation
{
struct InstallPane : InstallPaneT<InstallPane>
{
  InstallPane();
  void CancelButton_Click(winrt::Windows::Foundation::IInspectable const &    sender,
                          winrt::Microsoft::UI::Xaml::RoutedEventArgs const & e);
  void RemoveAll_Click(winrt::Windows::Foundation::IInspectable const &    sender,
                       winrt::Microsoft::UI::Xaml::RoutedEventArgs const & e);
  void Install_Click(winrt::Windows::Foundation::IInspectable const &    sender,
                     winrt::Microsoft::UI::Xaml::RoutedEventArgs const & e);

private:
  SuiteInstaller::Context mContext{ nullptr };
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct InstallPane : InstallPaneT<InstallPane, implementation::InstallPane>
{
};
}  // namespace winrt::SuiteInstaller::factory_implementation
