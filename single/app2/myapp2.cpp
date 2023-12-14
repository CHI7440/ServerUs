#include<iostream>
#include<serverus>
#include "tpl.h"
using namespace serverus;
using namespace std;

void one(Request& request, Response& response)
{
cout<<"Function One got called"<<endl;
request.forward("/xyz.html");
}

void two(Request& request, Response& response)
{
cout<<"Function Two got called"<<endl;
request.setAttribute("fromRequest","two");
request.forward("/three");
}

void three(Request& request, Response& response)
{
cout<<"Function Three got called"<<endl;
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Whatever</title>");
response.write("</head>");
response.write("<body>");
response.write("<form action='/getCar'>");
response.write("Select Car<br>");
response.write("<select name='carCode' id='car'>");
response.write("<option value='-1'>-Select-</option>");
response.write("<option value='1'>Volvo</option>");
response.write("<option value='2'>Saab</option>");
response.write("<option value='3'>Mercedes</option>");
response.write("<option value='4'>Audi</option>");
response.write("</select><br>");
response.write("<button type='submit'>Get Car</button>");
response.write("</form>");
response.write("</body>");
response.write("</html>");
response.close();
}

void getCar(Request& request, Response& response)
{
int carCode = stoi(request.get("carCode"));
if(carCode == -1)
{
request.setAttribute("errorMessage","Invalid Car Choice");
request.forward("/errorMessage.tpl");
}
else if(carCode == 1)  request.forward("/Volvo.html");
else if(carCode == 2)  request.forward("/Saab.html");
else if(carCode == 3)  request.forward("/Mercedes.html");
else if(carCode == 4)  request.forward("/Audi.html");
}

/*void errorMsg(Request& request, Response& response)
{
response.write("<DOCTYPE HTML>");
response.write("<html lang = 'en'>");
response.write("<head>");
response.write("<meta charset = 'utf-8'>");
response.write("<title>My Cool Website</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1>");
response.write(request.getAttribute("errorMessage"));
response.write("</h1>");
response.write("</body>");
response.write("</html>");
response.close();
}*/

int main()
{
ServerUs server(5050);
server.onRequest("/three",three);
server.onRequest("/two",two);
server.onRequest("/one",one);
server.onRequest("/getCar",getCar);
//server.onRequest("/errorMessage",errorMsg);
registerTPLs(&server);
server.start();
return 0;
}