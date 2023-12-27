#include<iostream>
#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<map>
#include<io.h>
using namespace std;


class TPLFileParser
{
map<string,string> fileFunctionMap;
WIN32_FIND_DATA fileData;
HANDLE fileHandle;
public:
TPLFileParser()
{
this->fileHandle = FindFirstFile("*.tpl",&this->fileData);
if(this->fileHandle == INVALID_HANDLE_VALUE) return;
while(true)
{
tpl2C(fileData.cFileName);
if(FindNextFile(fileHandle,&fileData) == 0) break;
}
FindClose(fileHandle);
createHeaderFile();
}


void createHeaderFile()
{
FILE *headerFile;
headerFile = fopen("tpl.h","r");
if(headerFile != NULL)
{
remove("tpl.h");
}
headerFile = fopen("tpl.h","w");
fputs("#ifndef ___TPL\n",headerFile);
fputs("#define ___TPL 123\n",headerFile);
fputs("#include<serverus>\n",headerFile);
fputs("using namespace serverus;\n",headerFile);
map<string,string>::iterator it = fileFunctionMap.begin();
while(it!=fileFunctionMap.end())
{
fputs("void ",headerFile);
fputs(it->second.c_str(),headerFile);
fputs("(Request&, Response&)",headerFile);
fputs(";\n",headerFile);
++it;
}
fputs("void registerTPLs(ServerUs *server)\n",headerFile);
fputs("{\n",headerFile);
it = fileFunctionMap.begin();
while(it!=fileFunctionMap.end())
{
fputs("server->onRequest(\"/",headerFile);
fputs(it->first.c_str(),headerFile);
fputs("\",",headerFile);
fputs(it->second.c_str(),headerFile);
fputs(");\n",headerFile);
++it;
}
fputs("}\n",headerFile);
fputs("#endif",headerFile);
fclose(headerFile);
}


void tpl2C(string fileName)
{
FILE *f;
f = fopen(fileName.c_str(),"r");
fseek(f,0,2);
int length = ftell(f);
if(length == 0)
{
printf("Length of file is zero\n");
fclose(f);
return;
}
fseek(f,0,0);
length = fileName.length();
int i=length-1;
int indexOfDot=0;
while(i>=0)
{
if(fileName[i]=='.')
{
indexOfDot=i;
break;
}
i--;
}
string newFileName = fileName + ".cpp";
string functionName = string("get")+fileName.substr(0,indexOfDot)+"_TPL";
if(functionName[3]>=97 && functionName[3]<=122) functionName[3] = functionName[3]-32;
pair<string,string> p;
p.first = fileName;
p.second = functionName;
fileFunctionMap.insert(p);
FILE *nf;
nf = fopen(newFileName.c_str(),"r");
if(nf!=NULL)
{
HANDLE handle = (HANDLE)_get_osfhandle(_fileno(nf));
FILETIME newFileLastWriteTime;
bool result = GetFileTime(handle,NULL,NULL,&newFileLastWriteTime);
if(CompareFileTime(&newFileLastWriteTime,&(fileData.ftLastWriteTime))==1)
{
printf("File Already Created\n");
fclose(nf);
fclose(f);
return;
}
remove(newFileName.c_str());
}
nf = fopen(newFileName.c_str(),"w");
fputs("#include<serverus>\n",nf);
fputs("using namespace serverus;\n",nf);
fputs("void ",nf);
fputs(functionName.c_str(),nf);
fputs("(Request& request, Response& response)",nf);
fputs("\n",nf);
fputs("{\n",nf);
char buffer[1024];
while(1)
{
if(fgets(buffer,sizeof(buffer),f)==NULL) break;
buffer[strlen(buffer)-1] = '\0';
fputs("response.write(\"",nf);
int i=0;
for(i=0;i<strlen(buffer);i++)
{
if(buffer[i]=='"')
{
fputc('\\',nf);
fputc(buffer[i],nf);
}
else if(buffer[i]=='$')
{
if(strncmp(buffer+i,"$$${",4)!=0)
{
fputc(buffer[i],nf);
}
else
{
int closingBracesFound = 0;
string key;
i+=4;
while(i<strlen(buffer))
{
if(buffer[i] == ' ') break;
if(buffer[i] == '}')
{
closingBracesFound = 1;
break;
}
key += buffer[i];
i++;
}
if(closingBracesFound==0)
{
int spos = i-key.length()-4;
int len = i-spos+1;
char temp[len+1];
strncpy(temp,buffer+spos,len);
temp[len]='\0';
fputs(temp,nf);
}
else
{
fputs("\"+request.getAttribute(\"",nf);
fputs(key.c_str(),nf);
fputs("\")+\"",nf);
}
}
}
else fputc(buffer[i],nf);
}
fputs("\");\n",nf);
}
fputs("}",nf);
fclose(nf);
fclose(f);
}
};

int main()
{
TPLFileParser *parse = new TPLFileParser();
return 0;
}