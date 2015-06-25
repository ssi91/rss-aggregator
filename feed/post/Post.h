#ifndef RSS_AGGREGATOR_POST_H
#define RSS_AGGREGATOR_POST_H

#include <string>
#include <vector>
#include <iostream>
#include "boost/date_time/local_time/local_time.hpp"
#include <map>

namespace feed
{
	class Post
	{
	private:
		std::string title;
		std::string preview;
		std::string body;
		time_t ts_pubDate;

	public:
		Post(const std::string &title, const std::string &preview, const time_t &ts_pubDate, const std::string &body) : title(title),
																														preview(preview),
																														ts_pubDate(ts_pubDate),
																														body(body)
		{ }

		Post() : ts_pubDate(0)
		{ }

		const std::string &getTitle() const;
		const std::string &getPreview() const;
		const std::string &getBody() const;
		const time_t &getTs_PubDate() const;
		std::string getTs_PubDatetoString() const;

		virtual const std::string &toOctopress() const;
		virtual const std::string &toOctopress(const std::string &separator) const;
	};

};

#endif //RSS_AGGREGATOR_POST_H
