#pragma once

#include "resource.h"



//
#include <strsafe.h>

#include <setupapi.h>
#include <cfgmgr32.h>
#include <newdev.h>

#include <hidsdi.h>

#include <atlstr.h>
#include <shellapi.h>




#define PtpDrvMgr_FLAG_FORCE       0x00000001

// exit codes
#define EXIT_OK      (0)
#define EXIT_REBOOT  (1)
#define EXIT_FAIL    (2)
#define EXIT_USAGE   (3)


BOOL EnbalePrivileges();
BOOL EnumerateDevices();
BOOL Rescan();
BOOL FindDevice();
BOOL RemoveDriver();
BOOL UpdateDriver();
BOOL UninstallDriver();
BOOL InstallDriver();
void Install();
void Uninstall();

BOOL DirExist(LPCWSTR szFilePathName);
BOOL FileExist(LPCWSTR szFilePathName);
BOOL MainFileExist(LPCWSTR szFileName);
BOOL DrvFileExist(LPCWSTR szFileName);
BOOL LogFileExist(LPCWSTR szFileName);
BOOL NewLogFile(LPCWSTR szFileName);
void DelLogFile(LPCWSTR szFileName);
BOOL GetTouchPad_I2C_hwID();
BOOL GetOEMDriverName();
BOOL SaveOEMDriverName();
BOOL SaveTouchPad_I2C_hwID();


wchar_t* mystrcat(const wchar_t* str1, const wchar_t* str2, const wchar_t* str3);//str1��ԭ�����ַ�����str2��str1�е�ĳ����Ҫ�滻�ַ�����str3���滻str2���ַ���
BOOL FuzzyCompareHwIds(PZPWSTR Array, const wchar_t* MatchHwId);
BOOL FindCurrentDriver(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo, _In_ PSP_DRVINFO_DATA DriverInfoData);
BOOL GetDeviceOEMdriverInfo(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo);
void Set_InstallationSources_Directory(wchar_t* szPath);

void DelMultiSz(_In_opt_ __drv_freesMem(object) PZPWSTR Array);
__drv_allocatesMem(object) LPTSTR* GetDevMultiSz(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo, _In_ DWORD Prop);
__drv_allocatesMem(object) LPTSTR* GetMultiSzIndexArray(_In_ __drv_aliasesMem LPTSTR MultiSz);



