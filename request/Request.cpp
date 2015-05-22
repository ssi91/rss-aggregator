//
// Created by ssi on 28.04.15.
//

#include "Request.h"
#include <string.h>
#include <stdio.h>
#include "../renderexception.h"

namespace vkmes
{
	Request::Request(const char *_clientId, const char *_secretKey, const char *_code)
	{
		if (_secretKey && _code && _clientId)
		{
			secretKey = new char[strlen(_secretKey)];
			strcpy(secretKey, _secretKey);
			code = new char[strlen(_code)];
			strcpy(code, _code);
			clientId = new char[strlen(_clientId)];
			strcpy(clientId, _clientId);
		}
		else
		{
			LOG_TRACE("Secret Key or Code or ClientId is NULL")
		}
	}

	Request::Request(const char *_clientId, const char *_secretKey, const char *_code, const char *_token) : Request(_clientId, _secretKey, _code)
	{
		if (_token)
		{
			token = new char[strlen(_token)];
			stpcpy(token, _token);
		}
		else
		{
			LOG_TRACE("Token is NULL")
		}
	}

	Request::~Request()
	{
		if (clientId)
			delete[] clientId;
		if (secretKey)
			delete[] secretKey;
		if (code)
			delete[] code;
		if (token)
			delete[] token;
	}

	char *Request::addGetParam(char *link, const char *pKey, const char *val) const
	{
		char *keyValString = new char[strlen(pKey) + 2 + strlen(val)];
		strcpy(keyValString, pKey);
		strcat(keyValString, "=");
		strcat(keyValString, val);
		size_t l = strlen(link) + 2 + strlen(keyValString);
		char *temp = new char[l];
		strcpy(temp, link);
		if (strchr(link, '?'))
		{
			strcat(temp, "&");
		}
		else
		{
			strcat(temp, "?");
		}
		strcat(temp, keyValString);
		delete[] keyValString;
		delete[] link;
		link = new char[l];
		strcpy(link, temp);
		delete[] temp;
		return link;
	}

	int Request::writeResp(char *data, size_t size, size_t nmemb, std::string *buffer)
	{
		//переменная - результат, по умолчанию нулевая
		int result = 0;
		//проверяем буфер
		if (buffer != NULL)
		{
			//добавляем к буферу строки из data, в количестве nmemb
			buffer->append(data, size * nmemb);
			//вычисляем объем принятых данных
			result = size * nmemb;
		}
		//вовзращаем результат
		return result;
	}

	void *Request::req(const char *uri, Stack<sp> &_paramStack, char const paramType)
	{
		CURL *curl;
		CURLcode result;
		curl = curl_easy_init();
		char errorBuffer[CURL_ERROR_SIZE];
		std::string buffer;
		if (curl)
		{
			char *url = new char[strlen(uri) + 1];
			strcpy(url, uri);
			if (!paramType)
			{
				while (_paramStack.getCount())
				{
					sp sp1;
					sp1 = _paramStack.pop();
					url = addGetParam(url, sp1.key, sp1.value);
				}
			}
			curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &errorBuffer);
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_HEADER, 1); //TODO уметь редактировать через параметр
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeResp);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
			result = curl_easy_perform(curl);
			if (result == CURLE_OK)
			{
				std::string *qq = new std::string(buffer);
				return (void *)qq;
			}
			curl_easy_cleanup(curl);
		}
		return nullptr;
	}
	
	int Request::write_data(void *ptr, unsigned long size, unsigned long nmemb, FILE *stream) 
	{
		size_t written = fwrite(ptr, size, nmemb, stream);
		return written;
	}

	void *Request::download(const char *uri) const
	{
		CURL *curl;
		FILE *fp;
		CURLcode res;
		char outfilename[FILENAME_MAX] = "/home/ssi/cppproj/dFile/composer.json";//TODO сделать относительный путь
		curl = curl_easy_init();
		if (curl)
	       	{
			fp = fopen(outfilename,"wb");
			curl_easy_setopt(curl, CURLOPT_URL, uri);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			res = curl_easy_perform(curl);

			curl_easy_cleanup(curl);
			fclose(fp);
		}
	}

}
