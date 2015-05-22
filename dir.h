#ifndef DIR_H
#define DIR_H
#include <string>
#include "renderexception.h"

#define __DIR__ parseLastDir(__FILE__)

std::string parseLastDir(const std::string _file);
#endif
