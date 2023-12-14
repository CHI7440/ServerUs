#include<serverus>
using namespace serverus;
void getErrorMessage_TPL(Request& request, Response& response)
{
response.write("<!DOCTYPE HTML>");
response.write("<html lang='en'>");
response.write("<head>");
response.write("<meta charset=\"utf-8\">");
response.write("<title>My Cool Website</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1>");
response.write(""+request.getAttribute("errorMessage")+"");
response.write("</h1>");
response.write("</body>");
response.write("</html>");
}