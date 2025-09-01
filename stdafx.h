#pragma once

#define __AFXWIN_H__

#include <atlstr.h>
#include <hstring.h>
#include <msidefs.h>
#include <msiquery.h>
#include <restrictederrorinfo.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <unknwn.h>
#include <wincodec.h>
#include <windows.h>

#define _COM_NO_STANDARD_GUIDS_  // disable hard-coded smart pointer DEFs
#include <comdef.h>
#include <comdefsp.h>
#include <commctrl.h>

// #define XML_UNICODE_WCHAR_T

// std
#include <algorithm>
#include <coroutine>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <ranges>
#include <regex>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Undefine GetCurrentTime macro to prevent
// conflict with Storyboard::GetCurrentTime
#undef GetCurrentTime

// clang-format off
#include <winrt/base.h>
#include <wil/cppwinrt_helpers.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.Xaml.Media.Animation.h>

#include <Microsoft.UI.Xaml.Window.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Controls.Primitives.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Data.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <winrt/Windows.Storage.Streams.h>
