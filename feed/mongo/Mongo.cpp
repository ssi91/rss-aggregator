//
// Created by ssi on 25.05.15.
//

#include "Mongo.h"
#include <sstream>

namespace feed
{
	std::vector<Post> &Mongo::getFromDB()
	{
		mongo::Query query;
		std::vector<mongo::BSONObj> feedVector;
		std::auto_ptr<mongo::DBClientCursor> cursor = conn->query(base, query, 0);

		std::vector<Post> *vP = new std::vector<Post>;

		while (cursor->more())
		{
			mongo::BSONObj obj = cursor->next();
			Post post(delQuotes(obj["title"].toString(false)), delQuotes(obj["preview"].toString(false)), atoi(obj["pubDate"].toString(false).c_str()),
					  delQuotes(obj["body"].toString()));
			vP->push_back(post);
		}
//TODO: не забыть удалить
		return *vP;
	}

	std::string Mongo::delQuotes(std::string string)
	{
		string.erase(0, 1);
		string.erase(string.length() - 1);
		return string;
	}

	void Mongo::setToDB(std::vector<Post> &vector)
	{
		for (std::vector<Post>::iterator post = vector.begin(); post != vector.end(); ++post)
		{
			if (isUniquePost(*post))
			{
				mongo::BSONObj o = BSON("title" << post->getTitle()
										<< "preview" << post->getPreview()
										<< "pubDate" << (int) post->getTs_PubDate()
										<< "body" << post->getBody());
				mongo::Query query(o);
				conn->insert("feed_test.testcoll", o);
			}
		}
	}

	bool Mongo::isUniquePost(const Post &post)
	{
		mongo::BSONObj o = BSON("title" << post.getTitle()
								<< "preview" << post.getPreview()
								<< "pubDate" << post.getTs_PubDatetoString());
		mongo::Query query(o);
		std::auto_ptr<mongo::DBClientCursor> cursor = conn->query(base, query);
		return cursor->itcount() == 0;
	}

	void Mongo::initConnection(const std::string &fullUri)
	{
		mongo::client::GlobalInstance instance;

		if (!instance.initialized())
		{
			//TODO fail
		}

		cs = mongo::ConnectionString::parse(fullUri, errmsg);

		if (!cs.isValid())
		{
			//TODO fail
		}

		conn.reset(cs.connect(errmsg));
		conn->auth("feed_test", "u1", "p1", errmsg);

	}

	Mongo::Mongo(const std::string &_uri, const std::string &_base, const unsigned &_port) : uri(_uri), base(_base), port(_port)
	{
		std::ostringstream portStream;
		portStream << port;
		std::string sp = portStream.str();
		std::string fullUri = uri + ":" + sp;
		initConnection(fullUri);
	}

	Mongo::Mongo(const std::string &fullUrl, const std::string &_baseCollect) : base(_baseCollect)
	{
		unsigned index = (unsigned int) fullUrl.find(':', fullUrl.find(':') + 1);
		uri = fullUrl.substr(0, index);

		std::istringstream portStream(fullUrl.substr(index + 1));
		portStream >> port;

		initConnection(fullUrl);
	}
}
