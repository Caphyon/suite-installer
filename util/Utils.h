#pragma once

template <class T>
[[nodiscard]] auto scope_guard(T fct) noexcept
{
  auto wrap = [f = std::move(fct)](void *)
  {
    f();
  };

  return std::unique_ptr<void, decltype(wrap)>(&wrap, std::move(wrap));
}

#define make_scope_guard(...) \
  scope_guard(                \
    [&]                       \
    {                         \
      __VA_ARGS__;            \
    });

std::filesystem::path GetModulePath();

std::vector<std::wstring> GetIniFileSectionNames(std::wstring_view aIniFilePath);

std::vector<std::wstring> GetKeysInSection(std::wstring_view aSection, std::wstring_view aIniPath);

std::wstring GetIniValue(std::wstring_view aIniFilePath,
                         std::wstring_view aSection,
                         std::wstring_view aKey);

void LaunchProcessAndWait(const std::wstring & aCommand);

HICON ExtractLargestIconFromExe(const std::wstring & exePath);

winrt::Windows::Foundation::IAsyncOperation<winrt::Microsoft::UI::Xaml::Media::Imaging::BitmapImage>
ConvertHIconToBitmapImageAsync(HICON hIcon);
