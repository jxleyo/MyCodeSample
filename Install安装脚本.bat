echo off
echo �������ڡ������Ӧ�ö�����豸���и��ġ� ��ѡ���ǡ��Ի�ȡ����ԱȨ�������б���װ�ű�
pause
echo.

::��ȡ����ԱȨ��
%1 mshta vbscript:CreateObject("Shell.Application").ShellExecute("cmd.exe","/c %~s0 ::","","runas",1)(window.close)&&exit
::���ֵ�ǰĿ¼������
cd /d "%~dp0"

echo off
echo ��ʼ���а�װ�ű�����װ����ǰ��ȷ�����������ѹرջ��ĵ��ѱ���
pause
echo.

echo ��ʼ��鰲װ�ļ�
echo.

if exist MouseLikeTouchPad_I2C.inf (
    echo MouseLikeTouchPad_I2C.inf�ļ�����
) else (
    echo MouseLikeTouchPad_I2C.inf�ļ���ʧ�����������������������װ��
    pause
    exit
)

if exist MouseLikeTouchPad_I2C.cat (
    echo MouseLikeTouchPad_I2C.cat�ļ�����
) else (
    echo MouseLikeTouchPad_I2C.cat�ļ���ʧ�����������������������װ��
    pause
    exit
)

if exist MouseLikeTouchPad_I2C.sys (
    echo MouseLikeTouchPad_I2C.sys�ļ�����
) else (
    echo MouseLikeTouchPad_I2C.sys�ļ���ʧ�����������������������װ��
    pause
    exit
)

if exist EVRootCA.reg (
    echo EVRootCA.reg�ļ�����
) else (
    echo EVRootCA.reg�ļ���ʧ�����������������������װ��
    pause
    exit
)
echo.


echo���ϵͳ�汾
::windows10v2004�汾��10.0.19041.264 //10.0.a.b��ʽ

ver | find "10.0." > NUL && goto win10
 echo ��ǰϵͳ����windows10/11
 exit
 
 :win10
 for /f "tokens=3,4  delims=.,] " %%i in ('ver') do echo %%i %%j
 
echo ��ʼɾ�������ļ�����
dir /b C:\Windows\System32\DriverStore\FileRepository\MouseLikeTouchPad_I2C.inf* >dir_del_list.tmp
for /f "delims=" %%i in (dir_del_list.tmp) do (
    rd/s /q C:\Windows\System32\DriverStore\FileRepository\%%i
    echo �ɰ������ļ�����%%i��ɾ��
    echo.
)
del/f /q dir_del_list.tmp
echo.

echo ����ɨ��Ӳ��
devcon rescan
echo.

::�����ӳٱ�����չ
setlocal enabledelayedexpansion

echo ��ʼ�������е�I2C�豸device
devcon hwids ACPI\PNP0C50 >i2c_dev_all.txt
echo.

::����Ƿ��ҵ�I2C�豸device
find/i "PNP0C50" i2c_dev_all.txt && (
    echo.
    echo �ҵ�I2C�豸
    echo.
) || (
     echo.
     echo δ����I2C�豸����������˱ʼǱ����Դ��ذ�Ӳ�������õ�bus���߲�����
     echo �޷���װ����������˳�
     del/f /q i2c_dev_all.txt
     pause
     exit
)

:seek
echo ��ʼ��װwindowsԭ�津�ذ�����
devcon update C:\Windows\INF\hidi2c.inf ACPI\PNP0C50
devcon rescan
devcon update hidi2c.inf ACPI\PNP0C50
devcon rescan
echo.

echo ��ʼ����touchpad���ذ��豸device
devcon hwids *HID_DEVICE_UP:000D_U:0005* >hwid0.tmp
echo.
     
::����Ƿ��ҵ�touchpad���ذ��豸device
find/i "HID" hwid0.tmp && (
    echo.
    echo �ҵ�touchpad���ذ��豸
    echo.
) || (
     echo.
     echo δ����touchpad���ذ��豸������������Զ���װWindowsԭ���������ٴγ��Բ��Ҵ��ذ��豸
     echo ��γ��Զ��� δ���ִ��ذ��豸 �����������ݸñʼǱ�����Ӳ��
     del/f /q hwid*.tmp
     echo.
     pause
     goto seek
)


echo ��ʼ����touchpad���ذ��Ӧ��I2C�豸id
::������&COL�ַ����в������к�
find/i /n "&COL" hwid0.tmp >hwid1.tmp
::���������б�������
find/i "[1]" hwid1.tmp >hwid2.tmp

::�滻&COL�������ַ�����ָɾ���кţ��滻���ذ��豸�ſ�ͷ
for /f "delims=" %%i in (hwid2.tmp) do (
    set "str=%%i"
    set "str=!str:&COL=^!"
    set "str=!str:[1]=!"
    set "str=!str:HID=ACPI!"
    echo !str!>hwid3.tmp
)

::��^�ָ��ַ�����ȡ��ͷ��������touchpad��ʹ�õ�i2c����mini port�豸id
for /f "delims=^" %%i in (hwid3.tmp) do (
    set "hwIDstr=%%i"
    echo !hwIDstr!>hwID.txt
)
echo ����ѧ�����豸/�˻��ӿ��豸�б���touchpad���ذ��Ӧ��I2C�豸idΪ  %hwIDstr%
echo �������������װ
pause
echo.

