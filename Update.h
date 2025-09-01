#pragma once

#include "Update.g.h"

namespace winrt::SuiteInstaller::implementation
{
struct Update : UpdateT<Update>
{
  Update() = default;

  hstring Title() const;
  void    Title(hstring const & value);

  hstring Description() const;
  void    Description(hstring const & value);

private:
  hstring mTitle;
  hstring mDescription;
};
}  // namespace winrt::SuiteInstaller::implementation

namespace winrt::SuiteInstaller::factory_implementation
{
struct Update : UpdateT<Update, implementation::Update>
{
};
}  // namespace winrt::SuiteInstaller::factory_implementation
