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
		//TODO не забудь удалить потом
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

	time_t Post::convertDateToTimestamp(const std::string &pubDate)
	{
		std::vector<std::string> words;
		std::istringstream ist(pubDate);
		std::string tmp;
		while (ist >> tmp)
			words.push_back(tmp);

		std::map<std::string, boost::date_time::months_of_year> mounthMap = setMonthMap();
		unsigned short month = mounthMap[words[2]];

		boost::posix_time::time_duration td(boost::posix_time::duration_from_string(words[4]));
		boost::local_time::time_zone_ptr tz(new boost::local_time::posix_time_zone("GMT"));
		boost::local_time::local_date_time ld(boost::gregorian::date(atoi(words[3].c_str()), month, atoi(words[1].c_str())), td, tz,
											  boost::local_time::local_date_time::NOT_DATE_TIME_ON_ERROR);
		boost::posix_time::time_duration::sec_type diff = (ld.utc_time() - boost::posix_time::from_time_t(0)).total_seconds();
		return (time_t) diff;
	}

	std::map<std::string, boost::date_time::months_of_year> Post::setMonthMap()
	{
		std::map<std::string, boost::date_time::months_of_year> mouthMap;
		mouthMap.insert(std::make_pair("Jan", boost::date_time::Jan));
		mouthMap.insert(std::make_pair("Feb", boost::date_time::Feb));
		mouthMap.insert(std::make_pair("Mar", boost::date_time::Mar));
		mouthMap.insert(std::make_pair("Apr", boost::date_time::Apr));
		mouthMap.insert(std::make_pair("May", boost::date_time::May));
		mouthMap.insert(std::make_pair("Jun", boost::date_time::Jun));
		mouthMap.insert(std::make_pair("Jul", boost::date_time::Jul));
		mouthMap.insert(std::make_pair("Aug", boost::date_time::Aug));
		mouthMap.insert(std::make_pair("Sep", boost::date_time::Sep));
		mouthMap.insert(std::make_pair("Oct", boost::date_time::Oct));
		mouthMap.insert(std::make_pair("Nov", boost::date_time::Nov));
		mouthMap.insert(std::make_pair("Dec", boost::date_time::Dec));

		//TODO сделать возврат по ссылке
		return mouthMap;
	}

	std::string Post::getTs_PubDatetoString() const
	{
		std::ostringstream pds;
		pds << ts_pubDate;
		return pds.str();
	}
}
