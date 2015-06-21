//
// Created by ssi on 25.05.15.
//

#ifndef RSS_AGGREGATOR_IDB_H
#define RSS_AGGREGATOR_IDB_H

#include <vector>
#include "post/Post.h"

namespace feed
{
	class IDB
	{
	public:
		virtual std::vector<Post> &getFromDB() = 0;
		virtual void setToDB(std::vector<Post> &) = 0;
	};
};
#endif //RSS_AGGREGATOR_IDB_H
