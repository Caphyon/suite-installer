#pragma once

#include "HomePage.g.h"

namespace winrt::SuiteInstaller::implementation
{
struct HomePage : HomePageT<HomePage>
{
  HomePage();

  void OnNavigatedFrom(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const & e);

  void FlipViewItem_Tapped(IInspectable const &                                      sender,
                           Microsoft::UI::Xaml::Input::TappedRoutedEventArgs const & e);

  void GridViewItem_Clicked(IInspectable const & sender,
                            Microsoft::UI::Xaml::Controls::ItemClickEventArgs const & /*e*/);

private:
  Microsoft::UI::Xaml::DispatcherTimer timer{ nullptr };

  SuiteInstaller::Context mContext{ nullptr };

  void SetupCarouselTimer();
  void OnTimerTick(IInspectable const & sender, IInspectable const & e);
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct HomePage : HomePageT<HomePage, implementation::HomePage>
{
};
}  // namespace winrt::SuiteInstaller::factory_implementation
