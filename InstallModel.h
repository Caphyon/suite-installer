#pragma once

#include "InstallModel.g.h"

namespace winrt::SuiteInstaller::implementation
{
struct InstallModel : InstallModelT<InstallModel>
{
  InstallModel();

  void HaveInstall(bool aHaveInstall);
  bool HaveInstall();

  void InvInstallInProgress(bool aHaveInstall);
  bool InvInstallInProgress();

  void AddPrerequisite(SuiteInstaller::Prerequisite const & aPrerequiste);
  void RemovePrerequisite(SuiteInstaller::Prerequisite const & aPrerequiste);
  void RemoveAllPrerequisites();

  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
  Prerequisites();

  //---------------------------------------------------------------------------
  winrt::event_token PropertyChanged(
    Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const & aHandler);
  void PropertyChanged(winrt::event_token const & aToken);
  void OnPropertyChanged(hstring aPropertyName);

private:
  winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> mPropertyChanged;

  bool mHaveInstall{ false };
  bool mInvInstallInProgress{ true };

  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
    mPrerequisites;
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct InstallModel : InstallModelT<InstallModel, implementation::InstallModel>
{
};
}  // namespace winrt::SuiteInstaller::factory_implementation
