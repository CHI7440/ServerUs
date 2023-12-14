#include<iostream>
#include<map>
#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<tmwp>
#include<vector>

using namespace std;
using namespace tmwp;

char *getMimeType(char *resource);


Response::Response(int clientSocketDescriptor)
{
this->clientSocketDescriptor = clientSocketDescriptor;
this->isClosed = false;
this->headerCreated = false;
}

void Response::createHeader()
{
char header[51];
strcpy(header,"HTTP/1.1 200 OK\nContent-Type:text/html\n\n");
send(this->clientSocketDescriptor,header,strlen(header),0);
this->headerCreated = true;
}

void Response::write(const char* responseBuffer)
{
if(responseBuffer==NULL) return;
int len = strlen(responseBuffer);
if(len==0) return;
if(this->headerCreated==false) createHeader();
send(this->clientSocketDescriptor,responseBuffer,len,0);
}

void Response::close()
{
if(this->isClosed) return;
closesocket(clientSocketDescriptor);
isClosed=true;
}

class URLEncoding
{
public:
static string encode(string& data)
{
string encodeData="";
int i;
for(i=0;i<data.length();i++)
{
// Reserved Characters Encoding
if(data[i] == '$') encodeData += "%24";
else if(data[i] == '&') encodeData += "%26";
else if(data[i] == '+') encodeData += "%2b";
else if(data[i] == ',') encodeData += "%2c";
else if(data[i] == '/') encodeData += "%2f";
else if(data[i] == ':') encodeData += "%3a";
else if(data[i] == ';') encodeData += "%3b";
else if(data[i] == '=') encodeData += "%3d";
else if(data[i] == '?') encodeData += "%3f";
else if(data[i] == '@') encodeData += "%40";
// Unsafe Characters Encoding
else if(data[i] == ' ') encodeData += "%20";
else if(data[i] == '"') encodeData += "%22";
else if(data[i] == '<') encodeData += "%3c";
else if(data[i] == '>') encodeData += "%3e";
else if(data[i] == '#') encodeData += "%23";
else if(data[i] == '%') encodeData += "%25";
else if(data[i] == '{') encodeData += "%7b";
else if(data[i] == '}') encodeData += "%7d";
else if(data[i] == '|') encodeData += "%7c";
else if(data[i] == '\\') encodeData += "%5c";
else if(data[i] == '^') encodeData += "%5e";
else if(data[i] == '~') encodeData += "%7e";
else if(data[i] == '[') encodeData += "%5b";
else if(data[i] == ']') encodeData += "%5d";
else if(data[i] == '`') encodeData += "%60";
else if(data[i] == ')') encodeData += "%28";
else if(data[i] == '(') encodeData += "%29";
else encodeData += data[i];
}
return encodeData;
}
static string decode(string& data)
{
string urlEncode;
string decodeData;
int i=0;
while(i<data.length())
{
if(data[i]=='%')
{
urlEncode = data.substr(i,3);
i+=3;
if(urlEncode.compare("%24")==0) decodeData += '$';
else if(urlEncode.compare("%26")==0) decodeData += '&';
else if(urlEncode.compare("%2B")==0) decodeData += '+';
else if(urlEncode.compare("%2C")==0) decodeData += ',';
else if(urlEncode.compare("%2F")==0) decodeData += '/';
else if(urlEncode.compare("%3A")==0) decodeData += ':';
else if(urlEncode.compare("%3B")==0) decodeData += ';';
else if(urlEncode.compare("%3D")==0) decodeData += '=';
else if(urlEncode.compare("%3F")==0) decodeData += '?';
else if(urlEncode.compare("%40")==0) decodeData += '@';
else if(urlEncode.compare("%20")==0) decodeData += ' ';
else if(urlEncode.compare("%22")==0) decodeData += '"';
else if(urlEncode.compare("%3C")==0) decodeData += '<';
else if(urlEncode.compare("%3E")==0) decodeData += '>';
else if(urlEncode.compare("%23")==0) decodeData += '#';
else if(urlEncode.compare("%25")==0) decodeData += '%';
else if(urlEncode.compare("%7B")==0) decodeData += '{';
else if(urlEncode.compare("%7D")==0) decodeData += '}';
else if(urlEncode.compare("%7C")==0) decodeData += '|';
else if(urlEncode.compare("%5C")==0) decodeData += '\\';
else if(urlEncode.compare("%5E")==0) decodeData += '^';
else if(urlEncode.compare("%7E")==0) decodeData += '~';
else if(urlEncode.compare("%5B")==0) decodeData += '[';
else if(urlEncode.compare("%5D")==0) decodeData += ']';
else if(urlEncode.compare("%60")==0) decodeData += '`';
else if(urlEncode.compare("%28")==0) decodeData += '(';
else if(urlEncode.compare("%29")==0) decodeData += ')';
}
else if(data[i]=='+')
{
decodeData += ' ';
i++;
}
else
{
decodeData += data[i];
i++;
}
}
return decodeData;
}
};


