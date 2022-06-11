// PtpDrvMgr.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "PtpDrvMgr.h"

#define MAX_LOADSTRING 100

static wchar_t OEM_DevDescription[] = L"MouseLikeTouchPad_I2C";
static wchar_t OEM_ProviderName[] = L"jxleyo.HRP";
static wchar_t OEM_MfgName[] = L"jxleyo.HRP";
static wchar_t I2C_COMPATIBLE_hwID[] = L"ACPI\\PNP0C50";
static wchar_t TouchPad_COMPATIBLE_hwID[] = L"HID_DEVICE_UP:000D_U:0005";
static wchar_t TouchPad_hwID[MAX_DEVICE_ID_LEN];
static wchar_t TouchPad_I2C_hwID[MAX_DEVICE_ID_LEN];
static wchar_t inf_FullPathName[MAX_PATH];
static wchar_t inf_name[] = L"Driver\\MouseLikeTouchPad_I2C.inf";
static wchar_t OEMinf_FullName[MAX_PATH];
static wchar_t OEMinf_name[MAX_PATH];
static BOOLEAN bOEMDriverExist;//存在驱动标志

static BOOLEAN bTouchPad_FOUND = FALSE;
static BOOLEAN bTouchPad_I2C_FOUND = FALSE;
static INT ExitCode = EXIT_OK;
static HWND hMainWnd;

static wchar_t exeFilePathName[MAX_PATH + 1] = { 0 };
static wchar_t exeFilePath[MAX_PATH + 1] = { 0 };
static wchar_t exeFileName[MAX_PATH + 1] = { 0 };

static char cmdFilePath[MAX_PATH + 1] = { 0 };

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名



// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
        //EnbalePrivileges();

    //GetModuleFileName(NULL, exeFilePath, MAX_PATH);
    //(wcsrchr(exeFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
    CString m_FilePathName, m_FilePath;//当前程序全路径名、目录路径
    GetModuleFileName(NULL, m_FilePathName.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
    wcscpy_s(exeFilePathName, m_FilePathName.AllocSysString());//    PathRemoveFileSpec
    int pos = m_FilePathName.ReverseFind('\\');
    m_FilePath = m_FilePathName.Left(pos);
    wcscpy_s(exeFilePath, m_FilePath.AllocSysString());
    wcscpy_s(exeFilePath, m_FilePath.AllocSysString());
    wcscpy_s(exeFileName, exeFilePathName + pos+1);

    //wchar_t str[MAX_PATH];
    //wcscpy_s(str, L"exeFilePathName=");
    //wcscat_s(str, exeFilePathName);
    //MessageBox(NULL, str, L"PtpDrvMgr", MB_OK);
    //memset(str, 0, MAX_PATH);
    //wcscpy_s(str, L"exeFilePath=");
    //wcscat_s(str, exeFilePath);
    //MessageBox(NULL, str, L"PtpDrvMgr", MB_OK);
    //memset(str, 0, MAX_PATH);
    //wcscpy_s(str, L"exeFileName=");
    //wcscat_s(str, exeFileName);
    //MessageBox(NULL, str, L"PtpDrvMgr", MB_OK);

    //
     // 获得INF目录
    char szPath[MAX_PATH];  // 获得系统目录
    GetWindowsDirectoryA(szPath, sizeof(szPath));
    // 格式化文件路径
    strcat_s(szPath, "\\system32");
    strcpy_s(cmdFilePath, szPath);

    //赋值源驱动inf路径
    wcscpy_s(inf_FullPathName, exeFilePath);
    wcscpy_s(inf_FullPathName, L"\\");
    wcscpy_s(inf_FullPathName, inf_name);
    
    //检测文件
    if (!MainFileExist(L"InstDrv.bat")) {
        MessageBox(NULL, L"InstDrv.bat文件丢失，请重新下载驱动包", L"PtpDrvMgr", MB_OK);
        return EXIT_FAIL;
    }
    if (!MainFileExist(L"Uninst.bat")) {
        MessageBox(NULL, L"Uninst.bat文件丢失，请重新下载驱动包", L"PtpDrvMgr", MB_OK);
        return EXIT_FAIL;
    }
    if (!MainFileExist(L"icon.ico")) {
        MessageBox(NULL, L"icon.ico文件丢失，请重新下载驱动包", L"PtpDrvMgr", MB_OK);
        return EXIT_FAIL;
    }

    if (!DrvFileExist(L"MouseLikeTouchPad_I2C.inf")) {
        MessageBox(NULL, L"MouseLikeTouchPad_I2C.inf文件丢失，请重新下载驱动包", L"PtpDrvMgr", MB_OK);
        return EXIT_FAIL;
    }
    if (!DrvFileExist(L"MouseLikeTouchPad_I2C.cat")) {
        MessageBox(NULL, L"MouseLikeTouchPad_I2C.inf文件丢失，请重新下载驱动包", L"PtpDrvMgr", MB_OK);
        return EXIT_FAIL;
    }
    if (!DrvFileExist(L"MouseLikeTouchPad_I2C.sys")) {
        MessageBox(NULL, L"MouseLikeTouchPad_I2C.inf文件丢失，请重新下载驱动包", L"PtpDrvMgr", MB_OK);
        return EXIT_FAIL;
    }

    if (!DirExist(L"LogFile")) {
        TCHAR szPath[MAX_PATH];
        wcscpy_s(szPath, exeFilePath);
        wcscpy_s(szPath, L"\\LogFile");
        CreateDirectory(szPath, NULL);
    }
    

    //无界面模式的命令
    if (wcscmp(lpCmdLine, L"Install") == 0) {
        Install();//二次调用本程序
    }
    else if (wcscmp(lpCmdLine, L"Uninstall") == 0) {
        Uninstall(); //调用本程序
    }
    else if (wcscmp(lpCmdLine, L"Rescan") == 0) {//实际执行Rescan
        Rescan();
    }
    else if (wcscmp(lpCmdLine, L"FindDevice") == 0) {//实际执行FindDevice
        FindDevice();
    }
    else if (wcscmp(lpCmdLine, L"InstallDriver") == 0) {//实际执行安装
        InstallDriver();
    }
    else if (wcscmp(lpCmdLine, L"UninstallDriver") == 0) {//实际执行卸载
        UninstallDriver();
    }
    else if (wcscmp(lpCmdLine, L"") == 0) {//无参数托盘对话框模式
        goto TrayDlg;
    }
    else {
        _tprintf(TEXT("%s [Command]\n"), exeFileName);
        printf("[Install]: Auto Find Device and InstallDriver\n");
        printf("[Uninstall]: Auto Find Device and UninstallDriver\n");
        printf("[Rescan]: Rescan Device\n");
        printf("[FindDevice]: Find Device and Save Result to File\n");
        printf("[InstallDriver]: InstallDriver\n");
        printf("[UninstallDriver]: UninstallDriver\n");
        ExitCode = EXIT_USAGE;
    }

    return ExitCode;



TrayDlg:
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PTPDRVMGR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return EXIT_FAIL;
    }

    //if (nCmdShow) {
    //    ShowWindow(hMainWnd, SW_HIDE);
    //}



    ShowWindow(hMainWnd, SW_SHOWNORMAL);//SW_SHOWNORMAL
    //
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PTPDRVMGR));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;

}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PTPDRVMGR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PTPDRVMGR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   hMainWnd = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}






