//
// Created by ssi on 25.05.15.
//

#include "Mongo.h"
#include <sstream>

namespace feed
{
	std::vector< Post > &Mongo::getFromDB()
	{
		mongo::Query query;
		std::vector< mongo::BSONObj > feedVector;
		std::auto_ptr< mongo::DBClientCursor > cursor = conn->query(base, query, 0);

		std::vector< Post > *vP = new std::vector< Post >;

		while (cursor->more())
		{
			mongo::BSONObj obj = cursor->next();
			Post post(obj["title"].str(), obj["preview"].str(), obj["pubDate"].numberInt(), obj["body"].str());
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

	void Mongo::setToDB(std::vector< Post > &vector) const
	{
		for (std::vector< Post >::iterator post = vector.begin(); post != vector.end(); ++post)
		{
//			if (isUniquePost(*post))
//			{
			mongo::BSONObj o = BSON("title" << post->getTitle().c_str()
									<< "preview" << post->getPreview().c_str()
									<< "pubDate" << (int) post->getTs_PubDate()
									<< "body" << post->getBody().c_str());
//				mongo::Query query(o);

			conn->insert(base, o);
//			}
		}
	}

	bool Mongo::isUniquePost(const Post &post) const
	{
		mongo::BSONObj o = BSON("title" << post.getTitle()
								<< "preview" << post.getPreview()
								<< "pubDate" << (int) post.getTs_PubDate());
		mongo::Query query(o);
		std::auto_ptr< mongo::DBClientCursor > cursor = conn->query(base, query);
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

		//TODO база, логин и пароль брать из полей
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

	const Post &Mongo::getLastByDate() const
	{
		mongo::Query query;
		mongo::BSONObj obj = conn->findOne(base, query.sort("pubDate", -1));
		Post *post = new Post(obj["title"].str(), obj["preview"].str(), obj["pubDate"].numberInt(), obj["body"].str());
		return *post;
	}
}
