#include<serverus>
#include<iostream>
#include<fstream>
#include<string.h>
using namespace serverus;
using namespace std;
//void doSomething(int dataCount, char *data[])

class Student
{
public:
char rollNumber[100];
char name[100];
};

void addStud(Request& request, Response& response)
{
Student s;
string rollNumber = request.get("rn");
cout<<rollNumber<<endl;
string name = request.get("nm");
strcpy(s.rollNumber, rollNumber.c_str());
strcpy(s.name, name.c_str());
fstream f("Student.data",ios::app|ios::binary);
f.write((char *)&s, sizeof(Student));
f.close();
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>Student ");
response.write(rollNumber.c_str());
response.write(" : ");
response.write(name.c_str());
response.write(" added</h3><br>");
response.write("<a href = 'index.html'>Ok</a>");
response.write("</body>");
response.write("</html>");
response.close();
}

void getAllStudents(Request& request, Response& response)
{
fstream f("Student.data",ios::in|ios::binary);
if(f.fail())
{
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>No Student Added<br>");
response.write("<a href = 'index.html'>Ok</a>");
response.write("</body>");
response.write("</html>");
response.close();
return;
}
f.seekg(0);
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<table>");
response.write("<tr>");
response.write("<th>Roll number</th>");
response.write("<th>Name</th>");
response.write("</tr>");
Student s;
while(!f.eof())
{
f.read((char*)&s, sizeof(Student));
if(f.fail()) break;
response.write("<tr>");
response.write("<td>");
response.write(s.rollNumber);
response.write("</td>");
response.write("<td>");
response.write(s.name);
response.write("</td>");
response.write("</tr>");
}
response.write("</table>");
response.write("<br>");
response.write("<a href = 'index.html'>Ok</a>");
response.write("</body>");
response.write("</html>");
f.close();
response.close();
}


void studentEditForm(Request& request, Response& response)
{
Student s;
string rollNumber = request.get("rn");
string name;
fstream f("Student.data", ios::out|ios::in|ios::binary);
if(f.fail())
{
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>No Student Found<br>");
response.write("<a href = 'index.html'>Ok</a>");
response.write("</body>");
response.write("</html>");
response.close();
f.close();
return;
}
else
{
int pos;
int flag = 0;
while(!f.eof())
{
pos = f.tellp();
f.read((char *)&s, sizeof(Student));
if(f.fail())
{
f.close();
break;
}
if(strcmp(s.rollNumber,rollNumber.c_str())==0)
{
f.seekp(pos);
name = s.name;
flag = 1;
f.close();
break;
}
}
if(flag==0)
{
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>No Student Found<br>");
response.write("<a href = 'index.html'>Ok</a>");
response.write("</body>");
response.write("</html>");
response.close();
return;
}
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1>Student (EDIT MODULE)</h1>");
response.write("<form method = 'get' action = 'updateStudent'>");
response.write("Roll Number:");
response.write(rollNumber.c_str());
response.write(" <input type='hidden' id='rn' name='rn', value='");
response.write(rollNumber.c_str());
response.write("'><br>");
response.write("Name <input type='text' id='nm' name='nm' value='");
response.write(name.c_str());
response.write("'><br>");
response.write("<button type='submit'>Update</button>");
response.write("</form>");
response.write("<a href = 'index.html'>Home</a>");
response.write("</body>");
response.write("</html>");
response.close();
}
}

void studentUpdate(Request& request, Response& response)
{
fstream f("Student.data", ios::out|ios::in|ios::binary);
Student s;
string rollNumber = request.get("rn");
string name = request.get("nm");
int pos;
while(!f.eof())
{
pos = f.tellp();
f.read((char *)&s, sizeof(Student));
if(f.fail()) break;
if(strcmp(s.rollNumber,rollNumber.c_str())==0)
{
f.seekp(pos);
strcpy(s.rollNumber,rollNumber.c_str());
strcpy(s.name,name.c_str());
f.write((char *)&s, sizeof(Student));
f.close();
break;
}
}
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>Student Updated<br>");
response.write("<a href = 'index.html'>Ok</a>");
response.write("</body>");
response.write("</html>");
response.close();
return;
}





void studentDeleteForm(Request& request, Response& response)
{
Student s;
string rollNumber = request.get("rn");
string name;
fstream f("Student.data", ios::out|ios::in|ios::binary);
if(f.fail())
{
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>No Student Found<br>");
response.write("<a href = 'index.html'>Ok</a>");
response.write("</body>");
response.write("</html>");
response.close();
f.close();
return;
}
else
{
int flag = 0;
while(!f.eof())
{
f.read((char *)&s, sizeof(Student));
if(f.fail())
{
f.close();
break;
}
if(strcmp(s.rollNumber,rollNumber.c_str())==0)
{
name = s.name;
flag = 1;
f.close();
break;
}
}
if(flag==0)
{
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>No Student Found<br>");
response.write("<a href = 'index.html'>Ok</a>");
response.write("</body>");
response.write("</html>");
response.close();
return;
}
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<h1>Student (DELETE MODULE)</h1>");
response.write("<form method = 'get' action = 'confirmDeleteStudent'>");
response.write("Roll Number: ");
response.write(rollNumber.c_str());
response.write(" <input type='hidden' id='rn' name='rn', value='");
response.write(rollNumber.c_str());
response.write("'><br>");
response.write("Name: ");
response.write(name.c_str());
response.write(" <input type='hidden' id='nm' name='nm' value='");
response.write(name.c_str());
response.write("'><br>");
response.write("<button type='submit'>Confirm Delete</button>");
response.write("</form>");
response.write("<a href = 'index.html'>Home</a>");
response.write("</body>");
response.write("</html>");
response.close();
}
}

void studentDelete(Request& request, Response& response)
{
string rollNumber = request.get("rn");
string name = request.get("nm");
ifstream file("Student.data", ios::in|ios::binary);
Student s;
ofstream temp("temp.data", ios::out|ios::binary);
while(!file.eof())
{
file.read((char *)&s, sizeof(Student));
if(file.fail()) break;
if(strcmp(s.rollNumber,rollNumber.c_str())!=0)
{
temp.write((char*)&s, sizeof(Student));
}
}
temp.close();
file.close();
ifstream t("temp.data",ios::in|ios::binary);
ofstream f("Student.data",ios::out|ios::binary);
while(true)
{
t.read((char *)&s, sizeof(Student));
if(t.fail()) break;
cout<<s.rollNumber<<", "<<s.name<<endl;
f.write((char *)&s, sizeof(Student));
}
t.close();
f.close();
t.open("temp.data",ios::out|ios::binary);
t.close();
response.write("<!DOCTYPE HTML>");
response.write("<html>");
response.write("<head>");
response.write("<meta charset='utf-8'>");
response.write("<title>Monko School</title>");
response.write("</head>");
response.write("<body>");
response.write("<h3>Student Deleted<br>");
response.write("<a href = 'index.html'>Ok</a>");
response.write("</body>");
response.write("</html>");
response.close();
}


int main()
{
ServerUs server(5050);
server.onRequest("/addStudent",addStud);
server.onRequest("/getStudents",getAllStudents);
server.onRequest("/editStudent",studentEditForm);
server.onRequest("/updateStudent",studentUpdate);
server.onRequest("/deleteStudent",studentDeleteForm);
server.onRequest("/confirmDeleteStudent",studentDelete);
server.start();
return 0;
}