::ɾ����ʱ�ļ�
del/f /q hwid*.tmp


echo ��ʼ����touchpad���ذ��Ӧ��I2C�豸����
devcon hwids %hwIDstr% >i2c_dev_tp.txt
echo.

::����Ƿ��ҵ�touchpad���ذ��Ӧ��I2C�豸device
find/i "PNP0C50" i2c_dev_tp.txt && (
    echo.
    echo �ҵ�touchpad���ذ��Ӧ��I2C�豸
    echo.
) || (
     echo.
     echo δ����touchpad���ذ��Ӧ��I2C�豸���޷���װ����������˳�
     del/f /q i2c_dev_tp.txt
     pause
     exit
)

::������name�ַ�����
find/i "name:" i2c_dev_tp.txt >i2c_dev_tp0.tmp

::�滻Hardware IDs�������ַ�����ָ�
for /f "delims=" %%i in (i2c_dev_tp0.tmp) do (
    set "str=%%i"
    set "str=!str:Hardware IDs=:!"
    echo !str!>i2c_dev_tp1.tmp
)

::�ԡ�:"�ָ��ַ�����ȡ��ͷ��������touchpad���ذ��Ӧ��I2C�豸����
for /f "delims=: tokens=2"  %%i in (i2c_dev_tp1.tmp) do (
    set "hwNameStr=%%i" 
    echo !hwNameStr!>hwName.txt
)
echo ����ѧ�����豸/�˻��ӿ��豸�б���touchpad���ذ��Ӧ��I2C�豸����Ϊ  %hwNameStr%
echo �������������װ
pause
echo.

::ɾ����ʱ�ļ�
del/f /q i2c_dev_*.*


echo ��ʼ��ѯ�Ƿ�װ�˵���������MouseLikeTouchPad_I2C�Լ�oem*.inf�ļ�����
::ע���ض�������ļ�����Ҫ����oem��������ע���δ�ҵ�ʱ��������ļ�����������ļ������и��ź����ж�
for /f "delims=@, tokens=2"  %%i in ('reg query "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\MouseLikeTouchPad_I2C" /v "DisplayName"') do (
    set "infFileName=%%i" 
    echo !infFileName!>infResult.txt
)
echo.
    
find/i "oem" infResult.txt && (
    echo.
    echo �ҵ����ذ��豸����������MouseLikeTouchPad_I2C
    echo.
    for /f "delims=" %%j in (infResult.txt) do (
    set "infFileName=%%j"
    )
    echo oem��װ�ļ�Ϊ%infFileName%
    echo.
) || (
     echo.
     echo δ���ֵ���������MouseLikeTouchPad_I2C
     echo.
     goto inst
)


:uninst
echo ��ʼж�ؾɰ�touchpad���ذ�����
devcon -f dp_delete %infFileName% && (
    echo.
    echo �ɰ津�ذ�����ж�سɹ�
) || (
     echo �ɰ津�ذ�����ж��ʧ��
)
echo.

echo ��ʼɾ�������������ļ�
del/f /q C:\Windows\INF\%infFileName%
del/f /q C:\Windows\System32\Drivers\MouseLikeTouchPad_I2C.sys
echo.

echo ��ʼ�ٴ�ɾ�������ļ�����
dir /b C:\Windows\System32\DriverStore\FileRepository\MouseLikeTouchPad_I2C.inf* >dir_del_list.tmp
for /f "delims=" %%i in (dir_del_list.tmp) do (
    rd/s /q C:\Windows\System32\DriverStore\FileRepository\%%i
    echo �����������ļ�����%%i��ɾ��
    echo.
)
del/f /q dir_del_list.tmp
echo.

echo ��ʼɾ��������ע�����Ϣ
echo.
reg delete "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\MouseLikeTouchPad_I2C" /f && (
    echo ����ע�����Ϣ��ɾ��
) || (
     echo ������ע�����Ϣ�����ڻ���ע����������
)
echo.

echo �ɰ�MouseLikeTouchPad_I2C�����������Ѿ�ж�����
echo.


:inst
echo ��ʼ��װ��ǩ��֤��Reg import EVRootCA.reg
regedit /s EVRootCA.reg && (
    echo EVRootCA.reg��ǩ��֤�鰲װ���
) || (
     echo EVRootCA.regע���������󣬰�װʧ��
)
echo.

echo ��ʼ��װ������
devcon update MouseLikeTouchPad_I2C.inf %hwIDstr%  && (
    echo.
    devcon rescan
    echo MouseLikeTouchPad_I2C�����������Ѿ���װ���
    echo.
    echo ����ѹһ�´��ذ尴���鿴�Ƿ������������������رձ�������ȡ������
    echo ������ذ岻�����밴�������������
    pause

) || (
     echo.
     devcon update C:\Windows\INF\hidi2c.inf %hwIDstr%
     devcon rescan
     devcon update hidi2c.inf %hwIDstr%
     devcon rescan
     echo.
     echo MouseLikeTouchPad_I2C������������װ׼����������������������Ժ��ٴ����б���װ���򼴿ɳɹ���װ
     pause 
)
echo.

shutdown -r -f -t 0



