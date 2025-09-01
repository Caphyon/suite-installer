#include "stdafx.h"
#include "Update.h"
#if __has_include("Update.g.cpp")
#include "Update.g.cpp"
#endif

namespace winrt::SuiteInstaller::implementation
{

hstring Update::Title() const
{
  return mTitle;
}

void Update::Title(hstring const & aValue)
{
  mTitle = aValue;
}

hstring Update::Description() const
{
  return mDescription;
}

void Update::Description(hstring const & aValue)
{
  mDescription = aValue;
}

}  // namespace winrt::SuiteInstaller::implementation
