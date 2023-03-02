
tolua++ -o tolua_file.cpp tolua.pkg

echo #include "pch.h"> temp.cpp
echo.>> temp.cpp
type tolua_file.cpp >> temp.cpp

DEL tolua_file.cpp
RENAME temp.cpp tolua_file.cpp