#include "stdafx.h"
#include "Context.h"
#if __has_include("Context.g.cpp")
#include "Context.g.cpp"
#endif

#include "App.xaml.h"
#include "Prerequisite.h"
#include "Update.h"
#include "util/HandleStream.h"
#include "util/Utils.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Media::Imaging;
using namespace string_view_literals;

namespace
{

constexpr auto kConfigFileName      = L"SuiteInstaller.ini";
constexpr auto kUpdatesInfoFileName = L"UpdatesInfo.ini";

constexpr auto kAppNameProp       = L"ProductName";
constexpr auto kAppIcon           = L"AppIcon";
constexpr auto kPropertiesSection = L"Properties";
constexpr auto kProductCodeKey    = L"ProductCode";

}  // namespace

namespace winrt::SuiteInstaller::implementation
{

Context::Context()
{
  filesystem::path configFilePath = GetModulePath().replace_filename(kConfigFileName);

  LoadPrerequisites(configFilePath);

  mAppName = L"SuiteInstaller";

  mProductCode          = GetIniValue(configFilePath.c_str(), kPropertiesSection, kProductCodeKey);
  mReleaseNotesFilePath = GetIniValue(configFilePath.c_str(), kPropertiesSection, L"ReleaseNotes");
  mExternalUiPath = GetIniValue(configFilePath.c_str(), kPropertiesSection, L"ExternalUiPath");
  mUpdaterPath    = GetIniValue(configFilePath.c_str(), kPropertiesSection, L"UpdaterPath");
  mFeedBackLink   = GetIniValue(configFilePath.c_str(), kPropertiesSection, L"FeedbackLink");
  mHelpLink       = GetIniValue(configFilePath.c_str(), kPropertiesSection, L"HelpLink");
  mAppName        = GetIniValue(configFilePath.c_str(), kPropertiesSection, kAppNameProp);
  mRunAsAdmin     = GetIniValue(configFilePath.c_str(), kPropertiesSection, L"RunAsAdmin") == L"2";
  mAllUsers       = GetIniValue(configFilePath.c_str(), kPropertiesSection, L"AllUsers") == L"1";

  LoadInstalledState();

  LoadReleaseNotes();

  LoadUpdates();
}

void Context::LoadPrerequisites(const filesystem::path & aConfigFilePath)
{
  mPrerequisites = winrt::multi_threaded_observable_vector<SuiteInstaller::Prerequisite>();

  auto prereqList = GetIniFileSectionNames(aConfigFilePath.c_str());

  for (auto prereq : prereqList)
  {
    wcout << prereq;

    // skip Properties section
    if (prereq == kPropertiesSection)
      continue;

    auto keysList = GetKeysInSection(prereq, aConfigFilePath.c_str());

    auto images =
      single_threaded_observable_vector<Microsoft::UI::Xaml::Media::Imaging::BitmapImage>();
    Microsoft::UI::Xaml::Media::Imaging::BitmapImage icon;

    auto prereqData =
      make<Prerequisite>(prereq.c_str(), L"", L"", L"", L"", L"", false, icon, images);
    auto prereqModel = prereqData.as<Prerequisite>();

    prereqModel->Installed(false);  // assume not installed, will be updated later

    for (auto key : keysList)
    {
      if (key == L"Display")
      {
        auto title = GetIniValue(aConfigFilePath.c_str(), prereq, key);
        prereqModel->Title(title.c_str());
      }
      else if (key == L"Description")
      {
        auto value = GetIniValue(aConfigFilePath.c_str(), prereq, key);
        prereqModel->Description(value.c_str());
      }
      else if (key == L"Publisher")
      {
        auto value = GetIniValue(aConfigFilePath.c_str(), prereq, key);
        prereqModel->Publisher(value.c_str());
      }
      else if (key == L"Category")
      {
        auto value = GetIniValue(aConfigFilePath.c_str(), prereq, key);
        prereqModel->Category(value.c_str());
      }
      else if (key == L"AdditionalTerms")
      {
        auto value = GetIniValue(aConfigFilePath.c_str(), prereq, key);
        prereqModel->AdditionalTerms(value.c_str());
      }
      else if (key == L"Icon")
      {
        filesystem::path imgPath = GetIniValue(aConfigFilePath.c_str(), prereq, key);

        if (imgPath.is_relative())
          imgPath = GetModulePath().remove_filename() / imgPath;

        icon.UriSource(winrt::Windows::Foundation::Uri(imgPath.c_str()));
      }
      else if (key == L"Images")
      {
        auto value = GetIniValue(aConfigFilePath.c_str(), prereq, key);
        if (!value.empty())
        {
          const wregex ws_re(L";");

          for (auto it = wsregex_token_iterator(value.begin(), value.end(), ws_re, -1);
               it != wsregex_token_iterator(); ++it)
          {
            filesystem::path imgPath(it->str());
            if (imgPath.is_relative())
              imgPath = GetModulePath().remove_filename() / imgPath;

            images.Append(Microsoft::UI::Xaml::Media::Imaging::BitmapImage(
              Windows::Foundation::Uri(imgPath.c_str())));
          }
        }
      }
    }

    mPrerequisites.Append(prereqData);
  }
}

hstring Context::AppName() const
{
  return mAppName;
}

void Context::AppName(hstring const & aAppName)
{
  mAppName = aAppName;
  OnPropertyChanged(kAppNameProp);
}

winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage Context::AppIcon()
{
  return mAppIcon;
}

void Context::AppIcon(winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage aAppIcon)
{
  mAppIcon = aAppIcon;
  OnPropertyChanged(kAppIcon);
}

Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
Context::Prerequisites()
{
  return mPrerequisites;
}

Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
Context::InstalledPrerequisites()
{
  return mInstalledPrerequisites;
}

Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
Context::UpdateDescriptions()
{
  return mUpdateDescriptions;
}

Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
Context::UpdateFeatures()
{
  return mUpdateFeatures;
}

Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
Context::UpdateEnhancements()
{
  return mUpdateEnhancements;
}

Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
Context::UpdateBugfixes()
{
  return mUpdateBugfixes;
}

Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Update>
Context::AllUpdates()
{
  return mAllUpdates;
}

int32_t Context::PrerequisitesSize()
{
  return mPrerequisites.Size();
}

winrt::event_token Context::PropertyChanged(
  Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const & aHandler)
{
  return mPropertyChanged.add(aHandler);
}

void Context::PropertyChanged(winrt::event_token const & aToken)
{
  mPropertyChanged.remove(aToken);
}
void Context::OnPropertyChanged(hstring aPropertyName)
{
  mPropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ aPropertyName });
}

