
# ServerUs

![ServerUs](https://github.com/CHI7440/ServerUs/assets/104809802/239fe46b-16b6-4ff6-b794-c13abd472382)


ServerUs is a single threaded webserver made in C++ used to deploy static and dynamic web application. The webserver also supports for the template html code which helps in adding the dynamic message to a web application.


## Prerequisites

* C++11 or higher versions
* MingW64 is preferred
## Getting Started

* Install the zip file and unzip it to the required location of your computer.

* Note that the main HTML file name should be `index.html` or `index.htm`


## How to use ServerUs?

To use ServerUs I have shared 3 example folders to get an example overview.

Although its quite easy to use ServerUs and I'm going to share each and every small step to give a better picture of the application.

### Deploying static websites

Static websites in simple terms are those web applications which doesn't requires the server side processing of data.

So for those websites first of all you have to create a folder where your HTML and other files will be there.

For instance in my examples I have create a folder named app3 which consists of a portfolio website having all the necessary files.

Now in that folder create a cpp file which will have the required code to start the server.

In portfolio website lets see what the code can be :
```c++
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
```

In the above code `server` is the object of class ServerUs with `5050` as a port number after that `server.start()` is to start the server. Also it is compulsory to include `serverus` header and to use the namespace `serverus`

### Deploying Dynamic Websites

Now deploying dynamic websites which requires the code to get implemented on server end.

So in that case ServerUs provides the onRequest method :
```c++
void onRequest(string url, void(*ptrOnRequest)(Request&,Response&))
```

The `onRequest` method takes two arguments, first one is the name of url associated with href link in the HTML code and second one is the function address of `void` return type and taking the instance of `Request` and `Response` class as an arguments.

The function will act as a mapping of the href link where you can write the code in c++ to perform any internal operation.

Also the class Request consists of get method:
```c++
string get(string name)
```
This method will simply return the value to key you have used in the form tag in HTML.

And the Response method consists of write method:
```c++
void write(string responseBuffer)
```
which is used to write the HTML code on the browser from the function.

Now Let's make it more clear by considering the app1 folder example:

We are creating a web application which will take the Roll Number and Name of Student and will put in the file at the server end. We are also providing the view, edit and delete student facility in that application.

__index.html__
```html
<DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset = 'UTF-8'>
<title>Monko School</title>
</head>
<body>
<a href='StudentAddForm.html'>Add Student</a><br>
<a href='getStudents'>View Students</a><br>
<a href='StudentEditForm.html'>Edit Student</a><br>
<a href='StudentDeleteForm.html'>Delete Student</a><br>
</body>
</html>
```
Here the `Add Student` redirects to `StudentAddForm.html`

`View Students` redirects to getStudents. Here the `getStudents` is nothing but a key to map with the function

`Edit Student` and `Delete Student` will redirect to `StudentEditForm.html` and `StudentDeleteForm.html` respectively

__StudentAddForm.html__
```html
<DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset = 'UTF-8'>
<title>Monko School</title>
</head>
<body>
<h1> Student (ADD MODULE) </h1>
<form method = 'get' action = 'addStudent'>
Roll Number <input type='text' id='rn' name='rn'><br>
Name <input type='text' id='nm' name='nm'><br>
<button type='submit'>Add</button>
</form>
<a href='index.html'>Home</a>
</body>
</html>
```
In this html file there is form which have `nm` and `rn` as the name to send with the url during submission. These names will act as key and the data entered in that field will act as value which can be extracted from `get` method of `Request` class in the cpp function.

Same goes with `StudentEditForm.html` and `StudentDeleteForm.html`.

Now `myapp1.cpp` is where we will write the logic to add,edit,view and delete the data form file also mapping the various requests. You can refer to myapp1.cpp from the app1 folder.

The code just consists of the logic for basic operation on the file and the main function has mapped all the Request URLs to there respective function. Here in various functions we have used `write` method of Response class to directly perform web related operation from cpp file.

__Note__   The URL Mapping with the help of `onRequest` method should be done before starting the server, otherwise the later mappings will have no importance.

### Request Forwarding and Template files

Request Forwarding is the feature of ServerUs which helps the programmer to forward the request to different function or file. This can be done by the `forward` method of Request class.

Template Files are the files with `.tpl` extension which are used to add dynamic messaging to web application

To implement the concept of template files:
1.  Set the attributes using `setAttribute` method:
```c++
void setAttribute(string name, string value)
```
in the main cpp file which takes name-value pair as an argument.

2. Now create a file with `.tpl` extension where you can write the basic html code but the name of the attribute should be like this : `$$${[name_of_attribute]}`

3. There is executable file named `TPLFileParser` present in the exe folder. Run that file on the current working folder. 

4. Now in main cpp file add the following line of code:
4.1 `#include"tpl.h"` as a header.

4.2 `registerTPLs(&[address_of_instanceof_ServerUs])` just before starting the server.

Here is the basic example of file with `.tpl` extension: 

__errorMessage.tpl__

```html
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset="utf-8">
<title>My Cool Website</title>
</head>
<body>
<h1>
$$${errorMessage}
</h1>
</body>
</html>
```
You can refer to app2 folder for more clarification.

### How to Compile?

Now to compile the code open the command prompt for the current working folder and write the following command : 
```c++
g++ -static *.cpp -o [file name].exe -I [location of the include folder] -L [location of the lib folder] -lserverus -lws2_32
```

for example in my example the lib and include folder are in serverus folder so the command will be :
```c++
g++ -static *.cpp -o myapp3.exe -I ..\serverus\include -L ..\serverus\lib -lserverus -lws2_32
```
And then run the code just by writing the name of executable file. 

After that server will get start and now in the browser writing `localhost:[portnumber]` will deploy your website on the browser
You can also access the web application from remote devices just by typing the `ipv4:portnumber` in your remote device browser URL.
You can also use Dynamic DNS Service to provide your server a dynamic domain in case if your ip address changes periodically.

## Application

* __Portfolio website__: Showcase your skills and experience with a personal website built on ServerUs. You can host your resume, projects, and contact information, offering a dynamic platform for self-promotion.
* __Static file hosting for a blog__: If you have a blog built with another technology like Python or PHP, you can use ServerUs to host its static assets like images, CSS, and JavaScript files. This can offload some workload and improve performance.
* __Internal team dashboard__: Develop a web dashboard for your team to access shared documents, track progress, and communicate updates. ServerUs can host the necessary HTML, CSS, and JavaScript files for the dashboard interface.


## Future Updations

* Currently the server supports only __GET__ Request but the future version will be able to handle various HTTP requests like : __POST, PUT, DELETE, HEAD__ etc.

* The future updations of the ServerUs will support the __multithreading__ which will help to deploy the sites simultaneously.
