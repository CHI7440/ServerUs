#include<windows.h>
#include<io.h>
#include<iostream>
#include<fstream>
#include<map>
#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<serverus>
#include<vector>
using namespace std;
using namespace serverus;

char *getMimeType(char *resource);

Request::Request()
{
method = NULL;
resource = NULL;
isClientSideTechnologyResource = 'Y';
mimeType = NULL;
dataCount = 0;
data = NULL;
forwardTo = NULL;
}

void Request::setAttribute(string name, string value)
{
if(name.length()==0 || value.length()==0) return;
attributesMapping.insert(pair<string,string>(name,value));
}

string Request::getAttribute(string name)
{
map<string,string>::iterator it = attributesMapping.find(name);
if(it == attributesMapping.end()) return "";
return it->second;
}

string Request::get(string name)
{
for(int i=0; i<this->dataCount; i++)
{
int keyIndex = 0;
while(this->data[i][keyIndex] != '=' && this->data[i][keyIndex] != '\0') keyIndex++;
string key(data[i],keyIndex);
if(key.compare(name)==0)
{
string value(data[i]+keyIndex+1);
return URLEncoding::decode(value);
}
}
return "";
}

void Request::forward(const char* resource)
{
printf("Forwarded resource : %s\n",resource);
if(resource == NULL) return;
int len = strlen(resource);
if(len == 0) return;
forwardTo = (char *)malloc((len+1)*sizeof(char));
strcpy(forwardTo,resource);
}

Response::Response(int clientSocketDescriptor)
{
this->clientSocketDescriptor = clientSocketDescriptor;
this->isClosed.insert({clientSocketDescriptor,false});
this->headerCreated = false;
printf("Opening Client Socket Descriptor for : %d\n",clientSocketDescriptor);
}

void Response::createHeader()
{
char header[51];
strcpy(header,"HTTP/1.1 200 OK\nContent-Type:text/html\n\n");
send(this->clientSocketDescriptor,header,strlen(header),0);
this->headerCreated = true;
}

void Response::write(string responseBuffer)
{
if(responseBuffer.length()==0) return;
int len = responseBuffer.length();
if(this->headerCreated==false) createHeader();
send(this->clientSocketDescriptor,responseBuffer.c_str(),len,0);
}

void Response::close()
{
if(this->isClosed[this->clientSocketDescriptor]) return;
printf("Closing client socket descriptor for : %d\n",clientSocketDescriptor);
closesocket(this->clientSocketDescriptor);
this->isClosed[this->clientSocketDescriptor] = true;
}

string URLEncoding::encode(string data)
{
string decodedData = string(data);
string encodeData="";
int i;
for(i=0;i<decodedData.length();i++)
{
// Reserved Characters Encoding
if(decodedData[i] == '$') encodeData += "%24";
else if(decodedData[i] == '&') encodeData += "%26";
else if(decodedData[i] == '+') encodeData += "%2b";
else if(decodedData[i] == ',') encodeData += "%2c";
else if(decodedData[i] == '/') encodeData += "%2f";
else if(decodedData[i] == ':') encodeData += "%3a";
else if(decodedData[i] == ';') encodeData += "%3b";
else if(decodedData[i] == '=') encodeData += "%3d";
else if(decodedData[i] == '?') encodeData += "%3f";
else if(decodedData[i] == '@') encodeData += "%40";
// Unsafe Characters Encoding
else if(decodedData[i] == ' ') encodeData += "%20";
else if(decodedData[i] == '"') encodeData += "%22";
else if(decodedData[i] == '<') encodeData += "%3c";
else if(decodedData[i] == '>') encodeData += "%3e";
else if(decodedData[i] == '#') encodeData += "%23";
else if(decodedData[i] == '%') encodeData += "%25";
else if(decodedData[i] == '{') encodeData += "%7b";
else if(decodedData[i] == '}') encodeData += "%7d";
else if(decodedData[i] == '|') encodeData += "%7c";
else if(decodedData[i] == '\\') encodeData += "%5c";
else if(decodedData[i] == '^') encodeData += "%5e";
else if(decodedData[i] == '~') encodeData += "%7e";
else if(decodedData[i] == '[') encodeData += "%5b";
else if(decodedData[i] == ']') encodeData += "%5d";
else if(decodedData[i] == '`') encodeData += "%60";
else if(decodedData[i] == ')') encodeData += "%28";
else if(decodedData[i] == '(') encodeData += "%29";
else encodeData += decodedData[i];
}
return encodeData;
}

string URLEncoding::decode(string data)
{
string encodedData = string(data);
string urlEncode;
string decodeData;
int i=0;
while(i<encodedData.length())
{
if(encodedData[i]=='%')
{
urlEncode = encodedData.substr(i,3);
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
else if(encodedData[i]=='+')
{
decodeData += ' ';
i++;
}
else
{
decodeData += encodedData[i];
i++;
}
}
return decodeData;
}


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
if(extensionEquals(resource+lastIndexOfDot+1,"tpl"))
{
mimeType = (char *) malloc(sizeof(char)*9);
strcpy(mimeType,"text/tpl");
}
printf("%s\n",mimeType);
return mimeType;
}