int extensionEquals(const char *left,const char *right)
{
char a,b;
a = *left;
b = *right;
while(*left != '\0' && *right != '\0')
{
a = *left;
b = *right;
if(a>=65 && a<=90) a+=32;
if(b>=65 && b<=90) b+=32;
if(a != b) return 0;
left++;
right++;
}
return *left == *right;
}

char *getMimeType(char *resource)
{
char *mimeType;
mimeType = NULL;
int len = strlen(resource);
if(len < 4) return mimeType;
int lastIndexOfDot = len-1;
while(lastIndexOfDot>0 && resource[lastIndexOfDot]!='.') lastIndexOfDot--;
if(lastIndexOfDot == 0) return mimeType;
if(extensionEquals(resource+lastIndexOfDot+1, "html"))
{
mimeType = (char *) malloc(sizeof(char)*10);
strcpy(mimeType,"text/html");
}
if(extensionEquals(resource+lastIndexOfDot+1, "css"))
{
mimeType = (char *) malloc(sizeof(char)*9);
strcpy(mimeType,"text/css");
}
if(extensionEquals(resource+lastIndexOfDot+1, "js"))
{
mimeType = (char *)malloc(sizeof(char)*16);
strcpy(mimeType,"text/javascript");
}
if(extensionEquals(resource+lastIndexOfDot+1, "jpg"))
{
mimeType = (char *) malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+lastIndexOfDot+1, "jpeg"))
{
mimeType = (char *) malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
if(extensionEquals(resource+lastIndexOfDot+1, "png"))
{
mimeType = (char *) malloc(sizeof(char)*10);
strcpy(mimeType,"image/png");
}
if(extensionEquals(resource+lastIndexOfDot+1, "ico"))
{
mimeType = (char *) malloc(sizeof(char)*13);
strcpy(mimeType,"image/x-icon");
}
return mimeType;
}

char isClientSideResource(char *resource)
{
int i;
for(i=0; resource[i] != '\0' && resource[i] != '.'; i++);
if(resource[i] == '\0') return 'N';
return 'Y'; // this will have to be changed later on
}

