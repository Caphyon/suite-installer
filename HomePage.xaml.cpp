#include "stdafx.h"
#include "HomePage.xaml.h"
#if __has_include("HomePage.g.cpp")
#include "HomePage.g.cpp"
#endif

#include "App.xaml.h"
#include "Prerequisite.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Media::Imaging;
using namespace Microsoft::UI::Xaml::Media::Animation;

namespace winrt::SuiteInstaller::implementation
{

HomePage::HomePage()
  : mContext(App::AppService().RuntimeContext())
{
  this->DataContext(mContext);

  SetupCarouselTimer();
}

void HomePage::SetupCarouselTimer()
{
  if (!timer)
  {
    timer = DispatcherTimer();
    timer.Interval(Windows::Foundation::TimeSpan{ std::chrono::seconds(5) });
    timer.Tick({ this, &HomePage::OnTimerTick });
    timer.Start();
  }
}

void HomePage::OnTimerTick(IInspectable const &, IInspectable const &)
{
  if (HomeFlipView().Items() && HomeFlipView().Items().Size() > 0)
  {
    HomeFlipView().SelectedIndex((HomeFlipView().SelectedIndex() + 1) %
                                 HomeFlipView().Items().Size());
  }
}

void HomePage::OnNavigatedFrom(Navigation::NavigationEventArgs const & e)
{
  if (timer)
  {
    timer.Stop();
    timer = nullptr;
  }
  HomePageT<HomePage>::OnNavigatedFrom(e);
}

void HomePage::FlipViewItem_Tapped(IInspectable const & sender,
                                   Input::TappedRoutedEventArgs const & /*e*/)
{
  auto grid = sender.try_as<Controls::Grid>();
  if (grid)
  {
    auto prereq = grid.DataContext().try_as<SuiteInstaller::Prerequisite>();
    if (prereq)
    {
      auto image = grid.FindName(L"ConnectedImage").try_as<Controls::Image>();
      auto title = grid.FindName(L"ConnectedTitle").try_as<Controls::TextBlock>();
      if (image)
      {
        auto animationService = ConnectedAnimationService::GetForCurrentView();
        animationService.PrepareToAnimate(L"ForwardConnectedAnimation", image);
        animationService.PrepareToAnimate(L"ForwardConnectedTitleAnimation", title);
      }

      Frame().Navigate(xaml_typename<SuiteInstaller::PrerequisitePage>(), prereq);
    }
  }
}

void HomePage::GridViewItem_Clicked(IInspectable const & /*sender*/,
                                    Controls::ItemClickEventArgs const & e)
{
  ConnectedGridView().PrepareConnectedAnimation(L"ForwardConnectedAnimation", e.ClickedItem(),
                                                L"ConnectedIcon");
  ConnectedGridView().PrepareConnectedAnimation(L"ForwardConnectedTitleAnimation", e.ClickedItem(),
                                                L"ConnectedTitle");

  Frame().Navigate(xaml_typename<SuiteInstaller::PrerequisitePage>(), e.ClickedItem());
}

}  // namespace winrt::SuiteInstaller::implementation
