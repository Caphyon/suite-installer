#include "stdafx.h"
#include "InstallModel.h"
#if __has_include("InstallModel.g.cpp")
#include "InstallModel.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::SuiteInstaller::implementation
{
InstallModel::InstallModel()
{
  mPrerequisites = winrt::multi_threaded_observable_vector<SuiteInstaller::Prerequisite>();
}

void InstallModel::HaveInstall(bool aHaveInstall)
{
  if (mHaveInstall != aHaveInstall)
  {
    mHaveInstall = aHaveInstall;
    OnPropertyChanged(L"HaveInstall");
  }
}

bool InstallModel::HaveInstall()
{
  return mHaveInstall;
}

void InstallModel::InvInstallInProgress(bool aInstallInProgress)
{
  if (mInvInstallInProgress != aInstallInProgress)
  {
    mInvInstallInProgress = aInstallInProgress;
    OnPropertyChanged(L"InvInstallInProgress");
  }
}

bool InstallModel::InvInstallInProgress()
{
  return mInvInstallInProgress;
}

void InstallModel::AddPrerequisite(SuiteInstaller::Prerequisite const & aPrerequiste)
{
  mPrerequisites.Append(aPrerequiste);

  HaveInstall(mPrerequisites.Size() > 0);
}

void InstallModel::RemovePrerequisite(SuiteInstaller::Prerequisite const & aPrerequiste)
{
  uint32_t index = 0;
  if (mPrerequisites.IndexOf(aPrerequiste, index))
  {
    mPrerequisites.RemoveAt(index);
    aPrerequiste.ProgressValue(0);
    aPrerequiste.CurrentAction(L"");
    aPrerequiste.DownloadInProgress(false);
    aPrerequiste.WaitingForInstall(false);
  }

  HaveInstall(mPrerequisites.Size() > 0);
}

void InstallModel::RemoveAllPrerequisites()
{
  for (auto prereq : mPrerequisites)
  {
    prereq.ProgressValue(0);
    prereq.CurrentAction(L"");
    prereq.DownloadInProgress(false);
    prereq.WaitingForInstall(false);
  }

  mPrerequisites.Clear();

  HaveInstall(mPrerequisites.Size() > 0);
}

Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
InstallModel::Prerequisites()
{
  return mPrerequisites;
}

winrt::event_token InstallModel::PropertyChanged(
  Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const & aHandler)
{
  return mPropertyChanged.add(aHandler);
}

void InstallModel::PropertyChanged(winrt::event_token const & aToken)
{
  mPropertyChanged.remove(aToken);
}

void InstallModel::OnPropertyChanged(hstring aPropertyName)
{
  mPropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ aPropertyName });
}

}  // namespace winrt::SuiteInstaller::implementation
