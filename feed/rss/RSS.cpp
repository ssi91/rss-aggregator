//
// Created by ssi on 23.05.15.
//

#include "RSS.h"
#include "libxml++/libxml++.h"

namespace feed
{
	std::vector< Stack< Post > > &RSS::readFeedFiles(const IDB &db, const std::string &path)
	{
		time_t lastDate;
		using namespace boost::filesystem;
		std::vector< Stack< Post > > *vector = new std::vector< Stack< Post > >;

		for (recursive_directory_iterator it(__DIR__ + "../../rss-files"), end; it != end; ++it)
		{
			try
			{
				std::string fullPath = canonical(*it).string();

				xmlpp::DomParser parser;
				parser.parse_file(fullPath);

				const xmlpp::Node *pNode = parser.get_document()->get_root_node();

				xmlpp::Node::NodeList rssChList;
				if (pNode->get_name() == "rss")
				{
					rssChList = pNode->get_children();
					pNode = pNode->get_first_child();
				}
				xmlpp::Node::NodeList itemList;
				for (xmlpp::Node::NodeList::iterator itln = rssChList.begin(); itln != rssChList.end(); ++itln)
				{
					if ((*itln)->get_name() == "channel")
					{
						pNode = *itln;
						break;
					}
				}
				itemList = pNode->get_children();

				time_t pubDate = 0;
				std::string category, globalLink;
				Stack< Post > stack;

				for (xmlpp::Node::NodeList::iterator item = itemList.begin(); item != itemList.end(); ++item)
				{
					if ((*item)->get_name() == "item")
					{

						std::string title, preview, body, link = "", media;
						xmlpp::Node::NodeList itemNodes = (*item)->get_children();
						for (xmlpp::Node::NodeList::iterator itemNode = itemNodes.begin(); itemNode != itemNodes.end(); ++itemNode)
						{
							if ((*itemNode)->get_name() == "link")
							{
								const xmlpp::TextNode *nodeText = dynamic_cast<const xmlpp::TextNode *>((*itemNode)->get_first_child());
								if (nodeText)
								{
									link = nodeText->get_content();
								}
								else
								{
									const xmlpp::ContentNode *nodeContent = dynamic_cast<const xmlpp::ContentNode *>((*itemNode)->get_first_child());
									if (nodeContent)
										link = nodeContent->get_content();
								}
								if (link == "")
									link = globalLink;
							}
							else if ((*itemNode)->get_name() == "enclosure")
							{
								const xmlpp::Element *element = dynamic_cast<const xmlpp::Element *>((*itemNode));
								if (element)
								{
									xmlpp::Attribute *attr = element->get_attribute("url");
									media = attr->get_value();
								}
							}
							else if ((*itemNode)->get_name() == "title")
							{
								const xmlpp::TextNode *nodeText = dynamic_cast<const xmlpp::TextNode *>((*itemNode)->get_first_child());
								if (nodeText)
								{
									title = nodeText->get_content();
								}
							}
							else if ((*itemNode)->get_name() == "title")
							{
								const xmlpp::TextNode *nodeText = dynamic_cast<const xmlpp::TextNode *>((*itemNode)->get_first_child());
								if (nodeText)
								{
									title = nodeText->get_content();
								}
							}
							else if ((*itemNode)->get_name() == "subtitle")
							{
								const xmlpp::TextNode *nodeText = dynamic_cast<const xmlpp::TextNode *>((*itemNode)->get_first_child());
								if (nodeText)
								{
									preview = nodeText->get_content();
								}
								else
								{
									const xmlpp::ContentNode *nodeContent = dynamic_cast<const xmlpp::ContentNode *>((*itemNode)->get_first_child());
									if (nodeContent)
										preview = nodeContent->get_content();
								}
							}
							else if ((*itemNode)->get_name() == "description")
							{
								const xmlpp::TextNode *nodeText = dynamic_cast<const xmlpp::TextNode *>((*itemNode)->get_first_child());
								if (nodeText)
								{
									body = nodeText->get_content();
								}
								else
								{
									const xmlpp::ContentNode *nodeContent = dynamic_cast<const xmlpp::ContentNode *>((*itemNode)->get_first_child());
									if (nodeContent)
										body = nodeContent->get_content();
								}
							}
							else if ((*itemNode)->get_name() == "encoded")
							{
								const xmlpp::TextNode *nodeText = dynamic_cast<const xmlpp::TextNode *>((*itemNode)->get_first_child());
								if (nodeText)
								{
									body = nodeText->get_content();
								}
								else
								{
									const xmlpp::ContentNode *nodeContent = dynamic_cast<const xmlpp::ContentNode *>((*itemNode)->get_first_child());
									if (nodeContent)
										body = nodeContent->get_content();
								}
							}
							else if ((*itemNode)->get_name() == "pubDate")
							{
								const xmlpp::TextNode *nodeText = dynamic_cast<const xmlpp::TextNode *>((*itemNode)->get_first_child());
								if (nodeText)
								{
									pubDate = convertDateToTimestamp(nodeText->get_content());
								}
							}
						}
						if (pubDate > lastDate)
						{
							Post post = Post(title, preview, pubDate, body, category, media, link);
							stack.push(post);
						}
						else
							break;
					}
					else if ((*item)->get_name() == "title")
					{
						const xmlpp::TextNode *nodeText = dynamic_cast<const xmlpp::TextNode *>((*item)->get_first_child());
						if (nodeText)
						{
							category = nodeText->get_content();
						}
						lastDate = db.getLastByDate(category).getTs_PubDate();
					}
					else if ((*item)->get_name() == "link")
					{
						const xmlpp::TextNode *nodeText = dynamic_cast<const xmlpp::TextNode *>((*item)->get_first_child());
						if (nodeText)
						{
							globalLink = nodeText->get_content();
						}
					}
					else
						continue;
				}
				if (stack.getCount())
					vector->push_back(stack);
			}
			catch (xmlpp::parse_error &pe)
			{
				continue;
			}
		}

		return *vector;
	}

