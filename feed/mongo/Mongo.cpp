//
// Created by ssi on 25.05.15.
//

#include "Mongo.h"
#include "../../renderexception.h"

namespace feed
{
	std::vector< Post > &Mongo::getFromDB()
	{
		mongo::Query query;
		std::auto_ptr< mongo::DBClientCursor > cursor = conn->query(base, query, 0);

		std::vector< Post > *vP = new std::vector< Post >;

		while (cursor->more())
		{
			mongo::BSONObj obj = cursor->next();
			Post post(obj["title"].str(), obj["preview"].str(), obj["pubDate"].numberInt(), obj["body"].str(), obj["category"].str());
			vP->push_back(post);
		}
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
			mongo::BSONObj o = BSON("title" << post->getTitle().c_str()
									<< "preview" << post->getPreview().c_str()
									<< "pubDate" << (int) post->getTs_PubDate()
									<< "body" << post->getBody().c_str()
									<< "category" << post->getCategory());
			conn->insert(base, o);
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
			std::ostringstream statusStream;
			statusStream << instance.status();
			std::string status = statusStream.str();
			LOG_TRACE("failed to initialize the client driver: " + status + "\n");
		}

		cs = mongo::ConnectionString::parse(fullUri, errmsg);

		if (!cs.isValid())
		{
			LOG_TRACE("Error parsing connection string " + uri + " (" + errmsg + ")\n");
		}

		conn.reset(cs.connect(errmsg));
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
		Post *post = new Post(obj["title"].str(), obj["preview"].str(), obj["pubDate"].numberInt(), obj["body"].str(), obj["category"].str());
		return *post;
	}

	bool Mongo::auth(const std::string &login, const std::string &password)
	{
		return conn->auth(base.substr(0, base.find('.')), login, password, errmsg);
	}
}
