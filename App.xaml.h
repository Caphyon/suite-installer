// Copyright (c) Caphyon.

#pragma once

#include "App.xaml.g.h"
#include "AppService.h"
#include "Context.h"
#include "ConverterBoolToObject.h"
#include "HomeIcon.h"
#include "InstallModel.h"
#include "LibraryIcon.h"
#include "Moon.h"
#include "Prerequisite.h"

namespace winrt::SuiteInstaller::implementation
{
struct App : AppT<App>
{
  App();

  static winrt::SuiteInstaller::AppService AppService();

  void OnLaunched(winrt::Microsoft::UI::Xaml::LaunchActivatedEventArgs const &);

  winrt::fire_and_forget SetTitleBarIcon();
  void                   SetTaskBarIcon();

private:
  winrt::Microsoft::UI::Xaml::Window mWindow{ nullptr };

  HICON mAppIcon;
};
}  // namespace winrt::SuiteInstaller::implementation