	time_t RSS::convertDateToTimestamp(const std::string &pubDate)
	{
		std::vector< std::string > words;
		std::istringstream ist(pubDate);
		std::string tmp;
		while (ist >> tmp)
			words.push_back(tmp);

		std::map< std::string, boost::date_time::months_of_year > mounthMap = setMonthMap();
		unsigned short month = mounthMap[words[2]];

		boost::posix_time::time_duration td(boost::posix_time::duration_from_string(words[4]));
		boost::local_time::time_zone_ptr tz(new boost::local_time::posix_time_zone("GMT"));
		boost::local_time::local_date_time ld(boost::gregorian::date(atoi(words[3].c_str()), month, atoi(words[1].c_str())), td, tz,
											  boost::local_time::local_date_time::NOT_DATE_TIME_ON_ERROR);
		boost::posix_time::time_duration::sec_type diff = (ld.utc_time() - boost::posix_time::from_time_t(0)).total_seconds();
		return (time_t) diff;
	}

	std::map< std::string, boost::date_time::months_of_year > &RSS::setMonthMap()
	{
		std::map< std::string, boost::date_time::months_of_year > *mouthMap = new std::map< std::string, boost::date_time::months_of_year >;
		mouthMap->insert(std::make_pair("Jan", boost::date_time::Jan));
		mouthMap->insert(std::make_pair("Feb", boost::date_time::Feb));
		mouthMap->insert(std::make_pair("Mar", boost::date_time::Mar));
		mouthMap->insert(std::make_pair("Apr", boost::date_time::Apr));
		mouthMap->insert(std::make_pair("May", boost::date_time::May));
		mouthMap->insert(std::make_pair("Jun", boost::date_time::Jun));
		mouthMap->insert(std::make_pair("Jul", boost::date_time::Jul));
		mouthMap->insert(std::make_pair("Aug", boost::date_time::Aug));
		mouthMap->insert(std::make_pair("Sep", boost::date_time::Sep));
		mouthMap->insert(std::make_pair("Oct", boost::date_time::Oct));
		mouthMap->insert(std::make_pair("Nov", boost::date_time::Nov));
		mouthMap->insert(std::make_pair("Dec", boost::date_time::Dec));

		return *mouthMap;
	}

	RSS::RSS(const IDB &db, bool autoInit)
	{
		if (autoInit)
		{
			listSort(readFeedFiles(db), posts);
		}

	}
}
