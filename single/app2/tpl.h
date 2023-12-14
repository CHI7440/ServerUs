#ifndef ___TPL
#define ___TPL 123
#include<serverus>
using namespace serverus;
void getErrorMessage_TPL(Request&, Response&);
void registerTPLs(ServerUs *server)
{
server->onRequest("/errorMessage.tpl",getErrorMessage_TPL);
}
#endif