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
del/f /q LogFIle\hid_dev.txt
del/f /q LogFIle\i2c_dev.txt
del/f /q LogFIle\dev*.tmp
echo.

::ɾ����ʷ��¼�ļ�
if exist LogFIle\Return_InstallDriver.txt (
    del/f /q LogFIle\Return_InstallDriver.txt
)
if exist LogFIle\InstallDriverSucceeded.txt (
    del/f /q LogFIle\InstallDriverSucceeded.txt
)
echo.

::���Ŀ¼
if not exist LogFIle (
    md LogFIle
    echo.
)


pnputil /scan-devices
echo scan-devicesɨ���豸ok
echo.

echo ��ʼ�������е�HID�豸device��ע���/connected��ʾ�Ѿ�����
pnputil /enum-devices /connected /class {745a17a0-74d3-11d0-b6fe-00a0c90f57da}  /ids /relations >LogFIle\hid_dev.txt
echo enum-devicesö���豸ok
echo.

::����Ƿ��ҵ�touchpad���ذ��豸device
find/i "HID_DEVICE_UP:000D_U:0005" LogFIle\hid_dev.txt || (
     echo δ���ִ��ذ��豸���밲װԭ���������ٴγ���
     echo No TouchPad device found. Please install the original driver and try again
     echo.
     echo INSTDRV_FAILED >LogFIle\Return_InstallDriver.txt
     exit
)

echo �ҵ�touchpad���ذ��豸
echo TouchPad device found.
echo.

echo ��ʼ����touchpad���ذ��Ӧ�ĸ���I2C�豸ʵ��InstanceID
echo.
 
 ::�滻�س����з�Ϊ���ŷ���ָע�����NUL����Ϊ׷��>>д��
for /f "delims=" %%i in (LogFIle\hid_dev.txt) do (
   set /p="%%i,"<nul>>LogFIle\dev0.tmp
 )

::�滻HID_DEVICE_UP:000D_U:0005Ϊ#����ָע��set /p��Ҫ�Ӷ���
for /f "delims=, tokens=*" %%i in (LogFIle\dev0.tmp) do (
    set "str=%%i"
    set "str=!str:HID_DEVICE_UP:000D_U:0005=#!"
    set /p="!str!,"<nul>>LogFIle\dev1.tmp
)

  ::��ȡ#�ָ���������ı���ע��set /p��Ҫ�Ӷ���
 for /f "delims=# tokens=2,*" %%i in (LogFIle\dev1.tmp) do (
   set /p="%%i,"<nul>>LogFIle\dev2.tmp
 )

  ::��ȡ:�ָ���������ı���ע��set /p��Ҫ�Ӷ���
 for /f "delims=: tokens=2" %%i in (LogFIle\dev2.tmp) do (
   set /p="%%i,"<nul>>LogFIle\dev3.tmp
 )

   ::��ȡ,�ָ���ǰ����ı�
 for /f "delims=, tokens=1" %%i in (LogFIle\dev3.tmp) do (
  set /p="%%i"<nul>>LogFIle\dev4.tmp
 )

    ::ɾ���ո�
 for /f "delims= " %%i in (LogFIle\dev4.tmp) do (
   set "str=%%i"
   echo !str!>LogFIle\TouchPad_I2C_devInstanceID.txt
 )
echo.

del/f /q LogFIle\hid_dev.txt
del/f /q LogFIle\dev*.tmp
echo.
 

 ::��֤InstanceID
  for /f "delims=" %%i in (LogFIle\TouchPad_I2C_devInstanceID.txt) do (
   set "i2c_dev_InstanceID=%%i"
   echo i2c_dev_InstanceID="!i2c_dev_InstanceID!"
   echo.
 )

 ::ע���/connected��ʾ�Ѿ�����
 pnputil /enum-devices /connected /instanceid "%i2c_dev_InstanceID%" /ids /relations /drivers >LogFIle\i2c_dev.txt
 echo.
 
 ::����Ƿ�Ϊi2c�豸device
find/i "ACPI\PNP0C50" LogFIle\i2c_dev.txt || (
     echo δ����i2c���ذ��豸���޷���װ����
     echo No I2C TouchPad device found, unable to install driver.
     echo.
     del/f /q LogFIle\i2c_dev.txt
     del/f /q LogFIle\TouchPad_I2C_devInstanceID.txt
     echo INSTDRV_FAILED >LogFIle\Return_InstallDriver.txt
     exit
)

echo �ҵ�touchpad���ذ�I2C�豸
echo TouchPad I2C device found.
echo.

 
 ::��װ������ֻ��ӵ��������в���װ��ע�����һ����Ҫ��/install
  pnputil /add-driver Driver\MouseLikeTouchPad_I2C.inf
  echo add-driver��װ����ok
  echo.
  
 ::ɾ��i2c���ذ��豸
pnputil /remove-device "%i2c_dev_InstanceID%"
echo remove-deviceɾ���豸ok
echo.

 :ɨ��i2c���ذ��豸ʹ���Զ���װoem����������
pnputil /scan-devices
echo scan-devicesɨ���豸ok
echo.


::��֤�Ƿ�װ�ɹ���ע���/connected��ʾ�Ѿ�����
 pnputil /enum-devices /connected /instanceid "%i2c_dev_InstanceID%" /ids /relations /drivers >LogFIle\i2c_dev.txt
 echo.
 
find/i "MouseLikeTouchPad_I2C" LogFIle\i2c_dev.txt || (
     echo ��װ����ʧ�ܣ���ж��������������������������
     echo Failed to install the driver. Please uninstall the other third-party driver and try again.
     echo.
     del/f /q LogFIle\i2c_dev.txt
     echo INSTDRV_FAILED >LogFIle\Return_InstallDriver.txt
     exit
)

del/f /q LogFIle\i2c_dev.txt
echo.

echo ��װ�����ɹ�
echo Driver installed successfully.
echo.
echo INSTDRV_OK >LogFIle\Return_InstallDriver.txt
echo INSTDRV_OK >LogFIle\InstallDriverSucceeded.txt
echo.
