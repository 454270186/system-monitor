#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds)
{
  int hh, mm, ss, remainder;
  string time;
  hh = seconds / 3600;
  remainder = seconds - hh * 3600;
  mm = remainder / 60;
  remainder -= mm * 60;
  ss = remainder;
  std::ostringstream elapsed_time;
  elapsed_time << "00" << to_string(hh) << ':' << "00" << to_string(mm) << ':' << "00" << to_string(ss);
  
  return elapsed_time.str();  //return time
}