char isClientSideResource(char *resource)
{
int i;
for(i=0; resource[i] != '\0' && resource[i] != '.'; i++);
if(resource[i] == '\0') return 'N';
if(resource[i]=='.' && strcmp(getMimeType(resource),"text/tpl")==0) return 'N';
return 'Y';
}

Request* parseRequest(char * bytes)
{
// REQUEST FORMAT : [Method Type(GET)] /[Resource Name(if not given so null)] HTTP/1.1
// GET /index.html HTTP/1.1
Request  *request;
request = new Request();
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


void ServerUs::clientSide(Request *request)
{
FILE *f;
char responseBuffer[1024];  // 1024 bytes
int i,rc,length;
if(request->resource == NULL)
{
f = fopen("index.html","rb");
if(f == NULL)// index.html file is not present
{
f = fopen("index.htm","rb");
}
if(f == NULL) // index.htm file is not present
{
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:162\nConnection:close\n\n<!DOCTYPE HTML><html lang = 'en'><head><meta charset = 'utf-8'><title>My Web Projector</title></head><body><h1 style = 'color:red'>Resource / not found</h1></body></html>");
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
printf("Sending 404 error code\n");
}
else // index.html file is present
{
fseek(f,0,2);
length = ftell(f);

sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection:close\n\n",length);
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
if(f == NULL) // request->resource file is not present
{
char tmp[501];
printf("Sending 404 error page\n");
sprintf(tmp,"<!DOCTYPE HTML><html lang = 'en'><head><meta charset = 'utf-8'><title>My Web Projector</title></head><body><h1 style = 'color:red'>Resource /%s not found</h1></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nConnection:close\n\n");
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else // request->resource file is present
{
fseek(f,0,2);
length = ftell(f);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%d\nConnection:close\n\n",request->mimeType,length);
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



void ServerUs::serverSide(Request* request)
{
char responseBuffer[1024];  // 1024 bytes
Response response(clientSocketDescriptor);
printf("Server side resource check : %s\n",request->resource);
map<string,void(*)(Request&,Response&)>::iterator it = allRequests.find(request->resource);
if(it == allRequests.end())
{
char tmp[501];
printf("Sending 404 error page\n");
sprintf(tmp,"<!DOCTYPE HTML><html lang = 'en'><head><meta charset = 'utf-8'><title>My Web Projector</title></head><body><h1 style = 'color:red'>Resource /%s not found</h1></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nConnection:close\n\n");
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
else
{
it->second(*request, response);
printf("function ended reached here\n");
if(request->forwardTo != NULL && strlen(request->forwardTo) > 0)
{
printf("forwarded to : %s\n",request->forwardTo);
int ii=0;
if(request->forwardTo[0] == '/') ii=1;
int len = strlen(request->forwardTo+ii);
free(request->resource);
request->resource = (char *)malloc(sizeof(char)*(len+1));
strcpy(request->resource,request->forwardTo+ii);
request->forwardTo = NULL;
free(request->mimeType);
request->mimeType = NULL;
char *mimeType = getMimeType(request->resource);
if(mimeType != NULL)
{
request->mimeType = (char *)malloc(sizeof(char)*(strlen(mimeType)+1));
strcpy(request->mimeType,mimeType);
}
request->isClientSideTechnologyResource = isClientSideResource(request->resource);
if(request->isClientSideTechnologyResource=='Y')
{
this->clientSide(request);
}
else
{
this->serverSide(request);
}
}
}
printf("Just Before Response close\n");
response.close();
if(request->data != NULL)
{
for(int x=0; x<request->dataCount; x++) free(request->data[x]);
free(request->data);
request->data = NULL;
}
}


ServerUs::ServerUs(int portNumber)
{
this->portNumber = portNumber;
this->clientSocketDescriptor = -1;
this->serverSocketDescriptor = -1;
}


void ServerUs::onRequest(string url, void(*ptrOnRequest)(Request&,Response&))
{
if(url.length()==0|| ptrOnRequest == NULL) return;
int ii=0;
if(url[0]=='/') ii=1;
allRequests[url.substr(ii)] = ptrOnRequest;
}

void ServerUs::start()
{
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
this->serverSocketDescriptor = serverSocketDescriptor;
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
this->clientSocketDescriptor = clientSocketDescriptor;
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
this->clientSide(request);
}
else // more changes to do in context to server side resource
{
printf("Server Side function Starts\n");
this->serverSide(request);
printf("Server Side reached here after multiple recursions\n");
}
printf("Just above free request\n");
free(request->method);
free(request->resource);
free(request->mimeType);
delete(request);
}
}
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}