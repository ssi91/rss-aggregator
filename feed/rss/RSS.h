#ifndef RSS_AGGREGATOR_RSS_H
#define RSS_AGGREGATOR_RSS_H

#include "../Feed.h"
#include <boost/filesystem.hpp>
#include "../../dir.h"

namespace feed
{
	class RSS : public Feed
	{
	private:
		std::vector< Stack< Post > > &readFeedFiles(const IDB &db, const std::string &path = "rss-files");

		static std::map< std::string, boost::date_time::months_of_year > &setMonthMap();
	public:
		RSS(const IDB &db, bool autoInit = true);

		static time_t convertDateToTimestamp(const std::string &pubDate);
	};
};

#endif //RSS_AGGREGATOR_RSS_H
