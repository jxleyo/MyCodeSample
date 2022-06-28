echo off
echo 弹出窗口“允许此应用对你的设备进行更改“ 请选择“是”以获取管理员权限来运行本安装脚本
echo Pop up window "allow this app to make changes to your device" please select "yes" to obtain administrator rights to run this installation script
echo.

::获取管理员权限
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
::保持当前目录下运行
cd /d "%~dp0"

echo off
echo 开始运行安装脚本，安装驱动前请确保其他程序已关闭或文档已保存
echo Start running the installation script. Before installing the driver, make sure that other programs have been closed or the document has been saved.
echo.


::开启延迟变量扩展
setlocal enabledelayedexpansion
echo.

 ::删除历史残留文件
 if exist LogFIle\hid_dev.txt (
    del/f /q LogFIle\hid_dev.txt
)
echo.


::删除历史记录文件
if exist LogFIle\Return_UninstDrv.txt (
    del/f /q LogFIle\Return_UninstDrv.txt
)
if exist LogFIle\UninstDrvSucceeded.txt (
    del/f /q LogFIle\UninstDrvSucceeded.txt
)
echo.

::检测目录
if not exist LogFIle (
    md LogFIle
    echo.
)

if not exist LogFIle\OEMDriverName.txt (
     echo 获取OEM驱动文件名失败。
     echo Failed to get OEMDriverName. 
     echo OEM_FAILED >LogFIle\Return_UninstDrv.txt
     echo.
     exit
)

  ::从OEMDriverName.txt文件读取oemfilename
  for /f "delims=" %%i in (LogFIle\OEMDriverName.txt) do (
   set "oemfilename=%%i"
   echo oemfilename="!oemfilename!"
   echo.
 )

 :卸载oem第三方驱动
 pnputil /delete-driver "%oemfilename%" /uninstall /force
 echo delete-driver卸载驱动ok
echo.

pnputil /scan-devices
echo scan-devices扫描设备ok
echo.


::验证是否卸载成功，
pnputil /enum-devices /class {745a17a0-74d3-11d0-b6fe-00a0c90f57da}  /ids /relations >LogFIle\hid_dev.txt
 echo.
 
find/i "MouseLikeTouchPad_I2C" LogFIle\hid_dev.txt || (
     goto unInstSucceded
)


if not exist LogFIle\TouchPad_I2C_devInstanceID.txt (
     echo 获取设备实例失败。
     echo Failed to get devInstanceID. 
     echo devInstanceID_FAILED >LogFIle\Return_UninstDrv.txt
     echo.
     exit
)

  ::从TouchPad_I2C_devInstanceID.txt文件读取设备实例
  for /f "delims=" %%i in (LogFIle\TouchPad_I2C_devInstanceID.txt) do (
   set "i2c_devInstanceID=%%i"
   echo i2c_devInstanceID="!i2c_devInstanceID!"
   echo.
 )
 
 ::先强制删除inf文件再移除设备实例
 pnputil /delete-driver "%oemfilename%" /force
 pnputil /remove-device "%i2c_devInstanceID%"
 
pnputil /scan-devices
echo scan-devices扫描设备ok
echo.


::验证是否卸载成功，
pnputil /enum-devices /class {745a17a0-74d3-11d0-b6fe-00a0c90f57da}  /ids /relations >LogFIle\hid_dev.txt
 echo.
 
find/i "MouseLikeTouchPad_I2C" LogFIle\hid_dev.txt && (
     echo 卸载驱动失败。
     echo Failed to unload the driver. 
     echo UNDRV_FAILED >LogFIle\Return_UninstDrv.txt
     echo.
     del/f /q LogFIle\hid_dev.txt
     exit
)


:unInstSucceded
del/f /q LogFIle\hid_dev.txt
echo 卸载驱动成功
echo Unload driver succeeded.
echo.
echo UNDRV_OK >LogFIle\Return_UninstDrv.txt
echo UNDRV_OK >LogFIle\UninstDrvSucceeded.txt
echo.
