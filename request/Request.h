//
// Created by ssi on 28.04.15.
//

#ifndef CLIENT_REQUEST_H
#define CLIENT_REQUEST_H

#include <curl/curl.h>
#include <string>
#include "../Stack.h"

namespace vkmes
{
	struct sp
	{
		char *key;
		char *value;

		sp(const char *_key = nullptr, const char *_value = nullptr);
		sp(const sp &_sp);
		~sp();
		sp &operator=(const sp &_sp);
	};

	class Request
	{
	private:
		char *secretKey;
		char *token;
		char *code;
		char *clientId;

		char *addGetParam(char *link, const char *pKey, const char *val) const;
		//char *getToken() const;

		static const char REQUEST_GET_PARAM = 0;
		static const char REQUEST_POST_PARAM = 1;
		static int writeResp(char *data, size_t size, size_t nmemb, std::string *buffer);
		static int write_data(void *ptr, unsigned long size, unsigned long nmemb, FILE *stream);
	public:
		Request(const char *_clientId, const char *_secretKey, const char *_code, const char *_token);
		Request(const char *_clientId, const char *_secretKey, const char *_code);
		~Request();
		//bool auth() const;
		void *req(const char *uri, Stack<sp> &_paramStack, char const paramType = 0);
		void *download(const char *uri) const;
	};

}
#endif //CLIENT_REQUEST_H
