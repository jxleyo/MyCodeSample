echo off
echo �������ڡ������Ӧ�ö�����豸���и��ġ� ��ѡ���ǡ��Ի�ȡ����ԱȨ�������б���װ�ű�
echo Pop up window "allow this app to make changes to your device" please select "yes" to obtain administrator rights to run this installation script
echo.

::��ȡ����ԱȨ��
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
::���ֵ�ǰĿ¼������
cd /d "%~dp0"

echo off
echo ��ʼ���а�װ�ű�����װ����ǰ��ȷ�����������ѹرջ��ĵ��ѱ���
echo Start running the installation script. Before installing the driver, make sure that other programs have been closed or the document has been saved.
echo.


::�����ӳٱ�����չ
setlocal enabledelayedexpansion
echo.

 ::ɾ����ʷ�����ļ�
 if exist LogFIle\hid_dev.txt (
    del/f /q LogFIle\hid_dev.txt
)
echo.


::ɾ����ʷ��¼�ļ�
if exist LogFIle\Return_UninstDrv.txt (
    del/f /q LogFIle\Return_UninstDrv.txt
)
if exist LogFIle\UninstDrvSucceeded.txt (
    del/f /q LogFIle\UninstDrvSucceeded.txt
)
echo.

::���Ŀ¼
if not exist LogFIle (
    md LogFIle
    echo.
)

if not exist LogFIle\OEMDriverName.txt (
     echo ��ȡOEM�����ļ���ʧ�ܡ�
     echo Failed to get OEMDriverName. 
     echo OEM_FAILED >LogFIle\Return_UninstDrv.txt
     echo.
     exit
)

  ::��OEMDriverName.txt�ļ���ȡoemfilename
  for /f "delims=" %%i in (LogFIle\OEMDriverName.txt) do (
   set "oemfilename=%%i"
   echo oemfilename="!oemfilename!"
   echo.
 )

 :ж��oem����������
 pnputil /delete-driver "%oemfilename%" /uninstall /force
 echo delete-driverж������ok
echo.

pnputil /scan-devices
echo scan-devicesɨ���豸ok
echo.


::��֤�Ƿ�ж�سɹ���
pnputil /enum-devices /class {745a17a0-74d3-11d0-b6fe-00a0c90f57da}  /ids /relations >LogFIle\hid_dev.txt
 echo.
 
find/i "MouseLikeTouchPad_I2C" LogFIle\hid_dev.txt || (
     goto unInstSucceded
)


if not exist LogFIle\TouchPad_I2C_devInstanceID.txt (
     echo ��ȡ�豸ʵ��ʧ�ܡ�
     echo Failed to get devInstanceID. 
     echo devInstanceID_FAILED >LogFIle\Return_UninstDrv.txt
     echo.
     exit
)

  ::��TouchPad_I2C_devInstanceID.txt�ļ���ȡ�豸ʵ��
  for /f "delims=" %%i in (LogFIle\TouchPad_I2C_devInstanceID.txt) do (
   set "i2c_devInstanceID=%%i"
   echo i2c_devInstanceID="!i2c_devInstanceID!"
   echo.
 )
 
 ::��ǿ��ɾ��inf�ļ����Ƴ��豸ʵ��
 pnputil /delete-driver "%oemfilename%" /force
 pnputil /remove-device "%i2c_devInstanceID%"
 
pnputil /scan-devices
echo scan-devicesɨ���豸ok
echo.


::��֤�Ƿ�ж�سɹ���
pnputil /enum-devices /class {745a17a0-74d3-11d0-b6fe-00a0c90f57da}  /ids /relations >LogFIle\hid_dev.txt
 echo.
 
find/i "MouseLikeTouchPad_I2C" LogFIle\hid_dev.txt && (
     echo ж������ʧ�ܡ�
     echo Failed to unload the driver. 
     echo UNDRV_FAILED >LogFIle\Return_UninstDrv.txt
     echo.
     del/f /q LogFIle\hid_dev.txt
     exit
)


:unInstSucceded
del/f /q LogFIle\hid_dev.txt
echo ж�������ɹ�
echo Unload driver succeeded.
echo.
echo UNDRV_OK >LogFIle\Return_UninstDrv.txt
echo UNDRV_OK >LogFIle\UninstDrvSucceeded.txt
echo.
