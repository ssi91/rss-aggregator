#ifndef RSS_AGGREGATOR_SAMEFUNCTIONS_H
#define RSS_AGGREGATOR_SAMEFUNCTIONS_H

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

/**
 * скачивает файл
 * @param uri указывает путь, откуда качать
 */
void download(char *uri, char *path);
/**
 * скачивает файл
 * @param f указатель на файл с ссылками
 */
void downloadFromFile(FILE *f, char *path);
int write_data(void *ptr, unsigned long size, unsigned long nmemb, FILE *stream);
char *getFileName(char *path, bool &mustDelete);

#endif //RSS_AGGREGATOR_SAMEFUNCTIONS_H
