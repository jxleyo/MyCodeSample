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

echo ��ʼ��鰲װ�ļ�
echo Start checking installation files
echo.

if exist MouseLikeTouchPad_I2C.inf (
    echo MouseLikeTouchPad_I2C.inf�ļ�����
) else (
    echo MouseLikeTouchPad_I2C.inf�ļ���ʧ�����������������������װ��
    echo MouseLikeTouchPad_I2C.inf File Lost��Please check or download the driver installation package again.
    pause
    exit
)

if exist MouseLikeTouchPad_I2C.cat (
    echo MouseLikeTouchPad_I2C.cat�ļ�����
) else (
    echo MouseLikeTouchPad_I2C.cat�ļ���ʧ�����������������������װ��
    echo MouseLikeTouchPad_I2C.cat File Lost��Please check or download the driver installation package again.
    pause
    exit
)

if exist MouseLikeTouchPad_I2C.sys (
    echo MouseLikeTouchPad_I2C.sys�ļ�����
) else (
    echo MouseLikeTouchPad_I2C.sys�ļ���ʧ�����������������������װ��
    echo MouseLikeTouchPad_I2C.sys File Lost��Please check or download the driver installation package again.
    pause
    exit
)


::�����ӳٱ�����չ
setlocal enabledelayedexpansion
echo.

 ::ɾ����ʷ�����ļ�
del/f /q hid_dev.txt
del/f /q i2c_dev.txt
del/f /q dev*.tmp
del/f /q drv*.tmp
echo.

::ɾ����ʷ��¼�ļ�
if exist Return.txt (
    del/f /q Return.txt
)
echo.

::д�뿪ʼ����bat���ź�
echo StartBAT>Return.txt
echo.

echo Check Windows Version..
ver>winver.txt
echo.

find "10.0." winver.txt || (
	 echo ��ǰϵͳ����windows10/11���޷���װ��
 	echo Current OS is not Windows10/11��Can't Install the Driver��
	 echo.
 	del/f /q winver.txt
 	echo.
	set var=VER_OS_ERR
	echo !var!>>Return.txt 
 	exit
) 

 ::windows10v2004����汾��10.0.19041.264 //10.0.a.b��ʽ��ֱ���ж�a>=19041���ɣ�ע��delims��ѻس����з��Զ������ָ������Զ��������tokens����ֵ������Ҫ���㻻������µ��к�
 for /f "delims=[.] tokens=4" %%i in (winver.txt) do (
   set "winver=%%i"
 )
 
::��ֵ�ַ�����ֵ��С�Ƚϣ�ע����Ҫ���ţ�����ʵ���������ַ���λ����ͬʱ��С�Ƚϲ���׼ȷ�ģ�����2041��19041��Ƚϻ��Ǵ�����
if ("%winver%" LSS "19041") (
     echo ��ǰwindowsϵͳ�汾̫�ͣ�������������
	echo The current version of windows system is too low. Please upgrade and try again.
	echo.
	del/f /q winver.txt
	echo.
	set var=VER_LOW_ERR
	echo !var!>>Return.txt 
	exit
)

echo ��ǰwindowsϵͳ�汾ƥ��ok
echo Current Windows system version matches OK

del/f /q winver.txt
echo.
set var=VER_OK
echo !var!>>Return.txt 
echo.

echo ��ʼ�������е�HID�豸device
pnputil /scan-devices
echo scan-devicesɨ���豸ok
echo.
pnputil /enum-devices /connected /class {745a17a0-74d3-11d0-b6fe-00a0c90f57da}  /ids /relations >hid_dev.txt
echo enum-devicesö���豸ok
echo.

::����Ƿ��ҵ�touchpad���ذ��豸device
find/i "HID_DEVICE_UP:000D_U:0005" hid_dev.txt || (
     echo δ���ִ��ذ��豸������ж������
     echo No TouchPad device found, no need to unload the driver.
     echo.
     set var=TP_NODEV_ERR
     echo !var!>>Return.txt
     exit
)

echo �ҵ�touchpad���ذ��豸
echo TouchPad device found.
echo.

echo ��ʼ����touchpad���ذ��Ӧ�ĸ���I2C�豸ʵ��InstanceID
echo.
 
 ::�滻�س����з�Ϊ���ŷ���ָע�����NUL����Ϊ׷��>>д��
for /f "delims=" %%i in (hid_dev.txt) do (
   set /p="%%i,"<nul>>dev0.tmp
 )

