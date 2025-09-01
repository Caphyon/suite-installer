#include "stdafx.h"
#include "Utils.h"
#include <winrt/Windows.Storage.Streams.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;
using namespace winrt::Microsoft::UI::Xaml::Media::Imaging;
using namespace winrt::Windows::Storage::Streams;

std::filesystem::path GetModulePath()
{
  // Buffer to store the value
  wchar_t buffer[1024];

  DWORD result = GetModuleFileNameW(nullptr, buffer, sizeof(buffer) / sizeof(wchar_t));
  result;

  return buffer;
}

std::vector<std::wstring> GetIniFileSectionNames(std::wstring_view ainiFilePath)
{
  // Buffer to store all section names
  wchar_t buffer[1024];  // Adjust size according to your needs

  // Retrieve all section names from the INI file
  DWORD result = GetPrivateProfileSectionNamesW(
    buffer,                            // Buffer to store section names
    sizeof(buffer) / sizeof(wchar_t),  // Size of the buffer (in wchar_t, not bytes)
    ainiFilePath.data()                // Path to the INI file
  );

  // Vector to store section names
  std::vector<std::wstring> sectionNames;

  // Check if the function was successful
  if (result > 0 && result < sizeof(buffer) / sizeof(wchar_t))
  {
    // Iterate through the buffer, which contains null-terminated strings
    wchar_t * section = buffer;
    while (*section)
    {
      // Add section name to the vector
      sectionNames.emplace_back(section);
      section += wcslen(section) + 1;  // Move to the next section name
    }
  }
  else
  {
    std::wcerr << L"Failed to retrieve section names or buffer is too small." << std::endl;
  }

  return sectionNames;
}

std::vector<std::wstring> GetKeysInSection(std::wstring_view aSection, std::wstring_view aIniPath)
{
  std::vector<std::wstring> keys;
  wchar_t                   buffer[2048];  // Buffer to hold keys
  DWORD charsRead = GetPrivateProfileStringW(aSection.data(), nullptr, L"", buffer, sizeof(buffer),
                                             aIniPath.data());

  if (charsRead > 0)
  {
    wchar_t * key = buffer;
    while (*key)
    {
      keys.push_back(key);
      key += wcslen(key) + 1;  // Move to the next key
    }
  }
  return keys;
}

std::wstring GetIniValue(std::wstring_view aIniFilePath,
                         std::wstring_view aSection,
                         std::wstring_view aKey)
{
  wchar_t buffer[1024];

  GetPrivateProfileString(aSection.data(), aKey.data(), nullptr, buffer,
                          sizeof(buffer) / sizeof(wchar_t), aIniFilePath.data());

  return buffer;
}

void LaunchProcessAndWait(const std::wstring & aCommand)
{
  PROCESS_INFORMATION pi{};
  STARTUPINFO         si{};
  si.cb = sizeof(si);

  // Create the process
  if (!CreateProcess(NULL, const_cast<wchar_t *>(aCommand.c_str()), NULL, NULL, TRUE, 0, NULL, NULL,
                     &si, &pi))
  {
    std::cerr << "CreateProcess failed (" << GetLastError() << ")\n";
    return;
  }

  // Wait for the process to finish
  WaitForSingleObject(pi.hProcess, INFINITE);

  // Close handles
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

HICON ExtractLargestIconFromExe(const std::wstring & exePath)
{
  UINT iconCount = ExtractIconExW(exePath.c_str(), -1, nullptr, nullptr, 0);
  if (iconCount == 0)
  {
    return nullptr;
  }

  std::vector<HICON> largeIcons(iconCount);
  std::vector<HICON> smallIcons(iconCount);

  // Extract all icons
  ExtractIconExW(exePath.c_str(), 0, largeIcons.data(), smallIcons.data(), iconCount);

  HICON largestIcon = largeIcons[iconCount - 1];

  for (UINT i = 0; i < iconCount - 1; i++)
  {
    DestroyIcon(largeIcons[i]);
    DestroyIcon(smallIcons[i]);
  }

  return largestIcon;
}

winrt::Windows::Foundation::IAsyncOperation<BitmapImage> ConvertHIconToBitmapImageAsync(HICON hIcon)
{
  winrt::apartment_context callingThread;

  co_await winrt::resume_background();

  ComPtr<IWICImagingFactory> wicFactory;
  HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
                                IID_PPV_ARGS(wicFactory.GetAddressOf()));

  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }

  ComPtr<IWICBitmap> wicBitmap;
  hr = wicFactory->CreateBitmapFromHICON(hIcon, wicBitmap.GetAddressOf());
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }

  ComPtr<IStream> memStream;
  hr = CreateStreamOnHGlobal(nullptr, TRUE, memStream.GetAddressOf());
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }

  ComPtr<IWICBitmapEncoder> encoder;
  hr = wicFactory->CreateEncoder(GUID_ContainerFormatPng, nullptr, encoder.GetAddressOf());
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }
  hr = encoder->Initialize(memStream.Get(), WICBitmapEncoderNoCache);
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }

  ComPtr<IWICBitmapFrameEncode> frame;
  hr = encoder->CreateNewFrame(frame.GetAddressOf(), nullptr);
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }
  hr = frame->Initialize(nullptr);

  UINT width = 0, height = 0;
  hr = wicBitmap->GetSize(&width, &height);
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }
  hr = frame->SetSize(width, height);
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }

  WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;
  hr                        = frame->SetPixelFormat(&format);
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }

  hr = frame->WriteSource(wicBitmap.Get(), nullptr);
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }
  hr = frame->Commit();
  hr = encoder->Commit();
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }

  LARGE_INTEGER zero = {};
  memStream->Seek(zero, STREAM_SEEK_SET, nullptr);

  STATSTG stat;
  hr = memStream->Stat(&stat, STATFLAG_NONAME);
  if (FAILED(hr))
  {
    co_return BitmapImage{};
  }
  const ULONG       size = stat.cbSize.LowPart;
  std::vector<BYTE> buffer(size);
  ULONG             bytesRead = 0;
  hr                          = memStream->Read(buffer.data(), size, &bytesRead);
  if (FAILED(hr) || bytesRead != size)
  {
    co_return BitmapImage{};
  }

  InMemoryRandomAccessStream randomStream;
  {
    DataWriter writer(randomStream.GetOutputStreamAt(0));
    writer.WriteBytes(buffer);
    co_await writer.StoreAsync();
    co_await writer.FlushAsync();
  }
  randomStream.Seek(0);

  co_await callingThread;
  BitmapImage bitmapImage;
  co_await bitmapImage.SetSourceAsync(randomStream);

  DestroyIcon(hIcon);

  co_return bitmapImage;
}