Request* parseRequest(char * bytes)
{
Request  *request;
request = (Request *)malloc(sizeof(Request));
char method[11];
char resource[1001];
int i,j;
for(i=0; bytes[i] != ' '; i++) method[i] = bytes[i];
method[i] = '\0';
i += 2;


char **data = NULL;
int dataCount = 0;
if(strcmp(method,"GET") == 0)
{
// whatever?dfkdsf=dsfdskf&dskfsdlfj=sdfiessd&dsfk=sdfksd
for(j=0; bytes[i] != ' '; j++,i++)
{
if(bytes[i] == '?') break;
resource[j] = bytes[i];
}
resource[j] = '\0';
if(bytes[i] == '?')
{
i++;
int si = i;
dataCount = 0;
while(bytes[i] != ' ')
{
if(bytes[i] == '&') dataCount++;
i++;
}
dataCount++;
data = (char **)malloc(sizeof(char *)*dataCount);
int *pc =(int *)malloc(sizeof(int)*dataCount);
i=si;
int j=0;
while(bytes[i]!=' ')
{
if(bytes[i]=='&')
{
pc[j] = i;
j++;
}
i++;
}
pc[j] = i;
i=si;
j=0;
int howManyToPick;
while(j<dataCount)
{
howManyToPick=pc[j]-i;
data[j]=(char *)malloc(sizeof(char)*(howManyToPick+1));
strncpy(data[j],bytes+i,howManyToPick);
data[j][howManyToPick] = '\0';
i=pc[j]+1;
j++;
}
free(pc);
}
}// method is of GET type

/*if(strcmp(method,"GET")==0)
{
for(j=0; bytes[i] != ' '; j++,i++)
{
if(bytes[i] == '?') break;
resource[j] = bytes[i];
}
resource[j] = '\0';
if(bytes[i] == '?')
{
i++;
string key="";
string value="";
while(bytes[i] != ' ')
{
if(bytes[i]=='=')
{
key = URLEncoding::decode(key);
i++;
while(bytes[i] != ' ' && bytes[i] != '&')
{
value += bytes[i];
i++;
}
value = URLEncoding::decode(value);
req->set(key,value);
key = "";
value = "";
}
if(bytes[i] == ' ') break;
if(bytes[i] == '&') i++;
key += bytes[i];
i++;
}
}
}*/


request->method = (char *) malloc((sizeof(char)*strlen(method))+1);
request->dataCount = dataCount;
request->data = data;
//request->req = req;
strcpy(request->method,method);
if(resource[0] == '\0')
{
request->resource = NULL;
request->isClientSideTechnologyResource = 'Y';
request->mimeType = NULL;
}
else
{
request->resource = (char *) malloc((sizeof(char)*strlen(resource))+1);
strcpy(request->resource,resource);
request->isClientSideTechnologyResource = isClientSideResource(resource);
request->mimeType = getMimeType(resource);
}
return request;
}




TMWebProjector::TMWebProjector(int portNumber)
{
this->portNumber = portNumber;
this->url = NULL;
this->ptrOnRequest = NULL;
}

TMWebProjector::~TMWebProjector()
{
if(this->url) delete [] this->url;
}

void TMWebProjector::onRequest(const char *url, void(*ptrOnRequest)(Request&,Response&))
{
this->url == NULL;
this->ptrOnRequest = NULL;
if(url == NULL || ptrOnRequest == NULL) return;
this->url = (char *)malloc(strlen(url)*sizeof(char));
strcpy(this->url, url);
this->ptrOnRequest = ptrOnRequest;
}

