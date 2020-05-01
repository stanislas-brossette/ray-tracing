#pragma once

struct StringException : public std::exception
{
   std::string s;
   StringException(std::string ss) : s(ss) {}
   ~StringException() throw () {} // Updated
   const char* what() const throw() { return s.c_str(); }
};
