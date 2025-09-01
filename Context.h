#pragma once

#include "Context.g.h"

namespace winrt::SuiteInstaller::implementation
{
struct Context : ContextT<Context>
{
  Context();

  // Application

  hstring AppName() const;
  void    AppName(hstring const & value);

  hstring FeedbackLink() const;
  hstring HelpLink() const;

  winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage AppIcon();
  void AppIcon(winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage aAppIcon);

  // Prerequisites
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
  Prerequisites();

  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
  InstalledPrerequisites();

  // Updates
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
  UpdateDescriptions();
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
  UpdateFeatures();
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
  UpdateEnhancements();
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
  UpdateBugfixes();
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update> AllUpdates();

  int32_t PrerequisitesSize();

  bool UpdatesLoaded();
  bool InstallStateLoaded();
  bool NewUpdate(); // accessor for mNewUpdate

  //------------------------------------------------------------------------------------------
  winrt::event_token PropertyChanged(
    Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const & aHandler);
  void PropertyChanged(winrt::event_token const & aToken);
  void OnPropertyChanged(hstring aPropertyName);

  winrt::Windows::Foundation::IAsyncOperation<bool> LoadInstalledState();

  winrt::Windows::Foundation::IAsyncOperation<bool> ReadUpdatesIni(wstring_view aFilePath);

  winrt::Windows::Foundation::IAsyncOperation<bool> LoadUpdates();

  winrt::Windows::Foundation::IAsyncOperation<bool> LoadReleaseNotes();

  winrt::Windows::Foundation::IAsyncOperation<bool> InstallPackages(
    winrt::Microsoft::UI::Dispatching::DispatcherQueue aDispatcher);

  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
  GetSearchList(hstring aQuerry);

  void LaunchUpdater(); // launch updater exe without waiting or params

private:
  winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> mPropertyChanged;

  hstring                                                 mAppName;
  winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage mAppIcon;

  std::wstring mProductCode;
  std::wstring mReleaseNotesFilePath;
  std::wstring mExternalUiPath;
  std::wstring mUpdaterPath;
  hstring      mFeedBackLink;
  hstring      mHelpLink;

  bool mInstallStateLoaded{ false };
  bool mUpdatesLoaded{ false };
  bool mNewUpdate{ false };
  bool mReleaseNotesLoaded{ false };
  bool mRunAsAdmin{ false };
  bool mAllUsers{ false };

  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
    mPrerequisites;
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
    mInstalledPrerequisites =
      winrt::multi_threaded_observable_vector<SuiteInstaller::Prerequisite>();

  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
    mUpdateDescriptions;
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
    mUpdateFeatures;
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
    mUpdateEnhancements;
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
    mUpdateBugfixes;
  Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update> mAllUpdates;

  void LoadPrerequisites(const std::filesystem::path & aConfigFilePah);

  void MarkPrereqInstalled(const winrt::SuiteInstaller::Prerequisite & aPrereq);
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct Context : ContextT<Context, implementation::Context>
{
};
}  // namespace winrt::SuiteInstaller::factory_implementation
