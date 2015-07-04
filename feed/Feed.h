//
// Created by ssi on 24.05.15.
//

#ifndef RSS_AGGREGATOR_FEED_H
#define RSS_AGGREGATOR_FEED_H

#include "post/Post.h"
#include "mongo/Mongo.h"
#include "../Stack.h"
#include <vector>

namespace feed
{
	class Feed
	{
	protected:
		std::vector< Post > posts;

		void sort(unsigned l, unsigned r);
		void sortByDate();

		void listSort(std::vector< Stack< Post > > &stackList, std::vector< Post > &result);
		virtual std::vector< Stack< Post > > &readFeedFiles(const IDB &mongo, const std::string &path = "rss-files") = 0;
	public:
		Feed(std::vector< Post > _posts) : posts(_posts)
		{ }

		Feed()
		{ }

		const size_t &countFeeds() const;
		void insertToDB(const IDB &db);
		const std::vector< Post > &getPosts() const;
	};

};

#endif //RSS_AGGREGATOR_FEED_H
