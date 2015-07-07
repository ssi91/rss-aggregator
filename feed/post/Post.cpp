//
// Created by ssi on 24.05.15.
//

#include "Post.h"

//using namespace boost::gregorian;
//using namespace boost::posix_time;
//using namespace boost::local_time;
namespace feed
{
	const std::string &Post::getTitle() const
	{
		return title;
	}

	const std::string &Post::getPreview() const
	{
		return preview;
	}

	const std::string &Post::getBody() const
	{
		return body;
	}

	const std::string &Post::toOctopress(const std::string &separator) const
	{
		std::string *octoPost = new std::string(preview + '\n' + separator + '\n' + body + '\n');
		return *octoPost;
	}

	const std::string &Post::toOctopress() const
	{
		return toOctopress("<!--more-->");
	}

	const time_t &Post::getTs_PubDate() const
	{
		return ts_pubDate;
	}


	std::string Post::getTs_PubDatetoString() const
	{
		std::ostringstream pds;
		pds << ts_pubDate;
		return pds.str();
	}

	const std::string & Post::getCategory() const
	{
		return category;
	}
}
