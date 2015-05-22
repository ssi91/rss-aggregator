#include "servsock.h"

extern int socketId;

bool getGetParam(char *to, const char *paramName, FCGX_Request *request)
{
	int i = 0;
	char *param_string;
	char get_params[100][100]; //массив параметров вида name=val
	//get_params[0] = new char[1024]; //массив параметров вида name=val
	//массив параметров вида name=val
	
	
	int j = 0; //счётчик параметров
	int jj = 0;
	param_string = FCGX_GetParam("QUERY_STRING", request->envp);

	while (param_string[i] != '\0')
	{
		if (param_string[i] == '&')
		{
			char buff[100];
			//buff = get_params[j];
			strcpy(buff, get_params[j]);
			j++;
			jj = 0;
			
		}
		else
		{
			get_params[j][jj] = param_string[i];
			jj++;
		}
		i++;
	}
	//get_params[j + 1] = NULL;
	//delete [] param_string;

	i = 0;
	char par[20], val[100];
	while (get_params[i][0] != '\0')
	{
		j = 0;
		int k = 0;
		bool is_param_name = false;
		while (get_params[i][j] != '\0')
		{
			if (get_params[i][j] == '=')
			{
				is_param_name = true;
			}
			else
			{
				if(!is_param_name)
				{
					par[j] = get_params[i][j];
				}
				else
				{
					val[k] = get_params[i][j];
					k++;
				}
			}
			j++;
		}

		if (strcmp(par, paramName) == 0)
		{
			strcpy(to, val);
			return true;
		}

		i++;
	}
	return false;
}

void *doit(void *a)
{
	int rc, i;
	FCGX_Request request;
	char *server_name;
	std::string s = __DIR__;
	s += "compils/index.templ.html";
	ifstream istr(s.c_str());
	string html = "";
	char c;
	istr.get(c);
	while(!istr.eof())
	{
		html += c;
		istr.get(c);
	}
//	cout << html << endl;
	istr.close();

	if (FCGX_InitRequest(&request, socketId, 0) != 0)
	{
		//ошибка при инициализации структуры запроса
		printf("Can not init request\n");
		return NULL;
	}
	printf("Request is inited\n");

	for (;;)
	{
		static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;
		//попробовать получить новый запрос
		printf("Try to accept new request\n");
		pthread_mutex_lock(&accept_mutex);
		rc = FCGX_Accept_r(&request);
		pthread_mutex_unlock(&accept_mutex);

		if (rc < 0)
		{
			//ошибка при получении запроса
			printf("Can not accept new request\n");
			break;
		}
		printf("request is accepted\n");
		char to[20], param[20];
		//FCGX_GetStr(param, 15, request.in);
		bool boo = getGetParam(to, "a", &request);
		//получить значение переменной
		server_name = FCGX_GetParam("QUERY_STRING", request.envp);

		//вывести все HTTP-заголовки (каждый заголовок с новой строки)
		FCGX_PutS("Content-type: text/html\r\n", request.out);

		//между заголовками и телом ответа нужно вывести пустую строку
		FCGX_PutS("\r\n", request.out);
		FCGX_PutS(html.c_str(), request.out);
		//вывести тело ответа (например - html-код веб-страницы)
		/*FCGX_PutS("<html>\r\n", request.out);
		FCGX_PutS("<head>\r\n", request.out);
		FCGX_PutS("<title>FastCGI Hello! (multi-threaded C, fcgiapp library)</title>\r\n", request.out);
		FCGX_PutS("</head>\r\n", request.out);
		FCGX_PutS("<body>\r\n", request.out);
		FCGX_PutS("<h1>FastCGI Hello! (multi-threaded C, fcgiapp library)</h1>\r\n", request.out);
		int i = 0;
		while (request.envp[i])
		{
			FCGX_PutS(request.envp[i], request.out);
			FCGX_PutS("<br>\r\n", request.out);
			i++;
		}
		//FCGX_PutS(request.in, request.out);
		FCGX_PutS("\r\n", request.out);
		FCGX_PutS(param, request.out);
		FCGX_PutS("\r\n", request.out);
		FCGX_PutS("<p>Request accepted from host <i>", request.out);
		FCGX_PutS(to ? to : "?", request.out);
		FCGX_PutS("</i></p>\r\n", request.out);
		FCGX_PutS("</body>\r\n", request.out);
		FCGX_PutS("</html>\r\n", request.out);*/

		//"заснуть" - имитация многопоточной среды
		sleep(0);

		//закрыть текущее соединение
		FCGX_Finish_r(&request);

		//завершающие действия - запись статистики, логгирование ошибок и т.п.
	}

	return NULL;
}