void Context::MarkPrereqInstalled(const winrt::SuiteInstaller::Prerequisite & aPrereq)
{
  aPrereq.Installed(true);
  InstalledPrerequisites().Append(aPrereq);

  uint32_t idx = 0;
  if (Prerequisites().IndexOf(aPrereq, idx))
    Prerequisites().RemoveAt(idx);
}

winrt::Windows::Foundation::IAsyncOperation<bool> Context::LoadInstalledState()
{
  if (mInstallStateLoaded)
    co_return true;

  winrt::apartment_context callingThread;
  co_await winrt::resume_background();

  wstring resultFile = filesystem::temp_directory_path() / (mProductCode + kConfigFileName);

  LaunchProcessAndWait(mExternalUiPath + LR"( /i )" + mProductCode +
                       LR"( AI_UNINSTALLER_CTP="" LIST_PREREQ=ALL PREREQ_LIST=NONE RESULT_FILE=)" +
                       resultFile);
  auto prereqList = GetIniFileSectionNames(resultFile);

  co_await callingThread;

  for (auto prereqId : prereqList)
  {
    auto installState = GetIniValue(resultFile, prereqId, L"InstallState");
    auto found        = GetIniValue(resultFile, prereqId, L"Found");
    auto openPath     = GetIniValue(resultFile, prereqId, L"OpenPath");

    if (installState == L"Installed")
    {
      for (auto prereqData : Prerequisites())
      {
        if (prereqData.Id() == prereqId)
        {
          MarkPrereqInstalled(prereqData);
          prereqData.OpenPath(openPath);
          break;
        }
      }
    }
  }

  // erase the generated file after loading its info
  filesystem::remove(resultFile);

  mInstallStateLoaded = true;
  OnPropertyChanged(L"InstallStateLoaded");
  co_return true;
}

