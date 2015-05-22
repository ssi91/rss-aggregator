#include "dir.h"

std::string parseLastDir(const std::string _file)
{
	if (_file == "")
		throw LOG_TRACE("dir-string is empty!")
	int endOfDirString = 0;
	for (int i = _file.length() - 1; i >= 0; i--)
	{
		if (_file[i] == '/')
		{
			return _file.substr(0, i + 1);
		}
		else if(!i)
		{
			throw LOG_TRACE("Wrong dir-string!")
		}
	}
}