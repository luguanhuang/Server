cpp\protoc.exe --cpp_out=..\..\protocol\pb %2
cpp\protoc.exe --cpp_out=..\..\protocol\pb %3
REM cpp\protoc.exe --python_out=E:\MyTools %1
csharp\protogen.exe -i:%1 -o:..\..\..\client\XMainClient\XMainClient\Network\protocol\protodefine.cs -p:detectMissing
csharp\protogen.exe -i:%1 -o:..\RobotTest\RobotTest\Network\protocol\protodefine.cs
