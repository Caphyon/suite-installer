#include "stdafx.h"
#include "Prerequisite.h"
#if __has_include("Prerequisite.g.cpp")
#include "Prerequisite.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Media;

namespace
{
constexpr const wchar_t * const kInstalledProp          = L"Installed";
constexpr const wchar_t * const kTitleProp              = L"Title";
constexpr const wchar_t * const kDescriptionProp        = L"Description";
constexpr const wchar_t * const kPublisherProp          = L"Publisher";
constexpr const wchar_t * const kCategoryProp           = L"Category";
constexpr const wchar_t * const kAdditionalTermsProp    = L"AdditionalTerms";
constexpr const wchar_t * const kCurrentActionProp      = L"CurrentAction";
constexpr const wchar_t * const kProgressValueProp      = L"ProgressValue";
constexpr const wchar_t * const kProgressRangeProp      = L"TotalRange";
constexpr const wchar_t * const kInstallInProgressProp  = L"InstallInProgress";
constexpr const wchar_t * const kDownloadInProgressProp = L"DownloadInProgress";
constexpr const wchar_t * const kWaitingForInstallProp  = L"WaitingForInstall";
constexpr const wchar_t * const kHasOpenPathProp        = L"HasOpenPath";

}  // namespace

namespace winrt::SuiteInstaller::implementation
{
Prerequisite::Prerequisite(
  hstring                                                                   aId,
  hstring                                                                   aTitle,
  hstring                                                                   aDescription,
  hstring                                                                   aPublisher,
  hstring                                                                   aCategory,
  hstring                                                                   aAdditionalInformation,
  bool                                                                      aInstalled,
  Imaging::BitmapImage                                                      aIcon,
  Windows::Foundation::Collections::IObservableVector<Imaging::BitmapImage> aImages)
  : mId(aId)
  , mTitle(aTitle)
  , mDescription(aDescription)
  , mPublisher(aPublisher)
  , mCategory(aCategory)
  , mAdditionalTerms(aAdditionalInformation)
  , mInstalled(aInstalled)
  , mIcon(aIcon)
  , mImages(aImages)
  , mHasOpenPath(false)
{
}

hstring Prerequisite::Id() const
{
  return mId;
}

hstring Prerequisite::Title() const
{
  return mTitle;
}

void Prerequisite::Title(hstring const & aValue)
{
  mTitle = aValue;
  OnPropertyChanged(kTitleProp);
}

hstring Prerequisite::Description() const
{
  return mDescription;
}

void Prerequisite::Description(hstring const & aValue)
{
  mDescription = aValue;
  OnPropertyChanged(kDescriptionProp);
}

hstring Prerequisite::Publisher() const
{
  return mPublisher;
}

void Prerequisite::Publisher(hstring const & aValue)
{
  mPublisher = aValue;
  OnPropertyChanged(kPublisherProp);
}

hstring Prerequisite::Category() const
{
  return mCategory;
}

void Prerequisite::Category(hstring const & aValue)
{
  mCategory = aValue;
  OnPropertyChanged(kCategoryProp);
}

hstring Prerequisite::AdditionalTerms() const
{
  return mAdditionalTerms;
}

void Prerequisite::AdditionalTerms(hstring const & aValue)
{
  mAdditionalTerms = aValue;
  OnPropertyChanged(kAdditionalTermsProp);
}

bool Prerequisite::Installed()
{
  return mInstalled;
}

void Prerequisite::Installed(bool aInstalled)
{
  mInstalled = aInstalled;

  OnPropertyChanged(kInstalledProp);
}

hstring Prerequisite::OpenPath() const
{
  return mOpenPath;
}

void Prerequisite::OpenPath(hstring const & aValue)
{
  mOpenPath = aValue;
  HasOpenPath(!aValue.empty());
}

bool Prerequisite::HasOpenPath()
{
  return mHasOpenPath;
}

void Prerequisite::HasOpenPath(bool aHasOpenPath)
{
  mHasOpenPath = aHasOpenPath;

  OnPropertyChanged(kHasOpenPathProp);
}

hstring Prerequisite::CurrentAction() const
{
  return mCurrentAction;
}

void Prerequisite::CurrentAction(hstring const & aValue)
{
  mCurrentAction = aValue;

  OnPropertyChanged(kCurrentActionProp);
}

int Prerequisite::ProgressValue() const
{
  return mProgressValue;
}

void Prerequisite::ProgressValue(int const & aValue)
{
  mProgressValue = aValue;

  OnPropertyChanged(kProgressValueProp);
}

int Prerequisite::TotalRange() const
{
  return mTotalRange;
}

void Prerequisite::TotalRange(int const & aValue)
{
  mTotalRange = aValue;

  OnPropertyChanged(kProgressRangeProp);
}

bool Prerequisite::InstallInProgress()
{
  return mInstallInProgress;
}

void Prerequisite::InstallInProgress(bool aInstallInProgress)
{
  mInstallInProgress = aInstallInProgress;

  OnPropertyChanged(kInstallInProgressProp);
}

bool Prerequisite::DownloadInProgress()
{
  return mDownloadInProgress;
}

void Prerequisite::DownloadInProgress(bool aValue)
{
  mDownloadInProgress = aValue;

  OnPropertyChanged(kDownloadInProgressProp);
}

bool Prerequisite::WaitingForInstall()
{
  return mWaitingForInstall || mInstalled;
}

void Prerequisite::WaitingForInstall(bool aWaitingForInstall)
{
  mWaitingForInstall = aWaitingForInstall;

  OnPropertyChanged(kWaitingForInstallProp);
}

void Prerequisite::ShowRemoveBtn(bool aShowRemoveBtn)
{
  if (mShowRemoveBtn != aShowRemoveBtn)
  {
    mShowRemoveBtn = aShowRemoveBtn;
    OnPropertyChanged(L"ShowRemoveBtn");
  }
}

bool Prerequisite::ShowRemoveBtn()
{
  return mShowRemoveBtn;
}

Windows::Foundation::Collections::IObservableVector<Imaging::BitmapImage> Prerequisite::Images()
{
  return mImages;
}

Imaging::BitmapImage Prerequisite::Icon()
{
  return mIcon;
}

winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage Prerequisite::CarouselImage()
{
  return *mImages.First();
}

winrt::event_token Prerequisite::PropertyChanged(
  Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const & aHandler)
{
  return mPropertyChanged.add(aHandler);
}

void Prerequisite::PropertyChanged(winrt::event_token const & aToken)
{
  mPropertyChanged.remove(aToken);
}

void Prerequisite::OnPropertyChanged(hstring aPropertyName)
{
  mPropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ aPropertyName });
}

}  // namespace winrt::SuiteInstaller::implementation
