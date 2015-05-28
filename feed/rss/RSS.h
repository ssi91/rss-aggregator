//
// Created by ssi on 23.05.15.
//

#ifndef RSS_AGGREGATOR_RSS_H
#define RSS_AGGREGATOR_RSS_H

/*
 * сбор данных всех RSS-ок, сортировка по дате,
 * загрузка в БД новые данныеЮ если они есть,
 * если данные обновленны, отправить сообщения в rabbitmq
 */
/*
 * загружать rss
 * выяснять, есть ли обновления
 * сортировать по дате, если обновления с нескольких лент
 * выдавать список не добавленных в базу постов
 * выдавать список всех постов(из БД и пришедшие)
 */
#include <bits/stringfwd.h>
#include <bits/stl_list.h>
#include "../Feed.h"

namespace feed
{
	class RSS : public Feed
	{
	private:
//		std::list<std::string> feeds;
		bool pasted;
		bool prepared;
	public:
		RSS(const std::list<std::string> &_feeds);
		RSS(const std::string &rssPath);

		bool prepareFeed();
		void pasteToDB();
	};
};

#endif //RSS_AGGREGATOR_RSS_H
