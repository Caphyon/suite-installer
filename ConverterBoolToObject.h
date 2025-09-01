#pragma once

#include "ConverterBoolToObject.g.h"

namespace winrt::SuiteInstaller::implementation
{
struct ConverterBoolToObject : ConverterBoolToObjectT<ConverterBoolToObject>
{
  inline static Microsoft::UI::Xaml::DependencyProperty kTrueValueProperty =
    Microsoft::UI::Xaml::DependencyProperty::Register(
      L"TrueValue",
      xaml_typename<Windows::Foundation::IInspectable>(),
      xaml_typename<SuiteInstaller::ConverterBoolToObject>(),
      Microsoft::UI::Xaml::PropertyMetadata{ nullptr, nullptr });

  inline static Microsoft::UI::Xaml::DependencyProperty kFalseValueProperty =
    Microsoft::UI::Xaml::DependencyProperty::Register(
      L"FalseValue",
      xaml_typename<Windows::Foundation::IInspectable>(),
      xaml_typename<SuiteInstaller::ConverterBoolToObject>(),
      Microsoft::UI::Xaml::PropertyMetadata{ nullptr, nullptr });

  inline static Microsoft::UI::Xaml::DependencyProperty TrueValueProperty()
  {
    return kTrueValueProperty;
  }

  inline static Microsoft::UI::Xaml::DependencyProperty FalseValueProperty()
  {
    return kFalseValueProperty;
  }

  ConverterBoolToObject() = default;

  Windows::Foundation::IInspectable TrueValue();
  void                              TrueValue(Windows::Foundation::IInspectable aValue);

  Windows::Foundation::IInspectable FalseValue();
  void                              FalseValue(Windows::Foundation::IInspectable aValue);

  Windows::Foundation::IInspectable Convert(
    Windows::Foundation::IInspectable const &           aValue,
    winrt::Windows::UI::Xaml::Interop::TypeName const & aTargetType,
    Windows::Foundation::IInspectable const &           aParameter,
    hstring const &                                     aLanguage);
  Windows::Foundation::IInspectable ConvertBack(
    Windows::Foundation::IInspectable const &           aValue,
    winrt::Windows::UI::Xaml::Interop::TypeName const & aTargetType,
    Windows::Foundation::IInspectable const &           aTarameter,
    hstring const &                                     aLanguage);

private:
  Windows::Foundation::IInspectable mTrueValue;
  Windows::Foundation::IInspectable mFalseValue;
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct ConverterBoolToObject
  : ConverterBoolToObjectT<ConverterBoolToObject, implementation::ConverterBoolToObject>
{
};
}  // namespace winrt::SuiteInstaller::factory_implementation
