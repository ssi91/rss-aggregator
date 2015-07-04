//
// Created by ssi on 24.05.15.
//

#include "Feed.h"

namespace feed
{
	const std::vector< Post > &Feed::getPosts() const
	{
		return posts;
	}


	void Feed::sort(unsigned l, unsigned r)
	{
		unsigned storL = l;
		unsigned storR = r;
		unsigned m = posts[(l + r) / 2].getTs_PubDate();
		while (l <= r)
		{
			while (posts[l].getTs_PubDate() < m)
			{
				++l;
			}
			while (posts[r].getTs_PubDate() > m)
			{
				--r;
			}
			if (l <= r)
			{
				std::swap(posts[l], posts[r]);
				++l;
				--r;
			}
		}

		if (storL < r)
			sort(storL, r);
		if (storR > l)
			sort(l, storR);

	}

	void Feed::sortByDate()
	{
		sort(0, posts.size() - 1);
	}

	void Feed::insertToDB(const IDB &db)
	{
		db.setToDB(posts);
	}

	void Feed::listSort(std::vector< Stack< Post > > &stackList, std::vector< Post > &result)
	{
		unsigned i = 0, k = 0;
		for (std::vector< Stack< Post > >::iterator stack = stackList.begin(); stack != stackList.end(); ++stack)
		{
			if (!stack->getCount())
			{
				stackList.erase(stack);
				continue;
			}
			if (stackList[k].getTop().getTs_PubDate() > stack->getTop().getTs_PubDate())
			{
				k = i;
			}
			++i;
		}
		if (stackList.size())
		{
			result.push_back(stackList[k].pop());

			if (!stackList[k].getCount())
			{
				stackList.erase(stackList.begin() + k - 1);
			}
			if (stackList.size())
			{
				listSort(stackList, result);
			}
		}
	}

	const size_t &Feed::countFeeds() const
	{
		return posts.size();
	}
}
