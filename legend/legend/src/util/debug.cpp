#include "src/util/debug.h"

#include "src/util/string_util.h"

namespace legend {
namespace util {
namespace debug {

//アサーション
void Assertion(bool expr, const std::wstring& format, ...) {
  va_list args;
  va_start(args, &format);
  const std::wstring msg = string_util::Format(format + L"\n", args);
  va_end(args);
  _ASSERT_EXPR(expr, msg.c_str());
}

void Log(const std::wstring& format, ...) {
  va_list args;
  va_start(args, &format);
  const std::wstring msg = string_util::Format(format + L"\n", args);
  va_end(args);
  OutputDebugStringW(msg.c_str());
}

}  // namespace debug
}  // namespace util
}  // namespace legend
