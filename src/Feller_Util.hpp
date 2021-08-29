#ifndef INCLUDED_FELLER_UTIL
#define INCLUDED_FELLER_UTIL

#include <chrono>
#include <string>

#include "Feller_Feller.hpp"

namespace Feller
{

namespace Util
{
/**
   time_to_string. Given a time point as input (denoted as `time`), this method
returns a string representation of the `time` variable. This method differs from
other approaches in that it does not rely on string streams or similar. This
method may throw due to allocation failures. Note that this function does not
modify the parameter. \param time: the time to be converted to a string. \return
a string representing the `time` parameter.
**/
std::string time_to_string(const std::chrono::time_point<std::chrono::system_clock> &time);
}  // namespace Util
}  // namespace Feller
#endif