winrt::Windows::Foundation::IAsyncOperation<bool> Context::ReadUpdatesIni(wstring_view aFilePath)
{
  auto sectionsList = GetIniFileSectionNames(aFilePath);
  ranges::sort(sectionsList);

  sectionsList.erase(ranges::remove(sectionsList, L"General").begin(), sectionsList.end());

  if (sectionsList.empty())
    co_return false;

  const auto prefixAndCollection =
    array{ pair{ L"Description", &mUpdateDescriptions }, pair{ L"Feature", &mUpdateFeatures },
           pair{ L"Enhancement", &mUpdateEnhancements }, pair{ L"BugFix", &mUpdateBugfixes } };
  for (auto & [prefix, collection] : prefixAndCollection)
  {
    if (*collection)
      collection->Clear();
    else
      *collection = winrt::single_threaded_observable_vector<SuiteInstaller::Update>();
  }

  if (mAllUpdates)
    mAllUpdates.Clear();
  else
    mAllUpdates = winrt::single_threaded_observable_vector<SuiteInstaller::Update>();

  auto latestUpdate = *(sectionsList | views::reverse).begin();

  auto keys = GetKeysInSection(latestUpdate, aFilePath);

  auto filterStartingWith = [&keys](const wchar_t * const aPrefix)
  {
    return keys | views::filter(
                    [=](auto key)
                    {
                      return key.starts_with(aPrefix);
                    });
  };

  for (auto & [prefix, collection] : prefixAndCollection)
  {
    for (auto key : filterStartingWith(prefix))
    {
      auto text = GetIniValue(aFilePath, latestUpdate, key);

      auto updateData  = make<Update>();
      auto updateModel = updateData.as<Update>();

      wstring title;
      wstring description;

      // extract the title (part before the :)
      if (size_t pos = text.find(':'); pos != text.npos)
      {
        title       = text.substr(0, pos - 1);
        description = text.substr(pos + 1);

        // capitalize first letter
        description[0] = towupper(description[0]);
      }
      else
      {
        description = text;
      }

      updateModel->Title(title.c_str());

      updateModel->Description(description.c_str());

      collection->Append(updateData);
      mAllUpdates.Append(updateData);
    }
  }
  co_return true;
}

winrt::Windows::Foundation::IAsyncOperation<bool> Context::LoadUpdates()
{
  if (mUpdatesLoaded)
    co_return true;

  if (mUpdaterPath.empty())
    co_return false;

  mUpdatesLoaded = true;
  OnPropertyChanged(L"UpdatesLoaded");

  // Prepend product code to the updates info temp filename to avoid collisions between products.
  auto resultFile = filesystem::temp_directory_path() / (mProductCode + kUpdatesInfoFileName);

  winrt::apartment_context callingThread;
  co_await winrt::resume_background();

  LaunchProcessAndWait(
    std::format(L"{} /justcheck -dumpdetected {}", mUpdaterPath, resultFile.c_str()));

  if (co_await ReadUpdatesIni(resultFile.c_str()))
  {
    mNewUpdate = true;
    OnPropertyChanged(L"NewUpdate");
  }

  // erase the generated file after loading its info
  filesystem::remove(resultFile);

  co_return true;
}

winrt::Windows::Foundation::IAsyncOperation<bool> Context::LoadReleaseNotes()
{
  if (mReleaseNotesLoaded)
    co_return true;

  mReleaseNotesLoaded = true;

  const auto & resultFile = mReleaseNotesFilePath;

  co_await ReadUpdatesIni(resultFile.c_str());

  co_return true;
}

