#ifndef HTTPAPI_H
#define HTTPAPI_H

#ifdef WIN32

#ifdef  HTTP_EXPORT
#define HTTP_API __declspec(dllexport)
#else
#define HTTP_API __declspec(dllimport)
#endif

#else

#define HTTP_API extern

#endif


enum HTTP_CODE
{
	HTTP_CODE_OK = 200,
	HTTP_CODE_BADREQUEST = 400,
	HTTP_CODE_NOTFOUND = 404,
};

typedef void(*HttpHandler)(unsigned int responseID, const char *command, const char *query);

extern "C" 
{
	HTTP_API int  CreateServer(const char *ip, const char *port);
	HTTP_API bool AddServerHandler(int serverid, const char *command, HttpHandler handler);
	HTTP_API void StopServer(int serverid);
	HTTP_API void UpdateServer();
	HTTP_API void SetResponse(unsigned int responseID, int code, const char *data);
}



#endif