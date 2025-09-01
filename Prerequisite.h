#pragma once

#include "Prerequisite.g.h"

namespace winrt::SuiteInstaller::implementation
{
struct Prerequisite : PrerequisiteT<Prerequisite>
{
  Prerequisite(hstring                                                 aId,
               hstring                                                 aTitle,
               hstring                                                 aDescription,
               hstring                                                 aPublisher,
               hstring                                                 aCategory,
               hstring                                                 aAdditionalInformation,
               bool                                                    aInstalled,
               winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage aIcon,
               Windows::Foundation::Collections::IObservableVector<
                 winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage> aImages);

  hstring Id() const;

  hstring Title() const;
  void    Title(hstring const & value);

  hstring Description() const;
  void    Description(hstring const & value);

  hstring Publisher() const;
  void    Publisher(hstring const & value);

  hstring Category() const;
  void    Category(hstring const & value);

  hstring AdditionalTerms() const;
  void    AdditionalTerms(hstring const & value);

  bool Installed();
  void Installed(bool aInstalled);

  hstring OpenPath() const;
  void    OpenPath(hstring const & value);

  bool HasOpenPath();
  void HasOpenPath(bool aHasOpenPath);

  hstring CurrentAction() const;
  void    CurrentAction(hstring const & value);

  int  ProgressValue() const;
  void ProgressValue(int const & aValue);

  int  TotalRange() const;
  void TotalRange(int const & aValue);

  bool InstallInProgress();
  void InstallInProgress(bool aInstallInProgress);

  bool DownloadInProgress();
  void DownloadInProgress(bool aValue);

  bool WaitingForInstall();
  void WaitingForInstall(bool aWaitingForInstall);

  bool ShowRemoveBtn();
  void ShowRemoveBtn(bool aShowRemoveBtn);

  Windows::Foundation::Collections::IObservableVector<
    winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage>
  Images();

  winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage Icon();
  winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage CarouselImage();

  //---------------------------------------------------------------------------
  winrt::event_token PropertyChanged(
    Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const & aHandler);
  void PropertyChanged(winrt::event_token const & aToken);
  void OnPropertyChanged(hstring aPropertyName);

private:
  winrt::event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> mPropertyChanged;

  hstring mId;
  hstring mTitle;
  hstring mDescription;
  hstring mPublisher;
  hstring mCategory;
  hstring mAdditionalTerms;
  bool    mInstalled;
  bool    mHasOpenPath;
  hstring mOpenPath;
  hstring mCurrentAction;
  int     mProgressValue;
  int     mTotalRange;

  bool mInstallInProgress{ false };
  bool mDownloadInProgress{ false };
  bool mWaitingForInstall{ false };

  bool mShowRemoveBtn{ true };

  winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage mIcon;
  Windows::Foundation::Collections::IObservableVector<
    winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage>
    mImages;
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct Prerequisite : PrerequisiteT<Prerequisite, implementation::Prerequisite>
{
};
}  // namespace winrt::SuiteInstaller::factory_implementation
