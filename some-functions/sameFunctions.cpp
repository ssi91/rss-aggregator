#include <iostream>
#include "sameFunctions.h"

//static int numFeed = 0;

void download(char *uri, char *_path)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;

	bool mustDelete = false;
	char *fName = getFileName(uri, mustDelete);

	char *path = new char[strlen(_path) + strlen(fName) + 1];
	strcpy(path, _path);

	if (path[strlen(path) - 1] != '/')
		strcat(path, "/");
//	if (!strcmp(getFileName(uri), ""))
//		strcat(path, "unknown_feed");
	strcat(path, fName);
	if (mustDelete)
		delete[] fName;
	curl = curl_easy_init();
	if (curl)
	{
		fp = fopen(path, "wb");
		curl_easy_setopt(curl, CURLOPT_URL, uri);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		fclose(fp);
	}
}

void downloadFromFile(FILE *f, char *path)
{
	if (!f)
		return; //TODO поменять, ничего не произойдёт
	while (!feof(f))
	{
		char uri[100];//TODO выделить память нормально
		fscanf(f, "%s[^\n]", uri);
//		++numFeed;
		download(uri, path);
	}
	fclose(f);
}

int write_data(void *ptr, unsigned long size, unsigned long nmemb, FILE *stream)
{
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

char *getFileName(char *path, bool &mustDelete)
{
	char *name;
	if (path[strlen(path) - 1] == '/')
	{
		name = new char[strlen("unknown_feed") + 1];
		strcpy(name, "unknown_feed");
		mustDelete = true;
	}
	else
	{
		name = strrchr(path, '/') + 1;
		mustDelete = false;
	}
	return name;
}
