#include "stdafx.h"
#include "AppService.h"
#if __has_include("AppService.g.cpp")
#include "AppService.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::SuiteInstaller::implementation
{
AppService::AppService()
{
}

SuiteInstaller::AppService AppService::GetInstance()
{
  auto fooFactory = winrt::make_self<factory_implementation::AppService>();
  return fooFactory->Singleton();
}

SuiteInstaller::Context AppService::RuntimeContext()
{
  if (!mRuntimeContext)
    mRuntimeContext = make<SuiteInstaller::implementation::Context>();

  return mRuntimeContext;
}

SuiteInstaller::InstallModel AppService::InstallingPrereqs()
{
  if (!mInstallingPrereqs)
    mInstallingPrereqs = make<SuiteInstaller::implementation::InstallModel>();

  return mInstallingPrereqs;
}

}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
SuiteInstaller::AppService winrt::SuiteInstaller::factory_implementation::AppService::Singleton()
{
  slim_lock_guard lock{ m_lock };

  if (!mSingleton)
  {
    mSingleton = winrt::make<SuiteInstaller::implementation::AppService>();
  }

  return mSingleton;
}
}  // namespace winrt::SuiteInstaller::factory_implementation
