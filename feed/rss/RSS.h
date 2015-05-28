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
 *
 */
#include <bits/stringfwd.h>
#include <bits/stl_list.h>

namespace feed
{
	class RSS
	{
	private:
		std::list<std::string> feeds;
		bool pasted;
	public:
		RSS(const std::list<std::string> &_feeds);

		void pasteToDB();
	};
};

#endif //RSS_AGGREGATOR_RSS_H
