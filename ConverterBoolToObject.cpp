#include "stdafx.h"
#include "ConverterBoolToObject.h"
#if __has_include("ConverterBoolToObject.g.cpp")
#include "ConverterBoolToObject.g.cpp"
#endif

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;

namespace winrt::SuiteInstaller::implementation
{
IInspectable ConverterBoolToObject::TrueValue()
{
  return mTrueValue;
}
void ConverterBoolToObject::TrueValue(IInspectable aValue)
{
  mTrueValue = aValue;
}
IInspectable ConverterBoolToObject::FalseValue()
{
  return mFalseValue;
}
void ConverterBoolToObject::FalseValue(IInspectable aValue)
{
  mFalseValue = aValue;
}

IInspectable ConverterBoolToObject::Convert(
  IInspectable const & aValue,
  winrt::Windows::UI::Xaml::Interop::TypeName const & /*aTargetType*/,
  IInspectable const & /*aParameter*/,
  hstring const & /*aPanguage*/)
{
  try
  {
    bool boolVal = winrt::unbox_value<bool>(aValue);
    return boolVal ? mTrueValue : mFalseValue;
  }
  catch (...)
  {
  }

  try
  {
    int intVal = winrt::unbox_value<int>(aValue);
    return (intVal == 0) ? mTrueValue : mFalseValue;
  }
  catch (...)
  {
  }

  return mFalseValue;
}
IInspectable ConverterBoolToObject::ConvertBack(
  IInspectable const & /*value*/,
  winrt::Windows::UI::Xaml::Interop::TypeName const & /*targetType*/,
  IInspectable const & /*parameter*/,
  hstring const & /*language*/)
{
  return nullptr;
}

}  // namespace winrt::SuiteInstaller::implementation