BOOL EnbalePrivileges()
{
    HANDLE Token;
    TOKEN_PRIVILEGES NewPrivileges;
    LUID Luid;

    //
    // we need to "turn on" reboot privilege
    // if any of this fails, try reboot anyway
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &Token)) {//TOKEN_ADJUST_PRIVILEGES//TOKEN_ALL_ACCESS 
        //printf("OpenProcessToken failed!\n");
        return FALSE;
    }

    if (!LookupPrivilegeValue(NULL, SE_LOAD_DRIVER_NAME, &Luid)) {//SE_LOAD_DRIVER_NAME//SE_SHUTDOWN_NAME 
        CloseHandle(Token);
        //printf("LookupPrivilegeValue failed!\n");
        return FALSE;
    }

    NewPrivileges.PrivilegeCount = 1;
    NewPrivileges.Privileges[0].Luid = Luid;
    NewPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    BOOL bRet = AdjustTokenPrivileges(Token, FALSE, &NewPrivileges, 0, NULL, NULL);
    if (bRet == FALSE)
    {
        CloseHandle(Token);
        //printf("AdjustTokenPrivileges failed!\n");
        return FALSE;
    }

    CloseHandle(Token);
    // 根据错误码判断是否特权都设置成功
    DWORD dwRet = 0;
    dwRet = GetLastError();
    if (ERROR_SUCCESS == dwRet)
    {
        //printf("EnbalePrivileges ok!\n");
        return TRUE;
    }
    else if (ERROR_NOT_ALL_ASSIGNED == dwRet)
    {
        //printf("ERROR_NOT_ALL_ASSIGNED!\n");
        return FALSE;
    }


    //printf("EnbalePrivileges end!\n");
    return TRUE;

}

BOOL Rescan()
{

    // reenumerate from the root of the devnode tree
    // totally CM based

    DEVINST devRoot;
    int failcode;

    failcode = CM_Locate_DevNode(&devRoot, NULL, CM_LOCATE_DEVNODE_NORMAL);
    if (failcode != CR_SUCCESS) {
        //printf("CM_Locate_DevNode failed!\n");
        return FALSE;
    }

    failcode = CM_Reenumerate_DevNode(devRoot, 0);
    if (failcode != CR_SUCCESS) {
        //printf("CM_Reenumerate_DevNode failed!\n");
        return FALSE;
    }

    //printf("Rescan ok!\n");
    return TRUE;

}


