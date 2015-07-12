#include "feed/post/Post.h"
#include "feed/mongo/Mongo.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include "dir.h"
#include "feed/rss/RSS.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include "some-functions/sameFunctions.h"

using namespace feed;
namespace fs = boost::filesystem;


int main(int argc, char *argv[])
{
	int argi = 1;
	std::string host;
	std::string base;
//	std::string path;
	std::string user;
	std::string pass;
	std::string filename;

	while (argc > argi && *argv[argi] == '-') // option
	{
		switch (*(argv[argi] + 1))
		{
			case 'h':
				++argi;
				host = argv[argi];
				break;
			case 'b':
				++argi;
				base = argv[argi];
				break;
			case 'f':
				++argi;
				filename = argv[argi];
				break;
			case 'u':
				++argi;
				user = argv[argi];
				break;
			case 'p':
				pass = argv[argi] + 2;
				break;
			default:
				std::cout << "       -h  Host" << std::endl
				<< "       -b  base.collection" << std::endl
				<< "       -f  path to save feeds" << std::endl;
				return EXIT_FAILURE;
		}
		argi++;
	}

//	std::string filename = __DIR__ + "feeds";
	const char rssDir[] = "rss-files/";
	char *dir = new char[strlen(parseLastDir(__FILE__).c_str()) + strlen(rssDir) + 1];
	strcpy(dir, parseLastDir(__FILE__).c_str());
	strcat(dir, "rss-files/");
	FILE *file = fopen(filename.c_str(), "r");
	downloadFromFile(file, dir);
	delete[] dir;
	try
	{
		Mongo mongo1(host, base);
		if (!mongo1.auth(user, pass))
			return 2;
		RSS feed(mongo1);
		if (!feed.countFeeds()) //TODO логгировать каждый запуск
			return 0;
		feed.insertToDB(mongo1);
		AmqpClient::Channel::ptr_t connection = AmqpClient::Channel::Create("localhost");
		connection->DeclareQueue("hello", false, false, false, false);
		connection->BasicPublish("", "hello", AmqpClient::BasicMessage::Create("update"));
	}
	catch (mongo::DBException &e)
	{
		std::cout << "caught DBException " << e.toString() << std::endl;
		return 1;
	}

	return 0;
}