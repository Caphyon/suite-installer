#pragma once

#include "AppService.g.h"
#include "Context.h"
#include "InstallModel.h"

namespace winrt::SuiteInstaller::implementation
{
struct AppService : AppServiceT<AppService>
{
  AppService();

  static SuiteInstaller::AppService GetInstance();

  SuiteInstaller::Context      RuntimeContext();
  SuiteInstaller::InstallModel InstallingPrereqs();

private:
  SuiteInstaller::Context      mRuntimeContext{ nullptr };
  SuiteInstaller::InstallModel mInstallingPrereqs{ nullptr };
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct AppService : AppServiceT<AppService, implementation::AppService, winrt::static_lifetime>
{
  SuiteInstaller::AppService Singleton();

private:
  winrt::slim_mutex          m_lock;
  SuiteInstaller::AppService mSingleton;
};
}  // namespace winrt::SuiteInstaller::factory_implementation
