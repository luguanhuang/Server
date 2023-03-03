echo off
echo start

setlocal

set /p update-res=是否更新资源(1:是,0:否):

if "%update-res%" == "1" (

echo  "删除旧的资源"

rmdir /s/q gsconf\table

rmdir /s/q gsconf\Curve
rmdir /s/q gsconf\SkillPackage
rmdir /s/q gsconf\CutScene

echo "复制资源"

mkdir gsconf\table
mkdir gsconf\table\AITree
mkdir gsconf\table\Level
mkdir gsconf\table\SceneBlock
mkdir gsconf\table\WayPoint
mkdir gsconf\table\SuperRisk

mkdir gsconf\CutScene
mkdir gsconf\SkillPackage
mkdir gsconf\Curve
 
xcopy /e /exclude:exclude.txt %XResourcePath%\XProject\Assets\Table gsconf\table\
xcopy /e /exclude:exclude.txt %XResourcePath%\XProject\Assets\Resources\Table\AITree gsconf\table\AITree
xcopy /e /exclude:exclude.txt %XResourcePath%\XProject\Assets\Resources\Table\Level gsconf\table\Level
xcopy /e /exclude:exclude.txt %XResourcePath%\XProject\Assets\Resources\Table\SceneBlock gsconf\table\SceneBlock
xcopy /e /exclude:exclude.txt %XResourcePath%\XProject\Assets\Resources\Table\WayPoint gsconf\table\WayPoint
xcopy /e /exclude:exclude.txt %XResourcePath%\XProject\Assets\Resources\Table\SuperRisk gsconf\table\SuperRisk

xcopy /e /exclude:exclude.txt %XResourcePath%\XProject\Assets\Resources\CutScene gsconf\CutScene\
xcopy /e /exclude:exclude.txt %XResourcePath%\XProject\Assets\Resources\SkillPackage gsconf\SkillPackage\
xcopy /e /exclude:exclude.txt %XResourcePath%\XProject\Assets\Editor\EditorResources\Server\Curve gsconf\Curve\

)

endlocal

echo "开启服务器"
REM start /MIN fmserver.exe conf/fm_conf.xml
REM start /MIN centerserver.exe conf/cs_conf.xml
start /MIN dbserver.exe conf/db_conf.xml
start /MIN gameserver.exe conf/gs_conf.xml
start /MIN gateserver.exe conf/gate_conf.xml
start /MIN masterserver.exe conf/ms_conf.xml
start /MIN controlserver.exe conf/ctrl_conf.xml
REM start /MIN worldserver.exe conf/world_conf.xml
REM start /MIN routerserver.exe conf/router_conf.xml
start /MIN gameserver.exe conf/gs_cross_conf.xml
start /MIN teamserver.exe conf/team_conf.xml
REM start /MIN audioserver.exe conf/audio_conf.xml
 

goto exitpoint


echo "无法查询到你的服务器ID"
echo "服务器开启失败"


REM 暂停2秒钟
REM ping 127.0.0.1 -n 2 > nul 

:exitpoint
pause
