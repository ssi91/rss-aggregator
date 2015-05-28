//
// Created by ssi on 24.05.15.
//

#include "Feed.h"

namespace feed
{
	const std::vector<Post> &Feed::getPosts() const
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

	Feed::Feed()
	{
		prepared = pasted = false;
	}

	bool Feed::prepareFeed()
	{
		Post lDP = Mongo::getLastByDate();
		if (lDP.getTs_PubDate() < posts.back().getTs_PubDate())
		{
			//TODO найти годные для вставки посты, а остальные удалить
			size_t i = posts.size() - 1;
			for (std::vector<Post>::iterator it = posts.end(); it != posts.begin(); --it)
			{
				if (it->getTs_PubDate() > lDP.getTs_PubDate())
				{
					--i;
				}
			}
			posts.erase(posts.begin(), posts.begin() + i - 1);
			prepared = true;
			pasted = false;
		}
		else
		{
			prepared = false;
			pasted = true;
		}
	}

	void Feed::insertToDB()
	{
		if (prepared && !pasted)
		{
			Mongo::setToDB(posts);
			pasted = true;
		}
	}
};