BOOL EnumerateDevices()
{
    BOOLEAN FOUND = FALSE;
    GUID     hidGuid;
    HidD_GetHidGuid(&hidGuid);

    HDEVINFO hDevInfo = SetupDiGetClassDevs(
        &hidGuid,
        NULL,
        NULL,
        (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        //printf("SetupDiGetClassDevs failed!\n");
        return 0;
    }

    SP_DEVICE_INTERFACE_DATA devInfoData;
    devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    int deviceNo = 0;

    SetLastError(NO_ERROR);
    //printf("while start\n");

    while (GetLastError() != ERROR_NO_MORE_ITEMS)
    {
        //printf("deviceNo=%d\n", deviceNo);
        if (SetupDiEnumInterfaceDevice(hDevInfo,
            0,
            &hidGuid,
            deviceNo,
            &devInfoData))
        {
            ULONG requiredLength = 0;
            SetupDiGetInterfaceDeviceDetail(hDevInfo,
                &devInfoData,
                NULL,
                0,
                &requiredLength,
                NULL);


            PSP_INTERFACE_DEVICE_DETAIL_DATA devDetail = (SP_INTERFACE_DEVICE_DETAIL_DATA*)malloc(requiredLength);
            devDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

            if (!SetupDiGetInterfaceDeviceDetail(hDevInfo,
                &devInfoData,
                devDetail,
                requiredLength,
                NULL,
                NULL))
            {
                //printf("SetupDiGetInterfaceDeviceDetail failed!\n");
                free(devDetail);
                SetupDiDestroyDeviceInfoList(hDevInfo);
                return 0;
            }

            //printf("DevicePath=%S\n", devDetail->DevicePath);

            ++deviceNo;

        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    return FOUND;
}


BOOL FindDevice()
{
    //FindDevice函数返回值不代表找到设备只是表面是否执行完毕，通过bTouchPad_FOUND、bTouchPad_I2C_FOUND和bOEMDriverExist的值来判定是否找到匹配的设备。

    HDEVINFO DeviceInfoSet;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD devIndex;

    bTouchPad_FOUND = FALSE;
    bTouchPad_I2C_FOUND = FALSE;

    TCHAR devInstance_ID[MAX_DEVICE_ID_LEN];//instance ID
    LPTSTR* devHwIDs;
    LPTSTR* devCompatibleIDs;

    SetLastError(NO_ERROR);

    // Create a Device Information Set with all present devices.

    DeviceInfoSet = SetupDiGetClassDevs(NULL, // All Classes
        0,
        0,
        DIGCF_ALLCLASSES | DIGCF_PRESENT); // All devices present on system
    if (DeviceInfoSet == INVALID_HANDLE_VALUE)
    {
        printf("SetupDiGetClassDevs err!\n");
        return FALSE;
    }

    //wprintf(TEXT("Search for TouchPad_hwID Device ID: [%s]\n"), TouchPad_COMPATIBLE_hwID);

    // Enumerate through all Devices.
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (devIndex = 0; SetupDiEnumDeviceInfo(DeviceInfoSet, devIndex, &DeviceInfoData); devIndex++)
    {
        devHwIDs = NULL;
        devCompatibleIDs = NULL;

        // determine instance ID
        if (CM_Get_Device_ID(DeviceInfoData.DevInst, devInstance_ID, MAX_DEVICE_ID_LEN, 0) != CR_SUCCESS) {
            devInstance_ID[0] = TEXT('\0');
        }
        ////wprintf(TEXT("TouchPad_hwID devInstance_ID: [%s]\n"), devInstance_ID); 

        devHwIDs = GetDevMultiSz(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID);
        devCompatibleIDs = GetDevMultiSz(DeviceInfoSet, &DeviceInfoData, SPDRP_COMPATIBLEIDS);

        if (FuzzyCompareHwIds(devHwIDs, TouchPad_COMPATIBLE_hwID) || FuzzyCompareHwIds(devCompatibleIDs, TouchPad_COMPATIBLE_hwID))
        {
            //printf("找到TouchPad触控板设备！\n");
            //wprintf(TEXT("TouchPad Device devInstance_ID= [%s]\n"), devInstance_ID);
            //wprintf(TEXT("TouchPad Device devHwIDs= [%s]\n"), devHwIDs[0]);
            wcscpy_s(TouchPad_hwID, devHwIDs[0]);
            wprintf(TEXT("TouchPad_hwID= [%s]\n"), TouchPad_hwID);
            NewLogFile(L"TouchPad_FOUND.txt");
           
            DelMultiSz(devHwIDs);
            DelMultiSz(devCompatibleIDs);

            bTouchPad_FOUND = TRUE;
            break;
        }

        DelMultiSz(devHwIDs);
        DelMultiSz(devCompatibleIDs);
    }


    if (!bTouchPad_FOUND) {
        printf("未找到TouchPad触控板设备！\n");
    }
    else {
        //生成I2C_hwID
        wchar_t* pstr;
        pstr = mystrcat(TouchPad_hwID, L"HID\\", L"ACPI\\");
        pstr = mystrcat(pstr, L"&Col02", L"\0");
        wcscpy_s(TouchPad_I2C_hwID, pstr);
        //wprintf(TEXT("TouchPad I2C_hwID= [%s]\n"), I2C_hwID);

        //验证I2C设备
        //printf("开始查找TouchPad触控板的I2C设备！\n");
        //wprintf(TEXT("Search for I2C_hwID Device ID: [%s]\n"), I2C_COMPATIBLE_hwID);

        for (devIndex = 0; SetupDiEnumDeviceInfo(DeviceInfoSet, devIndex, &DeviceInfoData); devIndex++)
        {
            devHwIDs = NULL;
            devCompatibleIDs = NULL;
            //
            // determine instance ID
            //
            if (CM_Get_Device_ID(DeviceInfoData.DevInst, devInstance_ID, MAX_DEVICE_ID_LEN, 0) != CR_SUCCESS) {
                printf("CM_Get_Device_ID err！\n");
                devInstance_ID[0] = TEXT('\0');
            }
            ////wprintf(TEXT("TouchPad I2C devInstance_ID: [%s]\n"), devInstance_ID);

            devHwIDs = GetDevMultiSz(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID);
            devCompatibleIDs = GetDevMultiSz(DeviceInfoSet, &DeviceInfoData, SPDRP_COMPATIBLEIDS);

            if (FuzzyCompareHwIds(devHwIDs, TouchPad_I2C_hwID) && FuzzyCompareHwIds(devCompatibleIDs, I2C_COMPATIBLE_hwID))
            {
                //printf("找到TouchPad触控板的I2C设备！\n");
                //wprintf(TEXT("TouchPad I2C Device devInstance_ID= [%s]\n"), devInstance_ID);
                //wprintf(TEXT("TouchPad I2C Device devHwIDs= [%s]\n"), devHwIDs[0]);
                wprintf(TEXT("TouchPad_I2C_hwID= [%s]\n"), TouchPad_I2C_hwID);
                SaveTouchPad_I2C_hwID();
   

                DelMultiSz(devHwIDs);
                DelMultiSz(devCompatibleIDs);

                bTouchPad_I2C_FOUND = TRUE;
                break;
            }

            DelMultiSz(devHwIDs);
            DelMultiSz(devCompatibleIDs);
        }

    }

    if (!bTouchPad_I2C_FOUND) {
        printf("未找到TouchPad触控板的I2C设备！\n");
    }
    else {
        BOOLEAN ret = GetDeviceOEMdriverInfo(DeviceInfoSet, &DeviceInfoData);
        if (!ret || !bOEMDriverExist) {//不存在第三方OEM驱动) 
            printf("OEM Driver Not Exist！\n");
        }

        ////
        //wchar_t szPath[MAX_PATH];
        //DWORD dwRet;
        //dwRet = GetCurrentDirectory(MAX_PATH, szPath);
        //if (dwRet == 0)    //返回零表示得到文件的当前路径失败
        //{
        //    printf("GetCurrentDirectory failed (%d)", GetLastError());
        //}
        //wprintf(TEXT("GetCurrentDirectory= [%s]\n"), szPath);

        ////设置安装路径
        //Set_InstallationSources_Directory(szPath);

        //DiShowUpdateDevice(NULL, DeviceInfoSet, &DeviceInfoData, 0, FALSE);
        ////DiShowUpdateDriver(NULL, OEMinf_FullName, 0, FALSE);

        //
    }

    //printf("FindDevice end\n");
    SetupDiDestroyDeviceInfoList(DeviceInfoSet);
    NewLogFile(L"Return_FindDevice.txt");
    return TRUE;
}


BOOL FindCurrentDriver(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo, _In_ PSP_DRVINFO_DATA DriverInfoData)
{
    SP_DEVINSTALL_PARAMS deviceInstallParams;
    WCHAR SectionName[LINE_LEN];
    WCHAR DrvDescription[LINE_LEN];
    WCHAR MfgName[LINE_LEN];
    WCHAR ProviderName[LINE_LEN];
    HKEY hKey = NULL;

    DWORD c;
    BOOL match = FALSE;


    ZeroMemory(&deviceInstallParams, sizeof(deviceInstallParams));
    deviceInstallParams.cbSize = sizeof(SP_DEVINSTALL_PARAMS);

    if (!SetupDiGetDeviceInstallParams(Devs, DevInfo, &deviceInstallParams)) {
        return FALSE;
    }

#ifdef DI_FLAGSEX_INSTALLEDDRIVER
    //
    // Set the flags that tell SetupDiBuildDriverInfoList to just put the
    // currently installed driver node in the list, and that it should allow
    // excluded drivers. This flag introduced in WinXP.
    //
    deviceInstallParams.FlagsEx |= (DI_FLAGSEX_INSTALLEDDRIVER | DI_FLAGSEX_ALLOWEXCLUDEDDRVS);

    if (SetupDiSetDeviceInstallParams(Devs, DevInfo, &deviceInstallParams)) {
        //
        // we were able to specify this flag, so proceed the easy way
        // we should get a list of no more than 1 driver
        //
        if (!SetupDiBuildDriverInfoList(Devs, DevInfo, SPDIT_CLASSDRIVER)) {
            return FALSE;
        }
        if (!SetupDiEnumDriverInfo(Devs, DevInfo, SPDIT_CLASSDRIVER,
            0, DriverInfoData)) {
            return FALSE;
        }
        //
        // we've selected the current driver
        //
        return TRUE;
    }
    deviceInstallParams.FlagsEx &= ~(DI_FLAGSEX_INSTALLEDDRIVER | DI_FLAGSEX_ALLOWEXCLUDEDDRVS);
#endif

    //
    // now search for drivers listed in the INF
    //
    //
    deviceInstallParams.Flags |= DI_ENUMSINGLEINF;
    deviceInstallParams.FlagsEx |= DI_FLAGSEX_ALLOWEXCLUDEDDRVS;

    if (!SetupDiSetDeviceInstallParams(Devs, DevInfo, &deviceInstallParams)) {
        return FALSE;
    }
    if (!SetupDiBuildDriverInfoList(Devs, DevInfo, SPDIT_CLASSDRIVER)) {
        return FALSE;
    }

    //
    // find the entry in the INF that was used to install the driver for
    // this device
    //
    for (c = 0; SetupDiEnumDriverInfo(Devs, DevInfo, SPDIT_CLASSDRIVER, c, DriverInfoData); c++) {
        if ((_tcscmp(DriverInfoData->MfgName, MfgName) == 0)
            && (_tcscmp(DriverInfoData->ProviderName, ProviderName) == 0)) {
            //
            // these two fields match, try more detailed info
            // to ensure we have the exact driver entry used
            //
            SP_DRVINFO_DETAIL_DATA detail;
            detail.cbSize = sizeof(SP_DRVINFO_DETAIL_DATA);
            if (!SetupDiGetDriverInfoDetail(Devs, DevInfo, DriverInfoData, &detail, sizeof(detail), NULL)
                && (GetLastError() != ERROR_INSUFFICIENT_BUFFER)) {
                continue;
            }
            if ((_tcscmp(detail.SectionName, SectionName) == 0) &&
                (_tcscmp(detail.DrvDescription, DrvDescription) == 0)) {
                match = TRUE;
                break;
            }
        }
    }
    if (!match) {
        SetupDiDestroyDriverInfoList(Devs, DevInfo, SPDIT_CLASSDRIVER);
    }
    return match;
}


BOOL GetDeviceOEMdriverInfo(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo)
{
    SP_DRVINFO_DATA driverInfoData;
    SP_DRVINFO_DETAIL_DATA driverInfoDetail;

    BOOL bSuccess = FALSE;
    bOEMDriverExist = FALSE;

    ZeroMemory(&driverInfoData, sizeof(driverInfoData));
    driverInfoData.cbSize = sizeof(driverInfoData);


    //FindCurrentDriver
    SP_DEVINSTALL_PARAMS deviceInstallParams;

    ZeroMemory(&deviceInstallParams, sizeof(deviceInstallParams));
    deviceInstallParams.cbSize = sizeof(SP_DEVINSTALL_PARAMS);

    if (!SetupDiGetDeviceInstallParams(Devs, DevInfo, &deviceInstallParams)) {
        printf("SetupDiGetDeviceInstallParams Failed!\n");
        return FALSE;
    }

    deviceInstallParams.FlagsEx |= (DI_FLAGSEX_INSTALLEDDRIVER | DI_FLAGSEX_ALLOWEXCLUDEDDRVS);

    if (SetupDiSetDeviceInstallParams(Devs, DevInfo, &deviceInstallParams)) {
        // we were able to specify this flag, so proceed the easy way
        // we should get a list of no more than 1 driver
        if (!SetupDiBuildDriverInfoList(Devs, DevInfo, SPDIT_CLASSDRIVER)) {
            printf("SetupDiBuildDriverInfoList Failed!\n");
            return FALSE;
        }
        if (!SetupDiEnumDriverInfo(Devs, DevInfo, SPDIT_CLASSDRIVER,
            0, &driverInfoData)) {

            printf("SetupDiEnumDriverInfo Failed!\n");
            SetupDiDestroyDriverInfoList(Devs, DevInfo, SPDIT_CLASSDRIVER);
            return FALSE;
        }
    }
    else {
        printf("SetupDiSetDeviceInstallParams Failed!\n");
        return FALSE;
    }


    // get useful driver information
    driverInfoDetail.cbSize = sizeof(SP_DRVINFO_DETAIL_DATA);
    if (!SetupDiGetDriverInfoDetail(Devs, DevInfo, &driverInfoData, &driverInfoDetail, sizeof(SP_DRVINFO_DETAIL_DATA), NULL) &&
        GetLastError() != ERROR_INSUFFICIENT_BUFFER) {

        // no information about driver or section
        printf("SetupDiGetDriverInfoDetail err!\n");
        goto final;
    }
    if (!driverInfoDetail.InfFileName[0] || !driverInfoDetail.SectionName[0]) {
        printf("driverInfoDetail err!\n");
        goto final;
    }

    //wprintf(TEXT("driverInfoDetail.InfFileName[0]: [%s]\n"), driverInfoDetail.InfFileName);


    // pretend to do the file-copy part of a driver install
    // to determine what files are used
    // the specified driver must be selected as the active driver
    if (!SetupDiSetSelectedDriver(Devs, DevInfo, &driverInfoData)) {
        printf("SetupDiSetSelectedDriver err!\n");
        goto final;
    }

    if (wcscmp(driverInfoData.Description, OEM_DevDescription) == 0 && wcscmp(driverInfoData.MfgName, OEM_MfgName) == 0 && wcscmp(driverInfoData.ProviderName, OEM_ProviderName) == 0) {
        //wprintf(TEXT("driverInfoData.Description: [%s]\n"), driverInfoData.Description);
        //wprintf(TEXT("driverInfoData.MfgName: [%s]\n"), driverInfoData.MfgName);
        //wprintf(TEXT("driverInfoData.ProviderName: [%s]\n"), driverInfoData.ProviderName);

        bOEMDriverExist = TRUE;//本OEM驱动存在
        wcscpy_s(OEMinf_FullName, driverInfoDetail.InfFileName);
        //wprintf(TEXT("OEMinf_FullName= [%s]\n"), OEMinf_FullName);

        // 获得INF目录
        WCHAR szPath[MAX_PATH];  // 获得系统目录
        GetWindowsDirectory(szPath, sizeof(szPath));
        // 格式化文件路径
        wcscat_s(szPath, L"\\INF\\");
        //wprintf(TEXT("INF PATH= [%s]\n"), szPath);

        //生成OEMinf_name
        wchar_t* pstr;
        pstr = mystrcat(OEMinf_FullName, szPath, L"\0");//szPath//L"C:\\WINDOWS\\INF\\"
        wcscpy_s(OEMinf_name, pstr);
        wprintf(TEXT("OEMinf_name= [%s]\n"), OEMinf_name);
        SaveOEMDriverName();

        //printf("GetDeviceOEMdriverInfo ok\n");
        bSuccess = TRUE;
    }
    else {
        wprintf(TEXT("driverInfoDetail.InfFileName[0]: [%s]\n"), driverInfoDetail.InfFileName);
    }


    final:

    SetupDiDestroyDriverInfoList(Devs, DevInfo, SPDIT_CLASSDRIVER);
    return bSuccess;

}


BOOL DirExist(LPCWSTR szFilePathName)
{
    if (GetFileAttributes(szFilePathName) == FILE_ATTRIBUTE_DIRECTORY) {
        return TRUE;
    }

    return FALSE;
}


BOOL FileExist(LPCWSTR szFilePathName)
{
    HANDLE hFile = CreateFile(szFilePathName,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }

    CloseHandle(hFile);
    return TRUE;
}


BOOL MainFileExist(LPCWSTR szFileName)
{
    WCHAR szFilePath[MAX_PATH];
    wcscpy_s(szFilePath, exeFilePath);
    wcscat_s(szFilePath, L"\\");
    wcscat_s(szFilePath, szFileName);

    HANDLE hFile = CreateFile(szFilePath,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }

    CloseHandle(hFile);
    return TRUE;
}

BOOL DrvFileExist(LPCWSTR szFileName)
{
    WCHAR szFilePath[MAX_PATH];
    wcscpy_s(szFilePath, exeFilePath);
    wcscat_s(szFilePath, L"\\Driver\\");
    wcscat_s(szFilePath, szFileName);

    HANDLE hFile = CreateFile(szFilePath,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }

    CloseHandle(hFile);
    return TRUE;
}


BOOL LogFileExist(LPCWSTR szFileName)
{
    WCHAR szFilePath[MAX_PATH];
    wcscpy_s(szFilePath, exeFilePath);
    wcscat_s(szFilePath, L"\\LogFile\\");
    wcscat_s(szFilePath, szFileName);

    HANDLE hFile = CreateFile(szFilePath,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
 
    if (hFile == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }

    CloseHandle(hFile);
    return TRUE;
}


void DelLogFile(LPCWSTR szFileName)
{
    WCHAR szFilePath[MAX_PATH];
    wcscpy_s(szFilePath, exeFilePath);
    wcscat_s(szFilePath, L"\\LogFile\\");
    wcscat_s(szFilePath, szFileName);

    DeleteFile(szFilePath);
}


BOOL NewLogFile(LPCWSTR szFileName)
{
    WCHAR szFilePath[MAX_PATH];
    wcscpy_s(szFilePath, exeFilePath);
    wcscat_s(szFilePath, L"\\LogFile\\");
    wcscat_s(szFilePath, szFileName);

    HANDLE hFile = CreateFile(szFilePath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }

    CloseHandle(hFile);
    return TRUE;

}


BOOL GetTouchPad_I2C_hwID()
{
    WCHAR szFilePath[MAX_PATH];
    wcscpy_s(szFilePath, exeFilePath);
    wcscat_s(szFilePath, L"\\LogFile\\");
    wcscat_s(szFilePath, L"TouchPad_I2C_FOUND.txt");

    HANDLE hFile = CreateFile(szFilePath,
        GENERIC_READ,
        0, //FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }

    DWORD filesize = GetFileSize(hFile, NULL);
    char* buffer = new char[filesize + 1];//最后一位为'/0',C-Style字符串的结束符。
    memset(buffer, 0, filesize + 1);
    DWORD readsize = 0;
    if (!ReadFile(hFile, buffer, filesize, &readsize, NULL)) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }

    CloseHandle(hFile);//关闭句柄
    buffer[filesize] = 0;


    char* pszMultiByte = buffer;
    int iSize;
    wchar_t* pwszUnicode;

    //返回接受字符串所需缓冲区的大小，已经包含字符结尾符'\0'
    iSize = MultiByteToWideChar(CP_ACP, 0, pszMultiByte, -1, NULL, 0);
    pwszUnicode = (wchar_t*)malloc(iSize * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, pszMultiByte, -1, pwszUnicode, iSize);


    wcscpy_s(TouchPad_I2C_hwID, pwszUnicode);

    //善后工作
    delete[]buffer;//注意是delete[]而不是delete
    return TRUE;

}


BOOL GetOEMDriverName()
{
    WCHAR szFilePath[MAX_PATH];
    wcscpy_s(szFilePath, exeFilePath);
    wcscat_s(szFilePath, L"\\LogFile\\");
    wcscat_s(szFilePath, L"OEMDriverName.txt");


    HANDLE hFile = CreateFile(szFilePath,
        GENERIC_READ,
        0, //FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE || GetLastError() == ERROR_FILE_NOT_FOUND) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }

 

    DWORD filesize = GetFileSize(hFile, NULL);
    char* buffer = new char[filesize + 1];//最后一位为'/0',C-Style字符串的结束符。
    memset(buffer, 0, filesize + 1);
    DWORD readsize = 0;
    if (!ReadFile(hFile, buffer, filesize, &readsize, NULL)) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }

    CloseHandle(hFile);//关闭句柄
    buffer[filesize] = 0;


    char* pszMultiByte = buffer;
    int iSize;
    wchar_t* pwszUnicode;

    //返回接受字符串所需缓冲区的大小，已经包含字符结尾符'\0'
    iSize = MultiByteToWideChar(CP_ACP, 0, pszMultiByte, -1, NULL, 0); 
    pwszUnicode = (wchar_t*)malloc(iSize * sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP, 0, pszMultiByte, -1, pwszUnicode, iSize);


    wcscpy_s(OEMinf_name, pwszUnicode);

     // 获得INF目录
    WCHAR szPath[MAX_PATH];  // 获得系统目录
    GetWindowsDirectory(szPath, sizeof(szPath));
    // 格式化文件路径
    wcscat_s(szPath, L"\\INF\\");
    wcscat_s(szPath, OEMinf_name);
    wcscpy_s(OEMinf_FullName, szPath);


    //善后工作
    delete[]buffer;//注意是delete[]而不是delete
    return TRUE;
}


BOOL SaveTouchPad_I2C_hwID()
{
    WCHAR szFilePath[MAX_PATH];
    wcscpy_s(szFilePath, exeFilePath);
    wcscat_s(szFilePath, L"\\LogFile\\");
    wcscat_s(szFilePath, L"TouchPad_I2C_FOUND.txt");

    HANDLE hFile = CreateFile(szFilePath,
        GENERIC_READ | GENERIC_WRITE, 
        0, //FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE || ::GetLastError() == ERROR_FILE_NOT_FOUND) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }

    int iSize;
    char* pszMultiByte;

    //返回接受字符串所需缓冲区的大小，已经包含字符结尾符'\0'
    iSize = WideCharToMultiByte(CP_ACP, 0, TouchPad_I2C_hwID, -1, NULL, 0, NULL, NULL); //iSize =wcslen(TouchPad_I2C_hwID)+1
    pszMultiByte = (char*)malloc(iSize * sizeof(char)); //不需要 pszMultiByte = (char*)malloc(iSize*sizeof(char)+1);
    WideCharToMultiByte(CP_ACP, 0, TouchPad_I2C_hwID, -1, pszMultiByte, iSize, NULL, NULL);

    DWORD writesize = 0;
    WriteFile(hFile, pszMultiByte, iSize-1, &writesize, NULL);
    CloseHandle(hFile);//关闭句柄


    return TRUE;
}


BOOL SaveOEMDriverName()
{
    WCHAR szFilePath[MAX_PATH];
    wcscpy_s(szFilePath, exeFilePath);
    wcscat_s(szFilePath, L"\\LogFile\\");
    wcscat_s(szFilePath, L"OEMDriverName.txt");

    HANDLE hFile = CreateFile(szFilePath,
        GENERIC_READ | GENERIC_WRITE,
        0, //FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE || ::GetLastError() == ERROR_FILE_NOT_FOUND) {
        CloseHandle(hFile);//关闭句柄
        return FALSE;
    }


    int iSize;
    char* pszMultiByte;

    //返回接受字符串所需缓冲区的大小，已经包含字符结尾符'\0'
    iSize = WideCharToMultiByte(CP_ACP, 0, OEMinf_name, -1, NULL, 0, NULL, NULL); //iSize =wcslen(OEMinf_name)+1
    pszMultiByte = (char*)malloc(iSize * sizeof(char)); //不需要 pszMultiByte = (char*)malloc(iSize*sizeof(char)+1);
    WideCharToMultiByte(CP_ACP, 0, OEMinf_name, -1, pszMultiByte, iSize, NULL, NULL);

    DWORD writesize = 0;
    WriteFile(hFile, pszMultiByte, iSize-1, &writesize, NULL);
    CloseHandle(hFile);//关闭句柄

    return TRUE;
}


void Install() {
    INT nRet = 0;////-0 系统内存或资源不足//--ERROR_BAD_FORMAT.EXE文件格式无效（比如不是32位应用程序）//--ERROR_FILE_NOT_FOUND 指定的文件设有找到//--ERROR_PATH_NOT_FOUND
    INT retry = 0;
    INT nTry = 0;

    while (Rescan())break;//重新扫描设备

    //批处理模式安装微软ACPI\MSFT0001标准硬件ID驱动

    //清理历史记录文件
    DelLogFile(L"Return_InstDrv.txt");
    DelLogFile(L"InstDrvSuccess.txt");

    retry = 0;
    nTry = 5;
    //char cmdLine[MAX_PATH];
    //memset(cmdLine, 0, MAX_PATH);
    //strcat_s(cmdLine, cmdFilePath);
    //strcpy_s(cmdLine, "\\cmd.exe /c InstDrv.bat");
    //nRet = WinExec(cmdLine, SW_HIDE);

    nRet = WinExec("cmd.exe /c InstDrv.bat", SW_HIDE);
    if (nRet > 30) {
waitInstBAT:
        Sleep(500);
        retry++;

        if (retry > nTry) {
            goto NextInstStep;//跳过批处理安装方式
        }

        if (LogFileExist(L"TouchPad_I2C_FOUND.txt")) {
            nTry=20;
        }

        if (!LogFileExist(L"Return_InstDrv.txt")) {//InstDrv.bat未执行结束
            goto waitInstBAT;
        }

        if (LogFileExist(L"InstDrvSuccess.txt")) {//InstDrv.bat安装驱动成功
            DelLogFile(L"Return_InstDrv.txt");
            DelLogFile(L"InstDrvSuccess.txt");
            goto InstSuccess;//直接跳到结尾
        }

        DelLogFile(L"Return_InstDrv.txt");
        DelLogFile(L"InstDrvSuccess.txt");
    }


NextInstStep:
    //清理历史记录文件
    while (LogFileExist(L"Return_FindDevice.txt"))DelLogFile(L"Return_FindDevice.txt");
    while (LogFileExist(L"Return_UninstallDriver.txt"))DelLogFile(L"Return_UninstallDriver.txt");
    while (LogFileExist(L"TouchPad_FOUND.txt"))DelLogFile(L"TouchPad_FOUND.txt");

    retry = 0;
    nTry = 3;
FindDev:
    nRet = WinExec("PtpDrvMgr.exe FindDevice", SW_HIDE);
    if (nRet < 31) {
        MessageBox(NULL, L"WinExec调用FindDevice子程序失败！取消安装，请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }

    retry++;
    Sleep(1500);
    if (retry > nTry) {
        MessageBox(NULL, L"查找设备失败！取消安装，请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }

    if (!LogFileExist(L"TouchPad_I2C_FOUND.txt")) {
        nTry= 5;
    }

    if (!LogFileExist(L"Return_FindDevice.txt")) {//FindDevice未执行结束
        goto FindDev;
    }

    //FindDevice执行完毕
    DelLogFile(L"Return_FindDevice.txt");

    if (!LogFileExist(L"TouchPad_FOUND.txt")) {
        MessageBox(NULL, L"未找到匹配的触控板设备，无法安装驱动。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }
    DelLogFile(L"TouchPad_FOUND.txt");

    if (!LogFileExist(L"TouchPad_I2C_FOUND.txt")) {
        MessageBox(NULL, L"未找到匹配的I2C总线触控板设备，无法安装驱动。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }



    retry = 0;
InstDrv:
    nRet = WinExec("PtpDrvMgr.exe InstallDriver", SW_HIDE);
    if (nRet < 31) {
        MessageBox(NULL, L"WinExec调用InstallDriver子程序失败！请稍后再试。", L"PtpDrvMgr", MB_OK);
    }
    else {
        Sleep(3000);
        retry++;
        if (retry > 5) {
            MessageBox(NULL, L"InstallDriver安装驱动失败！请稍后再试。", L"PtpDrvMgr", MB_OK);
            ExitCode = EXIT_FAIL;
            return;
        }

        if (!LogFileExist(L"Return_InstallDriver.txt")) {//InstallDriver未执行结束
            goto InstDrv;
        }     
    }

    if (!LogFileExist(L"Return_InstallDriver.txt")) {//InstallDriver未执行结束
        MessageBox(NULL, L"找到匹配的I2C总线触控板设备，安装驱动失败，请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }
    DelLogFile(L"Return_InstallDriver.txt");


    while (Rescan())break;//重新扫描设备



    //再次验证
CheckDev:
    nRet = WinExec("PtpDrvMgr.exe FindDevice", SW_HIDE);
    if (nRet < 31) {
        MessageBox(NULL, L"二次验证，WinExec调用FindDevice子程序失败！取消安装，请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }

    retry++;
    Sleep(1500);
    if (retry > 3) {
        MessageBox(NULL, L"二次验证，查找设备失败！请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }

    if (!LogFileExist(L"Return_FindDevice.txt")) {//FindDevice未执行结束
        goto FindDev;
    }

    //FindDevice执行完毕
    DelLogFile(L"Return_FindDevice.txt");

    if (!LogFileExist(L"OEMDriverName.txt")) {
        MessageBox(NULL, L"二次验证，安装驱动失败，请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }
    

InstSuccess:
    //清理历史记录文件
    while (LogFileExist(L"Return_FindDevice.txt"))DelLogFile(L"Return_FindDevice.txt");
    while (LogFileExist(L"Return_UninstallDriver.txt"))DelLogFile(L"Return_UninstallDriver.txt");
    while (LogFileExist(L"TouchPad_FOUND.txt"))DelLogFile(L"TouchPad_FOUND.txt");

    //存在驱动
    MessageBox(NULL, L"安装驱动成功！", L"PtpDrvMgr", MB_OK | MB_DEFBUTTON1);
    ExitCode = EXIT_OK;
}

void Uninstall() {
    INT nRet = 0;////-0 系统内存或资源不足//--ERROR_BAD_FORMAT.EXE文件格式无效（比如不是32位应用程序）//--ERROR_FILE_NOT_FOUND 指定的文件设有找到//--ERROR_PATH_NOT_FOUND
    INT retry = 0;
    INT nTry = 0;

    while (Rescan())break;//重新扫描设备

    //清理历史记录文件
    while(LogFileExist(L"Return_FindDevice.txt"))DelLogFile(L"Return_FindDevice.txt");
    while (LogFileExist(L"Return_UninstallDriver.txt"))DelLogFile(L"Return_UninstallDriver.txt");
    while (LogFileExist(L"TouchPad_FOUND.txt"))DelLogFile(L"TouchPad_FOUND.txt");


    retry = 0;
    nTry = 3;
FindDev:
    nRet = WinExec("PtpDrvMgr.exe FindDevice", SW_HIDE);
    if (nRet < 31) {
        MessageBox(NULL, L"WinExec调用FindDevice子程序失败！取消卸载，请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }

    retry++;
    Sleep(1500);
    if (retry > nTry) {
        MessageBox(NULL, L"查找设备失败！取消卸载，请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }

    if (!LogFileExist(L"TouchPad_I2C_FOUND.txt")) {
        nTry = 5;
    }

    if (!LogFileExist(L"Return_FindDevice.txt")) {//FindDevice未执行结束
        goto FindDev;
    }

    //FindDevice执行完毕
    DelLogFile(L"Return_FindDevice.txt");

    if (!LogFileExist(L"TouchPad_FOUND.txt")) {
        MessageBox(NULL, L"未找到匹配的触控板设备，无需卸载驱动，可直接卸载程序。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_OK;//这里不用安装，直接返回成功
        return;
    }
    DelLogFile(L"TouchPad_FOUND.txt");

    if (!LogFileExist(L"TouchPad_I2C_FOUND.txt")) {
        MessageBox(NULL, L"未找到匹配的I2C总线触控板设备，无需卸载驱动，可直接卸载程序。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_OK;//这里不用安装，直接返回成功
        return;
    }



    retry = 0;
UninstDrv:
    nRet = WinExec("PtpDrvMgr.exe UninstallDriver", SW_HIDE);
    if (nRet < 31) {
        MessageBox(NULL, L"WinExec调用UninstallDriver子程序失败！请稍后再试。", L"PtpDrvMgr", MB_OK);
    }
    else {
        Sleep(2000);
        retry++;
        if (retry > 5) {//批处理模式卸载驱动
            nTry = 0;
            //char cmdLine[MAX_PATH];
            //memset(cmdLine, 0, MAX_PATH);
            //strcat_s(cmdLine, cmdFilePath);
            //strcpy_s(cmdLine, "\\cmd.exe /c UninstDrv.bat");
UninstBAT:     
            retry = 0;
            nTry++;

            //nRet = WinExec(cmdLine, SW_HIDE);
            nRet = WinExec("cmd.exe /c UninstDrv.bat", SW_HIDE);
waitUninstBAT:
                Sleep(500);
                retry++;

                if (retry > 10) {
                    goto UninstBAT;//超时再次批处理卸载方式
                }

                if (nTry > 3) {
                    goto NextUninstStep;//跳过批处理卸载方式
                }

                if (!LogFileExist(L"Return_UninstDrv.txt")) {//UninstDrv.bat未执行结束
                    goto waitUninstBAT;
                }

                if (LogFileExist(L"UninstDrvSuccess.txt")) {//UninstDrv.bat卸载驱动成功
                    DelLogFile(L"Return_UninstDrv.txt");
                    DelLogFile(L"UninstDrvSuccess.txt");
                    goto UninstDrvSuccess;//直接跳到结尾
                }

                DelLogFile(L"Return_UninstDrv.txt");
                DelLogFile(L"UninstDrvSuccess.txt");


NextUninstStep:
            MessageBox(NULL, L"UninstallDriver卸载驱动失败！请稍后再试。", L"PtpDrvMgr", MB_OK);
            ExitCode = EXIT_FAIL;
            return;
        }

        if (!LogFileExist(L"Return_UninstallDriver.txt")) {//InstallDriver未执行结束
            goto UninstDrv;
        }        
    }

    if (!LogFileExist(L"Return_UninstallDriver.txt")) {//InstallDriver未执行结束
        MessageBox(NULL, L"找到匹配的I2C总线触控板设备，卸载驱动失败，请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }
    DelLogFile(L"Return_UninstallDriver.txt");


    while (Rescan())break;//重新扫描设备



    //再次验证
CheckDev:
    nRet = WinExec("PtpDrvMgr.exe FindDevice", SW_HIDE);
    if (nRet < 31) {
        MessageBox(NULL, L"二次验证，WinExec调用FindDevice子程序失败！取消安装，请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }

    retry++;
    Sleep(1500);
    if (retry > 3) {
        MessageBox(NULL, L"二次验证，查找设备失败！请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }

    if (!LogFileExist(L"Return_FindDevice.txt")) {//FindDevice未执行结束
        goto FindDev;
    }

    //FindDevice执行完毕
    DelLogFile(L"Return_FindDevice.txt");

    if (LogFileExist(L"OEMDriver.txt")) {
        MessageBox(NULL, L"二次验证，卸载驱动失败，请稍后再试。", L"PtpDrvMgr", MB_OK);
        ExitCode = EXIT_FAIL;
        return;
    }


UninstDrvSuccess:
    //清理历史记录文件
    while (LogFileExist(L"Return_FindDevice.txt"))DelLogFile(L"Return_FindDevice.txt");
    while (LogFileExist(L"Return_UninstallDriver.txt"))DelLogFile(L"Return_UninstallDriver.txt");
    while (LogFileExist(L"TouchPad_FOUND.txt"))DelLogFile(L"TouchPad_FOUND.txt");

    //不存在驱动
    MessageBox(NULL, L"卸载驱动成功！", L"PtpDrvMgr", MB_OK | MB_DEFBUTTON1);
    ExitCode = EXIT_OK;

}

BOOL InstallDriver() {
    //
    INT ret = 0;
    
    if (!GetTouchPad_I2C_hwID()) {
        goto END;
    }

    ret = UpdateDriver();
    if (!ret) {
        return FALSE;
    }


END:
    NewLogFile(L"Return_InstallDriver.txt");
    return ret;
}


BOOL UninstallDriver() {
    //
    INT ret = 0;

    if (GetOEMDriverName()) {
        bOEMDriverExist = TRUE;
    }

    ret = RemoveDriver();
    if (!ret) {
        return FALSE;
    }

    NewLogFile(L"Return_UninstallDriver.txt");
    return ret;

}


BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
    if (CTRL_CLOSE_EVENT == dwCtrlType) {
        //printf("exit %d\n", dwCtrlType);
    }

    ::MessageBox(NULL, L"退出！", L"提示", MB_OKCANCEL);

    return TRUE;
}


BOOL FuzzyCompareHwIds(PZPWSTR Array, const wchar_t* MatchHwId)
{
    if (Array) {
        while (Array[0]) {
            if (wcscmp(Array[0], MatchHwId) == 0) {
                return TRUE;
            }
            Array++;
        }
    }
    return FALSE;
}

void DelMultiSz(_In_opt_ __drv_freesMem(object) PZPWSTR Array)
{
    if (Array) {
        Array--;
        if (Array[0]) {
            delete[] Array[0];
        }
        delete[] Array;
    }
}


__drv_allocatesMem(object)
LPTSTR* GetDevMultiSz(_In_ HDEVINFO Devs, _In_ PSP_DEVINFO_DATA DevInfo, _In_ DWORD Prop)
{
    LPTSTR buffer;
    DWORD size;
    DWORD reqSize;
    DWORD dataType;
    LPTSTR* array;
    DWORD szChars;

    size = 8192; // initial guess, nothing magic about this
    buffer = new TCHAR[(size / sizeof(TCHAR)) + 2];
    if (!buffer) {
        return NULL;
    }
    while (!SetupDiGetDeviceRegistryProperty(Devs, DevInfo, Prop, &dataType, (LPBYTE)buffer, size, &reqSize)) {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
            goto failed;
        }
        if (dataType != REG_MULTI_SZ) {
            goto failed;
        }
        size = reqSize;
        delete[] buffer;
        buffer = new TCHAR[(size / sizeof(TCHAR)) + 2];
        if (!buffer) {
            goto failed;
        }

        //wprintf(TEXT("Device ID is: [%s]\n"), buffer);
    }
    szChars = reqSize / sizeof(TCHAR);
    buffer[szChars] = TEXT('\0');
    buffer[szChars + 1] = TEXT('\0');
    array = GetMultiSzIndexArray(buffer);
    if (array) {
        return array;
    }

failed:
    if (buffer) {
        delete[] buffer;
    }
    return NULL;
}


__drv_allocatesMem(object)
LPTSTR* GetMultiSzIndexArray(_In_ __drv_aliasesMem LPTSTR MultiSz)
{
    LPTSTR scan;
    LPTSTR* array;
    int elements;

    for (scan = MultiSz, elements = 0; scan[0]; elements++) {
        scan += _tcslen(scan) + 1;
    }
    array = new LPTSTR[elements + 2];
    if (!array) {
        return NULL;
    }
    array[0] = MultiSz;
    array++;
    if (elements) {
        for (scan = MultiSz, elements = 0; scan[0]; elements++) {
            array[elements] = scan;
            scan += _tcslen(scan) + 1;
        }
    }
    array[elements] = NULL;
    return array;
}

wchar_t* mystrcat(const wchar_t* str1, const wchar_t* str2, const wchar_t* str3)//str1是原来的字符串，str2是str1中的某段需要替换字符串，str3是替换str2的字符串
{
    int strlen1 = 0, strlen2 = 0, strlen3 = 0;
    strlen1 = wcslen(str1);
    strlen2 = wcslen(str2);
    strlen3 = wcslen(str3);
    int num = 0, num1 = 0, num2 = 0, num3 = 0;//num1从str1起。。。
    int m = 0;//替代num1的值
    int count = 0;//记录有几个像str2的字符串
    int len = 2 * (count * (strlen3 - strlen2) + strlen1 + 1);
    len = (strlen1 * strlen3 / strlen2 + 1) * 2;
    wchar_t* p_str = (wchar_t*)malloc(len);
    wchar_t* str = p_str;
    for (num1 = 0; num1 < strlen1; num1++)
    {
        m = num1;
        num2 = 0;
        while (num2 < strlen2)
        {
            if (str1[m] != str2[num2])
                break;
            m++;
            num2++;
        }
        if (num2 >= strlen2)
        {
            for (num3 = 0; num3 < strlen3; num3++)
                str[num++] = str3[num3];
            num1 += (strlen2 - 1);
            count++;
        }
        else
            str[num++] = str1[num1];
    }
    len = 2 * (count * (strlen3 - strlen2) + strlen1 + 1);
    str[len / 2 - 1] = L'\0';
    return p_str;
}


BOOL RemoveDevice()
{
    HDEVINFO DeviceInfoSet;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD devIndex;
    BOOLEAN bMATCH = FALSE;
    BOOLEAN bFOUND = FALSE;
    BOOLEAN bSuccess = FALSE;


    // Create a Device Information Set with all present devices.

    DeviceInfoSet = SetupDiGetClassDevs(NULL, // All Classes
        0,
        0,
        DIGCF_ALLCLASSES | DIGCF_PRESENT); // All devices present on system
    if (DeviceInfoSet == INVALID_HANDLE_VALUE)
    {
        //printf("RemoveDevice SetupDiGetClassDevs err!\n");
        return FALSE;
    }

    //wprintf(TEXT("RemoveDevice Search for TouchPad_hwID Device ID: [%s]\n"), TouchPad_COMPATIBLE_hwID);

    // Enumerate through all Devices.
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (devIndex = 0; SetupDiEnumDeviceInfo(DeviceInfoSet, devIndex, &DeviceInfoData); devIndex++)
    {
        TCHAR devInstance_ID[MAX_DEVICE_ID_LEN];//instance ID
        LPTSTR* hwIds = NULL;
        LPTSTR* compatIds = NULL;
        //
        // determine instance ID
        //
        if (CM_Get_Device_ID(DeviceInfoData.DevInst, devInstance_ID, MAX_DEVICE_ID_LEN, 0) != CR_SUCCESS) {
            devInstance_ID[0] = TEXT('\0');
        }
        ////wprintf(TEXT("devInstance_ID: [%s]\n"), devInstance_ID); 

        hwIds = GetDevMultiSz(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID);
        compatIds = GetDevMultiSz(DeviceInfoSet, &DeviceInfoData, SPDRP_COMPATIBLEIDS);

        if (FuzzyCompareHwIds(hwIds, TouchPad_COMPATIBLE_hwID) || FuzzyCompareHwIds(compatIds, TouchPad_COMPATIBLE_hwID))
        {
            //printf("RemoveDevice 找到TouchPad触控板设备！\n");
            //wprintf(TEXT("RemoveDevice TouchPad Device devInstance_ID= [%s]\n"), devInstance_ID);
            //wprintf(TEXT("RemoveDevice TouchPad Device hwIds= [%s]\n"), hwIds[0]);
            wcscpy_s(TouchPad_hwID, hwIds[0]);
            //wprintf(TEXT("RemoveDevice TouchPad_hwID= [%s]\n"), TouchPad_hwID);

            bMATCH = TRUE;
            DelMultiSz(hwIds);
            DelMultiSz(compatIds);
            break;
        }

        DelMultiSz(hwIds);
        DelMultiSz(compatIds);
    }


    if (bMATCH)
    {
        //生成I2C_hwID
        wchar_t* pstr;
        pstr = mystrcat(TouchPad_hwID, L"HID\\", L"ACPI\\");
        pstr = mystrcat(pstr, L"&Col02", L"\0");
        wcscpy_s(TouchPad_I2C_hwID, pstr);
        //wprintf(TEXT("RemoveDevice TouchPad I2C_hwID= [%s]\n"), I2C_hwID);

        //验证I2C设备
        //printf("RemoveDevice 开始查找TouchPad触控板的I2C设备！\n");
        //wprintf(TEXT("RemoveDevice Search for I2C_hwID Device ID: [%s]\n"), I2C_COMPATIBLE_hwID);

        for (devIndex = 0; SetupDiEnumDeviceInfo(DeviceInfoSet, devIndex, &DeviceInfoData); devIndex++)
        {
            TCHAR devInstID[MAX_DEVICE_ID_LEN];//instance ID
            LPTSTR* devHwIds = NULL;
            LPTSTR* devCompatIds = NULL;
            //
            // determine instance ID
            //
            if (CM_Get_Device_ID(DeviceInfoData.DevInst, devInstID, MAX_DEVICE_ID_LEN, 0) != CR_SUCCESS) {
                devInstID[0] = TEXT('\0');
            }
            ////wprintf(TEXT("devInstID: [%s]\n"), devInstID);

            devHwIds = GetDevMultiSz(DeviceInfoSet, &DeviceInfoData, SPDRP_HARDWAREID);
            devCompatIds = GetDevMultiSz(DeviceInfoSet, &DeviceInfoData, SPDRP_COMPATIBLEIDS);

            if (FuzzyCompareHwIds(devHwIds, TouchPad_I2C_hwID) && FuzzyCompareHwIds(devCompatIds, I2C_COMPATIBLE_hwID))
            {
                //printf("RemoveDevice 找到TouchPad触控板的I2C设备！\n");
                //wprintf(TEXT("RemoveDevice TouchPad I2C Device devInstID= [%s]\n"), devInstID);
                //wprintf(TEXT("RemoveDevice TouchPad I2C Device devHwIds= [%s]\n"), devHwIds[0]);

                BOOLEAN ret = GetDeviceOEMdriverInfo(DeviceInfoSet, &DeviceInfoData);

                bFOUND = TRUE;
                DelMultiSz(devHwIds);
                DelMultiSz(devCompatIds);
                break;
            }

            DelMultiSz(devHwIds);
            DelMultiSz(devCompatIds);
        }


        if (!bFOUND) {
            //printf("RemoveDevice 未找到TouchPad触控板的I2C设备！\n");
        }
        else {

            SP_REMOVEDEVICE_PARAMS rmdParams;
            SP_DEVINSTALL_PARAMS devParams;

            // need hardware ID before trying to remove, as we wont have it after
            SP_DEVINFO_LIST_DETAIL_DATA devInfoListDetail;

            devInfoListDetail.cbSize = sizeof(devInfoListDetail);
            if (!SetupDiGetDeviceInfoListDetail(DeviceInfoSet, &devInfoListDetail)) {
                // skip this
                //printf("RemoveDevice SetupDiGetDeviceInfoListDetail not exist！\n");
                bSuccess = TRUE;//
                goto END;
            }

            rmdParams.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
            rmdParams.ClassInstallHeader.InstallFunction = DIF_REMOVE;
            rmdParams.Scope = DI_REMOVEDEVICE_GLOBAL;
            rmdParams.HwProfile = 0;
            if (!SetupDiSetClassInstallParams(DeviceInfoSet, &DeviceInfoData, &rmdParams.ClassInstallHeader, sizeof(rmdParams)) ||
                !SetupDiCallClassInstaller(DIF_REMOVE, DeviceInfoSet, &DeviceInfoData)) {
                // failed to invoke DIF_REMOVE
                //printf("RemoveDevice failed to invoke DIF_REMOVE！\n");
            }
            else {
                // see if device needs reboot
                devParams.cbSize = sizeof(devParams);
                if (SetupDiGetDeviceInstallParams(DeviceInfoSet, &DeviceInfoData, &devParams) && (devParams.Flags & (DI_NEEDRESTART | DI_NEEDREBOOT))) {
                    // reboot required
                    //printf("RemoveDevice reboot required！\n");
                }
                else {
                    // appears to have succeeded
                    //printf("RemoveDevice succeeded！\n");
                }

            }


            bSuccess = TRUE;//
        }
    }
    else {
        //printf("RemoveDevice 未找到TouchPad触控板设备！\n");
    }


END:
    SetupDiDestroyDeviceInfoList(DeviceInfoSet);
    return bSuccess;
}

BOOL RemoveDriver()
{
    BOOLEAN bSuccess = FALSE;

    if (bOEMDriverExist) {//存在驱动
        printf("start RemoveDriver\n");

        //删除驱动 
        if (!DiUninstallDriver(NULL, OEMinf_FullName, DIURFLAG_NO_REMOVE_INF, FALSE)) {
            printf("DiUninstallDriver Fail %d!\n", GetLastError());
            return FALSE;
        }
        else {
            printf("DiUninstallDriver ok！\n");
        }

        //DPDelete
        if (!SetupUninstallOEMInf(OEMinf_name, PtpDrvMgr_FLAG_FORCE, NULL)) {
            if (GetLastError() == ERROR_INF_IN_USE_BY_DEVICES) {
                printf("ERROR_INF_IN_USE_BY_DEVICES！\n");
            }
            else if (GetLastError() == ERROR_NOT_AN_INSTALLED_OEM_INF) {
                printf("ERROR_NOT_AN_INSTALLED_OEM_INF！\n");
            }
            else {
                printf("SetupUninstallOEMInf FAILED！\n");
            }
        }
        else {
            printf("SetupUninstallOEMInf ok！\n");
            bSuccess = TRUE;
        }
    }

    return bSuccess;
}


BOOL UpdateDriver()
{

    BOOL reboot = FALSE;
    LPCTSTR hwid = NULL;
    LPCTSTR inf = NULL;
    DWORD flags = INSTALLFLAG_FORCE;

    BOOLEAN bSuccess = FALSE;

    //printf("start UpdateDriver\n");

    inf = inf_FullPathName;
    if (!inf[0]) {
        printf("inf err！\n");
        return FALSE;
    }
    //wprintf(TEXT("update inf= [%s]\n"), inf);

    hwid = TouchPad_I2C_hwID;
    if (!hwid[0]) {
        printf("hwid err！\n");
        return FALSE;
    }
    //wprintf(TEXT("update hwid= [%s]\n"), hwid);

    printf("start UpdateDriver\n");
    if (!UpdateDriverForPlugAndPlayDevices(NULL, hwid, inf, flags, &reboot)) {
        printf("UpdateDriverForPlugAndPlayDevices failed！\n");
        return FALSE;
    }

    if (reboot) {
        printf("A reboot is needed to complete driver install！\n");
    }

    bSuccess = TRUE;
    printf("UpdateDriverForPlugAndPlayDevices ok！\n");

    return bSuccess;
}


void Set_InstallationSources_Directory(wchar_t* szPath)
{
    wprintf(TEXT("Set_InstallationSources_Directory= [%s]\n"), szPath);
    HKEY hKey;

    int len = wcslen(szPath) + 1;

    // 使用RegCreateKey能保证如果Softwaredaheng_directx不存在的话，创建一个。
    if (RegCreateKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Setup", &hKey) == ERROR_SUCCESS) {
        if (RegSetValueEx(hKey, L"Installation Sources", 0, REG_MULTI_SZ, (CONST BYTE*)szPath, 2 * len) == ERROR_SUCCESS) {
            printf("RegSetValue: Installation Sources = %s ", szPath);
        }
    }
    RegCloseKey(hKey);
}
