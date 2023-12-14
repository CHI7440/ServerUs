#include<tmwp>
#include<iostream>
using namespace tmwp;
using namespace std;
void doSomething(Request& req, Response& response)
{
cout<<"Great, this code gets executed on server side"<<endl;
string n = req.get("nm");
string c = req.get("ct");
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Whatever</title>");
response.write("</head>");
response.write("<body>");
response.write("Hello ");
response.write(n);
response.write(" from ");
response.write(c);
response.write("<br>");
response.write("Data Saved");
response.write("</body>");
response.write("</html>");
//req.forward("thankYou.html");
response.close();
}
int main()
{
TMWebProjector server(5050);
server.onRequest("/addStudent",doSomething);
server.start();
return 0;
}