::�滻HID_DEVICE_UP:000D_U:0005Ϊ#����ָע��set /p��Ҫ�Ӷ���
for /f "delims=, tokens=*" %%i in (dev0.tmp) do (
    set "str=%%i"
    set "str=!str:HID_DEVICE_UP:000D_U:0005=#!"
    set /p="!str!,"<nul>>dev1.tmp
)

  ::��ȡ#�ָ���������ı���ע��set /p��Ҫ�Ӷ���
 for /f "delims=# tokens=2,*" %%i in (dev1.tmp) do (
   set /p="%%i,"<nul>>dev2.tmp
 )

  ::��ȡ:�ָ���������ı���ע��set /p��Ҫ�Ӷ���
 for /f "delims=: tokens=2" %%i in (dev2.tmp) do (
   set /p="%%i,"<nul>>dev3.tmp
 )

   ::��ȡ,�ָ���ǰ����ı�
 for /f "delims=, tokens=1" %%i in (dev3.tmp) do (
  set /p="%%i"<nul>>dev4.tmp
 )

    ::ɾ���ո�
 for /f "delims= " %%i in (dev4.tmp) do (
   set "str=%%i"
   echo !str!>i2c_dev_InstanceID.txt
 )
echo.

del/f /q hid_dev.txt
del/f /q dev*.tmp
 echo.
 

 ::��֤InstanceID
  for /f "delims= " %%i in (i2c_dev_InstanceID.txt) do (
   set "i2c_dev_InstanceID=%%i"
   echo i2c_dev_InstanceID="!i2c_dev_InstanceID!"
   echo.
 )
 
 ::ע���/connected��ʾ�Ѿ�����
 pnputil /enum-devices /connected /instanceid "%i2c_dev_InstanceID%" /ids /relations /drivers >i2c_dev.txt
echo.

 ::����Ƿ�Ϊi2c�豸device
find/i "ACPI\PNP0C50" i2c_dev.txt || (
     echo δ����i2c���ذ��豸������ж������
     echo No i2c TouchPad device found, no need to unload the driver.
     echo.
     del/f /q i2c_dev.txt
      set var=TP_NOI2C_ERR
     echo !var!>>Return.txt
     exit
)

echo �ҵ�i2c���ذ��豸
echo I2C TouchPad device found.
echo.
set var=TP_OK
echo !var!>>Return.txt
echo.
 
 
::����Ƿ�װMouseLikeTouchPad_I2C����
find/i "MouseLikeTouchPad_I2C" i2c_dev.txt || (
     echo δ����MouseLikeTouchPad_I2C����������ж������
     echo No MouseLikeTouchPad_I2C driver found, no need to unload the driver.
     echo.
     del/f /q i2c_dev.txt
     set var=DRV_OEM_ERR
     echo !var!>>Return.txt
     exit
)

echo �ҵ�MouseLikeTouchPad_I2C����
echo.

echo ��ʼ����MouseLikeTouchPad_I2C����oem�ļ���
echo.

 ::ɾ����ʷ�����ļ�
del/f /q drv*.tmp
 echo.
 
 ::�滻�س����з�Ϊ���ŷ���ָע�����NUL����Ϊ׷��>>д��
for /f "delims=" %%i in (i2c_dev.txt) do (
   set /p="%%i,"<nul>>drv0.tmp
 )

::�滻mouseliketouchpad_i2c.infΪ#����ָע��set /p��Ҫ�Ӷ���
for /f "delims=, tokens=*" %%i in (drv0.tmp) do (
    set "str=%%i"
    set "str=!str:mouseliketouchpad_i2c.inf=#!"
    set /p="!str!,"<nul>>drv1.tmp
)

  ::��ȡ#�ָ���ǰ����ı�
 for /f "delims=# tokens=1" %%i in (drv1.tmp) do (
   set /p="%%i"<nul>>drv2.tmp
 )

::�滻oemΪ[����ָע��set /p��Ҫ�Ӷ���
for /f "delims=, tokens=*" %%i in (drv2.tmp) do (
    set "str=%%i"
    set "str=!str:oem=[!"
    set /p="!str!,"<nul>>drv3.tmp
)

  ::��ȡ���һ��[�ָ���������ı���ע��tokensҪѡ2������������в���nul���治��׷�Ӷ���>
 for /f "delims=[ tokens=2,*" %%i in (drv3.tmp) do (
   set /p="%%i,"<nul>drv4.tmp
 )

   ::��ȡ,�ָ���ǰ����ı�
 for /f "delims=, tokens=1" %%i in (drv4.tmp) do (
  set /p="oem%%i"<nul>oemfilename.txt
 )
echo.

 ::ɾ����ʷ�����ļ�
del/f /q drv*.tmp
echo.


  ::��ȡoemfilename
  for /f "delims=" %%i in (oemfilename.txt) do (
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


::��֤�Ƿ�ж�سɹ���ע���/connected��ʾ�Ѿ�����
 pnputil /enum-devices /connected /instanceid "%i2c_dev_InstanceID%" /ids /relations /drivers >i2c_dev.txt
 echo.
 
find/i "MouseLikeTouchPad_I2C" i2c_dev.txt && (
     echo ж������ʧ�ܣ�����������
     echo Failed to unload the driver. Please try again.
     echo.
     del/f /q i2c_dev.txt
     set var=UNDRV_ERR
     echo !var!>>Return.txt
     exit
)

del/f /q i2c_dev.txt
echo ж�������ɹ�
echo Unload driver succeeded.
echo.
set var=UNDRV_OK
echo !var!>>Return.txt
echo.


