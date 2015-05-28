//
// Created by ssi on 23.05.15.
//

#include "RSS.h"

namespace feed
{

	RSS::RSS(const std::string &rssPath)
	{
		if (rssPath.substr(0, 4) == "http")
		{
			//TODO parsing by address
		}
		else
		{
			//TODO work with file in file system
		}
	}

	void RSS::pasteToDB()
	{
		pasted = true;
	}

	bool RSS::prepareFeed()
	{
		sortByDate();
		prepared = true;
	}
}