void ReadProgressMessaged(HANDLE                                             aPipe,
                          winrt::Microsoft::UI::Dispatching::DispatcherQueue aDispatcher,
                          bool *                                             aContinueReading)
{
  // Wait for the installer stub (client) to connect.
  if (!ConnectNamedPipe(aPipe, nullptr) && GetLastError() != ERROR_PIPE_CONNECTED)
  {
    wcerr << L"ConnectNamedPipe failed (" << GetLastError() << L")\n";
    return;
  }

  // Wrap the pipe handle into our stream helper.
  HandleStream stream(aPipe);

  auto installmodel = App::AppService().InstallingPrereqs();

  uint32_t downloadingPrereqIdx = 0;
  uint32_t installingPrereqIdx  = downloadingPrereqIdx;
  installingPrereqIdx;
  uint32_t prereqCount = installmodel.Prerequisites().Size();
  uint32_t doneCount   = 0;

  auto prereq        = installmodel.Prerequisites().GetAt(downloadingPrereqIdx);
  auto prereqInstall = installmodel.Prerequisites().GetAt(installingPrereqIdx);

  aDispatcher.TryEnqueue(
    [prereq, installmodel]()
    {
      prereq.DownloadInProgress(true);

      installmodel.InvInstallInProgress(false);
    });

  auto Respond = [&](UINT aRes = IDCANCEL)
  {
    Serialization::Serialize(static_cast<UINT>(aRes), stream);
    stream.flush();
  };

  // Process incoming messages.
  UINT msgId             = 0;
  bool displayActionData = true;
  while (*aContinueReading && Serialization::Deserialize(msgId, stream))
  {
    // Use the remainder to determine the message type.
    bool isDownload = msgId / 10'000 == 1;
    switch (msgId % 10000)
    {
    case 3004:  // Action data message.
    {
      wstring msg;
      if (!Serialization::Deserialize(msg, stream))
      {
        *aContinueReading = false;
        break;
      }

      if (!installmodel.HaveInstall())
        break;

      if (!displayActionData)
        break;

      if (isDownload)
      {
        if (msg.ends_with(L"completed"))
        {
          ++downloadingPrereqIdx;

          aDispatcher.TryEnqueue(
            [prereq]()
            {
              prereq.DownloadInProgress(false);
              prereq.CurrentAction(L"Downloaded");
            });

          if (prereqCount <= downloadingPrereqIdx)
            break;

          auto range = prereq.TotalRange();
          prereq     = installmodel.Prerequisites().GetAt(downloadingPrereqIdx - doneCount);
          aDispatcher.TryEnqueue(
            [prereq, msg, range]()
            {
              prereq.DownloadInProgress(true);
              prereq.ProgressValue(0);
              prereq.TotalRange(range);
            });
        }
        else
        {
          aDispatcher.TryEnqueue(
            [prereq, msg]()
            {
              prereq.CurrentAction(msg);
            });
        }
      }
      else
      {
        // advance installing
        bool completed = msg.ends_with(L"completed");
        if (completed)
        {
          ++installingPrereqIdx;

          aDispatcher.TryEnqueue(
            [prereqInstall, installmodel, &doneCount]()
            {
              prereqInstall.Installed(true);
              doneCount++;

              installmodel.RemovePrerequisite(prereqInstall);
            });

          if (prereqCount <= installingPrereqIdx)
          {
            displayActionData = false;
            break;
          }

          prereqInstall = installmodel.Prerequisites().GetAt(installingPrereqIdx - doneCount);
        }
        else
        {
          aDispatcher.TryEnqueue(
            [prereqInstall, msg]()
            {
              prereqInstall.CurrentAction(msg);
              prereqInstall.DownloadInProgress(false);
              prereqInstall.InstallInProgress(true);
            });
        }
      }
      break;
    }
    case 5008:  // Reset progress range.
    {
      UINT value;
      if (!Serialization::Deserialize(value, stream))
      {
        *aContinueReading = false;
        break;
      }

      if (!installmodel.HaveInstall())
        return Respond();

      if (!isDownload)
        break;

      aDispatcher.TryEnqueue(
        [prereq, installmodel, value, prereqCount]()
        {
          prereq.ProgressValue(0);
          prereq.TotalRange(value / prereqCount);
        });

      break;
    }
    case 5010:  // Progress update.
    {
      UINT value;
      if (!Serialization::Deserialize(value, stream))
      {
        *aContinueReading = false;
        break;
      }

      if (!installmodel.HaveInstall())
        break;

      if (isDownload)
      {
        aDispatcher.TryEnqueue(
          [prereq, value]()
          {
            auto currentProgress = prereq.ProgressValue();
            auto totalRange      = prereq.TotalRange();
            if (totalRange == 0)
              return;
            currentProgress += value;
            if (currentProgress > totalRange)
              currentProgress = totalRange;
            prereq.ProgressValue(currentProgress);
          });
      }
      else
      {
        // advance installing progress ticks only once per app, so the installing current installing
        // app can be advanced from action data.
        value;
      }
      break;
    }
    default:
    {
      Respond();
      *aContinueReading = false;

      break;
    }
    }

    if (*aContinueReading)
    {
      Respond(IDOK);
    }
  }
}

