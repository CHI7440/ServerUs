g++ -c serverus.cpp -I ..\include
ar rcs serverus.lib *.o
del ..\lib\serverus.lib
move serverus.lib ..\lib