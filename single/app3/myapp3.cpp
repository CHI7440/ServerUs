#include<iostream>
#include<serverus>
using namespace serverus;
using namespace std;

int main()
{
ServerUs server(5050);
server.start();
return 0;
}