winrt::Windows::Foundation::IAsyncOperation<bool> Context::InstallPackages(
  winrt::Microsoft::UI::Dispatching::DispatcherQueue aDispatcher)
{
  winrt::apartment_context callingThread;
  auto                     installmodel = App::AppService().InstallingPrereqs();

  wstring prereqs;
  for (auto prereq : installmodel.Prerequisites())
  {
    if (!prereqs.empty())
      prereqs += '|';

    prereqs += prereq.Id();

    prereq.DownloadInProgress(false);
    prereq.InstallInProgress(false);

    // [TODO] this could be a shared property for all prerequisites
    prereq.ShowRemoveBtn(false);
  }

  co_await winrt::resume_background();

  // Set up the process startup info.
  PROCESS_INFORMATION pi{};
  STARTUPINFO         si{};
  si.cb = sizeof(si);

  wstring resultFile = filesystem::temp_directory_path() / (mProductCode + kConfigFileName);

  // Build pipe name and command-line.
  wstring pipeName = LR"(\\.\pipe\PackagesProgress)" + mProductCode;
  wstring command  = std::format(
    LR"(/i {} /qn AI_PARALLEL_DOWNLOAD=1 AI_UNINSTALLER_CTP="" )"
    LR"(PREREQ_LIST="{}" LIST_PREREQ="{}" RESULT_FILE="{}" REINSTALLMODE=ecmus PREREQ_PROGRESS_PIPE_NAME={} {})",
    mProductCode, prereqs, prereqs, resultFile, pipeName,
    (mRunAsAdmin || !mAllUsers)
       ? L"REINSTALL=Config"
       : L"");  // reinstall Config feature only if the silent installation has necesary priviledges

  // Create the installer process. Use ShellExecute to elevate privileges if needed.

  SHELLEXECUTEINFO sei = {
    .cbSize = sizeof(SHELLEXECUTEINFO),
    .fMask  = SEE_MASK_NOCLOSEPROCESS,
    .lpVerb = mRunAsAdmin ? L"runas" : nullptr,  // Use "runas" to elevate privileges if needed.
    .lpFile = mExternalUiPath.c_str(),
    .lpParameters = command.c_str(),
    .nShow        = SW_SHOWNORMAL,
  };

  auto readdCloseBtn = [installmodel]
  {
    for (auto prereq : installmodel.Prerequisites())
    {
      prereq.ShowRemoveBtn(true);
    }
  };

  if (!ShellExecuteEx(&sei))
  {
    DWORD err = GetLastError();
    OutputDebugString(std::format(L"ShellExecuteEx failed with error {}\n", err).c_str());

    aDispatcher.TryEnqueue(readdCloseBtn);

    co_return false;
  }

  auto closeProcHandles = make_scope_guard(CloseHandle(sei.hProcess));

  // Create a named pipe for progress messages.
  HANDLE pipe = CreateNamedPipe(pipeName.c_str(), PIPE_ACCESS_DUPLEX,
                                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                PIPE_UNLIMITED_INSTANCES, 1024, 1024, 5000, nullptr);

  if (pipe == INVALID_HANDLE_VALUE)
  {
    OutputDebugString(std::format(L"CreateNamedPipe failed ({})\n", GetLastError()).c_str());

    aDispatcher.TryEnqueue(readdCloseBtn);

    co_return false;
  }

  auto closePipe = make_scope_guard(CloseHandle(pipe));

  bool continueReading = true;

  // Read progress messages from the installer stub.
  thread th(ReadProgressMessaged, pipe, aDispatcher, &continueReading);

  WaitForSingleObject(sei.hProcess, INFINITE);

  DWORD exitCode;
  if (GetExitCodeProcess(sei.hProcess, &exitCode))
  {
    printf("Process exited with code %lu\n", exitCode);
  }
  else
  {
    printf("Failed to get exit code: %lu\n", GetLastError());
  }

  continueReading = false;
  th.join();

  auto prereqList = GetIniFileSectionNames(resultFile);

  for (auto prereqId : prereqList)
  {
    auto installState = GetIniValue(resultFile, prereqId, L"InstallState");
    auto openPath     = GetIniValue(resultFile, prereqId, L"OpenPath");

    if (installState == L"Installed")
    {
      aDispatcher.TryEnqueue(
        [openPath, prereqId, this]()
        {
          for (auto prereqData : App::AppService().RuntimeContext().Prerequisites())
          {
            if (prereqData.Id() == prereqId)
            {
              MarkPrereqInstalled(prereqData);
              prereqData.OpenPath(openPath);
              break;
            }
          }
        });
    }
  }

  // erase the generated file after loading its info
  filesystem::remove(resultFile);

  aDispatcher.TryEnqueue(
    [installmodel, this, aDispatcher]()
    {
      if (installmodel.Prerequisites().Size() > 0)
      {
        InstallPackages(aDispatcher);
      }
      else
      {
        installmodel.InvInstallInProgress(true);
      }
    });

  co_return true;
}

