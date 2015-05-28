//
// Created by ssi on 24.05.15.
//

#ifndef RSS_AGGREGATOR_FEED_H
#define RSS_AGGREGATOR_FEED_H

#include "post/Post.h"
#include "mongo/Mongo.h"
/*
 * выяснять, есть ли обновления
 * сортировать по дате, если обновления с нескольких лент
 * выдавать список не добавленных в базу постов
 * выдавать список всех постов(из БД и пришедшие)
 */
namespace feed
{
	class Feed
	{
	protected:
		std::vector<Post> posts;
		bool prepared;
		bool pasted;

		void sort(unsigned l, unsigned r);
		void sortByDate();

		Feed();
		bool prepareFeed();
		void insertToDB();
	public:
		Feed(std::vector<Post> _posts) : posts(_posts)
		{ }

		const std::vector<Post> &getPosts() const;
	};

};

#endif //RSS_AGGREGATOR_FEED_H
