//
// Created by ssi on 01.05.15.
//

#include "Request.h"
#include <string.h>
#include "../renderexception.h"

namespace vkmes
{
	sp::sp(const char *_key, const char *_value)
	{
		if (_key && _value)
		{
			key = new char[strlen(_key) + 1];
			strcpy(key, _key);

			value = new char[strlen(_value) + 1];
			strcpy(value, _value);
		}
		else
		{
			std::cout << "here!" << std::endl;
			key = nullptr;
			value = nullptr;
//			LOG_TRACE("Key or Value is null")
		}
	}

	sp::sp(const sp &_sp)
	{

		std::cout << "here CC!" << std::endl;
		key = new char[strlen(_sp.key) + 1];
		strcpy(key, _sp.key);

		value = new char[strlen(_sp.value) + 1];
		strcpy(value, _sp.value);
	}

	sp &sp::operator=(const sp &_sp)
	{
		std::cout << "here O=!" << std::endl;
		if (key)
			delete[] key;
		if (value)
			delete[] value;
		key = new char[strlen(_sp.key) + 1];
		strcpy(key, _sp.key);

		value = new char[strlen(_sp.value) + 1];
		strcpy(value, _sp.value);
		return *this;
	}

	sp::~sp()
	{
		if (key)
			delete[] key;
		if (value)
			delete[] value;
	}
}