Windows::Foundation::Collections::IObservableVector<winrt::SuiteInstaller::Prerequisite>
Context::GetSearchList(hstring aQuery)
{
  wstring query = aQuery.c_str();
  transform(query.begin(), query.end(), query.begin(), ::towlower);

  auto filterPrerequisites = [&](auto const & prerequisites, auto & result)
  {
    for (auto const & prereq : prerequisites)
    {
      wstring title = prereq.Title().c_str();
      transform(title.begin(), title.end(), title.begin(), ::towlower);

      if (title.find(query) != wstring::npos)
      {
        result.Append(prereq);
      }
    }
  };

  auto searchedPrereqs = winrt::single_threaded_observable_vector<SuiteInstaller::Prerequisite>();

  filterPrerequisites(mPrerequisites, searchedPrereqs);
  filterPrerequisites(mInstalledPrerequisites, searchedPrereqs);

  return searchedPrereqs;
}

bool Context::UpdatesLoaded()
{
  return mUpdatesLoaded;
}

bool Context::InstallStateLoaded()
{
  return mInstallStateLoaded;
}

bool Context::NewUpdate()
{
  return mNewUpdate;
}

hstring Context::FeedbackLink() const
{
  return mFeedBackLink;
}
hstring Context::HelpLink() const
{
  return mHelpLink;
}

void Context::LaunchUpdater()
{
  if (mUpdaterPath.empty())
    return;

  auto hInst = (INT_PTR)::ShellExecute(0, 0, mUpdaterPath.c_str(), 0, 0, SW_SHOWNORMAL);
  if (hInst <= 32)
  {
    OutputDebugString(
      std::format(L"Failed to launch updater. ShellExecute Return Code: {}, Last Error: {}\n",
                  hInst, GetLastError())
        .c_str());
  }
}
}  // namespace winrt::SuiteInstaller::implementation