void TMWebProjector::start()
{
FILE *f;
int i,rc;
long length;
WSADATA wsaData;
WORD ver;
int serverSocketDescriptor;
int clientSocketDescriptor;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
int successCode;
int bytesExtracted;
char responseBuffer[1024];  // 1024 bytes
char requestBuffer[8192]; // 1024 * 8 = 8192
int len;
ver = MAKEWORD(1,1);
WSAStartup(ver,&wsaData); // socket library initialized
serverSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
if(serverSocketDescriptor < 0)
{
printf("Unable to create socket\n");
WSACleanup();
return;
}
serverSocketInformation.sin_family = AF_INET;
serverSocketInformation.sin_port = htons(this->portNumber);
serverSocketInformation.sin_addr.s_addr = htonl(INADDR_ANY);
successCode = bind(serverSocketDescriptor, (struct sockaddr *)&serverSocketInformation, sizeof(serverSocketInformation));
if(successCode < 0)
{
printf("Unable to bind socket\n");
WSACleanup();
return;
}
listen(serverSocketDescriptor, 10);
len = sizeof(clientSocketInformation);
while(1)
{
printf("Server is ready to accept request at port no. %d\n", this->portNumber);
clientSocketDescriptor = accept(serverSocketDescriptor, (struct sockaddr *)&clientSocketInformation, &len);
if(clientSocketDescriptor < 0)
{
printf("Unable to connect with client\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
bytesExtracted = recv(clientSocketDescriptor, requestBuffer, 8192, 0);
requestBuffer[bytesExtracted] = '\0';
if(bytesExtracted < 0)
{
// what to do is yet to be decided
}
else if(bytesExtracted == 0)
{
// what to do is yet to be decided
}
else
{
Request *request;
request = parseRequest(requestBuffer);
printf("Recieved request for : %s\n", request->resource);
if(request->isClientSideTechnologyResource == 'Y')
{
if(request->resource == NULL)
{
f = fopen("index.html","rb");
if(f == NULL)
{
f = fopen("index.htm","rb");
}
if(f == NULL) // file is not present
{
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:162\nConnection:close\n\n<!DOCTYPE HTML><html lang = 'en'><head><meta charset = 'utf-8'><title>My Web Projector</title></head><body><h1 style = 'color:red'>Resource / not found</h1></body></html>");
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
printf("1 Sending 404 error code\n");
}
else // file is present
{
fseek(f,0,2);
length = ftell(f);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%ld\nConnection:close\n\n",length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
printf("Response sent for : %s\n", request->resource);
fseek(f,0,0);
i = 0;
while(i<length)
{
rc = length - i;
if(rc > 1024) rc = 1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,1024,0);
i += 1024;
}
fclose(f);
closesocket(clientSocketDescriptor);
}
}
else // when request->resource != NULL
{
f = fopen(request->resource,"rb");
if(f == NULL) // file is not present
{
char tmp[501];
printf("2 Sending 404 error page\n");
sprintf(tmp,"<!DOCTYPE HTML><html lang = 'en'><head><meta charset = 'utf-8'><title>My Web Projector</title></head><body><h1 style = 'color:red'>Resource /%s not found</h1></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection:close\n\n",strlen(requestBuffer));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else // file is present
{
fseek(f,0,2);
length = ftell(f);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%ld\nConnection:close\n\n",request->mimeType,length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
fseek(f,0,0);
i = 0;
while(i<length)
{
rc = length - i;
if(rc > 1024) rc = 1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,1024,0);
i+=1024;
}
printf("Response sent for : %s\n", request->resource);
fclose(f);
}
closesocket(clientSocketDescriptor);
}
}
else // more changes to do in context to server side resource
{
if(this->url == NULL || this->ptrOnRequest == NULL)
{
char tmp[501];
printf("3 Sending 404 error page\n");
sprintf(tmp,"<!DOCTYPE HTML><html lang = 'en'><head><meta charset = 'utf-8'><title>My Web Projector</title></head><body><h1 style = 'color:red'>Resource /%s not found</h1></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nConnection:close\n\n");
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
closesocket(clientSocketDescriptor);
}
else
{
Response response(clientSocketDescriptor);
int ii=0;
if(this->url[0] == '/') ii=1;
if(strcmp(this->url+ii,request->resource) == 0)
{
this->ptrOnRequest(*request, response);
if(request->data != NULL)
{
for(int x=0; x<request->dataCount; x++) free(request->data[x]);
free(request->data);
}
/*printf("Sending processed page\n");
char tmp[501];
sprintf(tmp,"<!DOCTYPE HTML><html lang = 'en'><head><meta charset = 'utf-8'><title>My Web Projector</title></head><body><h1 style = 'color:red'>Resource /%s processed</h1></body></html>",rq.getResource().c_str());
closesocket(clientSocketDescriptor);*/
}
else
{
printf("Sending 404 error page\n");
char tmp[501];
sprintf(tmp,"<!DOCTYPE HTML><html lang = 'en'><head><meta charset = 'utf-8'><title>My Web Projector</title></head><body><h1 style = 'color:red'>Resource /%s not found</h1></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nConnection:close\n\n");
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
closesocket(clientSocketDescriptor);
}
}
}
free(request->method);
free(request->resource);
free(request->mimeType);
free(request);
}
}
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}