//
// Created by ssi on 25.05.15.
//

#ifndef RSS_AGGREGATOR_MONGO_H
#define RSS_AGGREGATOR_MONGO_H
#include "../Idb.h"
#include "mongo/client/dbclient.h" // the mongo c++ driver
#include "../post/Post.h"

namespace feed
{
	class Mongo : public IDB
	{
	private:
		boost::scoped_ptr< mongo::DBClientBase > conn;
		mongo::ConnectionString cs;
		std::string base;
		std::string uri;
		std::string errmsg;
		unsigned port;

		void initConnection(const std::string &fullUri);
	public:
		Mongo(const std::string &_uri, const std::string &_base, const unsigned &_port);
		Mongo(const std::string &fullUrl, const std::string &_baseCollect);

		std::vector< Post > &getFromDB();
		void setToDB(std::vector< Post > &) const;
		const Post &getLastByDate(std::string &category) const;
		bool isUniquePost(const Post &) const;
		bool auth(const std::string &login, const std::string &password);

		static std::string delQuotes(std::string);
	};
};

#endif //RSS_AGGREGATOR_MONGO_H
