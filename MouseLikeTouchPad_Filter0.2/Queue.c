/*++

Module Name:

    queue.c

Abstract:

    This file contains the queue entry points and callbacks.

Environment:

    User-mode Driver Framework 2

--*/

#include "driver.h"

NTSTATUS
MouseLikeTouchPadFilterQueueInitialize(
    _In_ WDFDEVICE Device
    )
{
    WDFQUEUE queue;
    NTSTATUS status;
    WDF_IO_QUEUE_CONFIG queueConfig;

    //
    // Configure a default queue so that requests that are not
    // configure-fowarded using WdfDeviceConfigureRequestDispatching to goto
    // other queues get dispatched here.
    //
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(
         &queueConfig,
        WdfIoQueueDispatchParallel
        );

    queueConfig.EvtIoDeviceControl = MouseLikeTouchPadFilterEvtIoDeviceControl;
    queueConfig.EvtIoInternalDeviceControl = MouseLikeTouchPadFilterEvtIoInternalDeviceControl;//�������Ըú�������ִ��
    queueConfig.EvtIoStop = MouseLikeTouchPadFilterEvtIoStop;
    queueConfig.EvtIoRead = MouseLikeTouchPadFilterEvtIoRead;
    queueConfig.EvtIoDefault = MouseLikeTouchPadFilterEvtIoDefault;//�������Ըú�������ִ��
    queueConfig.EvtIoWrite = MouseLikeTouchPadFilterEvtIoWrite;//�������Ըú�������ִ��
    queueConfig.EvtIoResume = MouseLikeTouchPadFilterEvtIoResume;//�������Ըú�������ִ��

    status = WdfIoQueueCreate(
                 Device,
                 &queueConfig,
                 WDF_NO_OBJECT_ATTRIBUTES,
                 &queue
                 );

    if(!NT_SUCCESS(status)) {
        return status;
    }

    return status;
}

VOID
MouseLikeTouchPadFilterEvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
    )
{
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    NTSTATUS status = STATUS_SUCCESS;
    WDFDEVICE device = WdfIoQueueGetDevice(Queue);
    DEVICE_CONTEXT* pDevContext = DeviceGetContext(device);
    UNREFERENCED_PARAMETER(pDevContext);

    UNREFERENCED_PARAMETER(status);
    UNREFERENCED_PARAMETER(IoControlCode);
    //Filter_DispatchPassThrough(Request, WdfDeviceGetIoTarget(device));//�������Ըú���ʹ�����Ʋ�����Ч������ִ��ptp����ʧЧ����걨����Ч


    switch (IoControlCode)
    {
    case IOCTL_HID_GET_COLLECTION_INFORMATION://�������Ի�ִ��
        RegDebug(pDevContext, L"IOCTL_HID_GET_COLLECTION_INFORMATION", NULL, IoControlCode); 
        break;

    case IOCTL_HID_GET_COLLECTION_DESCRIPTOR://�������Ի�ִ��
        RegDebug(pDevContext, L"IOCTL_HID_GET_COLLECTION_DESCRIPTOR", NULL, IoControlCode); 
        break;

    case IOCTL_HID_GET_PRODUCT_STRING://�������Ի�ִ��
        break;

    case IOCTL_HID_GET_FEATURE://�������Ի�ִ��
        break;

    default:
        break;
    }

    //������IoControlCode�����²㴦��
    Filter_DispatchPassThrough(Request, WdfDeviceGetIoTarget(device));
    return;
}


VOID
MouseLikeTouchPadFilterEvtIoInternalDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
)
{
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    NTSTATUS status = STATUS_SUCCESS;
    WDFDEVICE device = WdfIoQueueGetDevice(Queue);
    DEVICE_CONTEXT* pDevContext = DeviceGetContext(device);
    UNREFERENCED_PARAMETER(pDevContext);

    UNREFERENCED_PARAMETER(status);
    UNREFERENCED_PARAMETER(IoControlCode);
    //Filter_DispatchPassThrough(Request, WdfDeviceGetIoTarget(device));

    MessageBox(NULL, L"ʧ����ֹ", L"��ֹ", 0);
    WdfRequestComplete(Request, STATUS_SUCCESS);

    return;
}


VOID
MouseLikeTouchPadFilterEvtIoStop(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ ULONG ActionFlags
)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Request);
    UNREFERENCED_PARAMETER(ActionFlags);

    return;
}


VOID
MouseLikeTouchPadFilterEvtIoRead(
    _In_
    WDFQUEUE Queue,
    _In_
    WDFREQUEST Request,
    _In_
    size_t Length
)
{
    UNREFERENCED_PARAMETER(Length);

    NTSTATUS Status = STATUS_SUCCESS;
    WDFDEVICE device = WdfIoQueueGetDevice(Queue);
    DEVICE_CONTEXT* pDevContext = DeviceGetContext(device);

    UNREFERENCED_PARAMETER(pDevContext);

    RegDebug(pDevContext, L"EvtIoRead runtimes", NULL, Status);

    Filter_DispatchPassThrough(Request, WdfDeviceGetIoTarget(device));

};

VOID
MouseLikeTouchPadFilterEvtIoWrite(
    _In_
    WDFQUEUE Queue,
    _In_
    WDFREQUEST Request,
    _In_
    size_t Length
)
{
    //��������MouseLikeTouchPadFilterEvtIoWriteû��ִ�й�
    UNREFERENCED_PARAMETER(Length);
    //MessageBox(NULL, L"ʧ����ֹ", L"��ֹ", 0);

    NTSTATUS Status = STATUS_SUCCESS;
    WDFDEVICE device = WdfIoQueueGetDevice(Queue);
    DEVICE_CONTEXT* pDevContext = DeviceGetContext(device);

    UNREFERENCED_PARAMETER(pDevContext);

    RegDebug(pDevContext, L"EvtIoWrite runtimes", NULL, Status);

    Filter_DispatchPassThrough(Request, WdfDeviceGetIoTarget(device));

};


VOID
MouseLikeTouchPadFilterEvtIoDefault(
    _In_
    WDFQUEUE Queue,
    _In_
    WDFREQUEST Request
)
{
    //��������MouseLikeTouchPadFilterEvtIoDEFAULTû��ִ�й�
    //MessageBox(NULL, L"ʧ����ֹ", L"��ֹ", 0);

    NTSTATUS Status = STATUS_SUCCESS;
    WDFDEVICE device = WdfIoQueueGetDevice(Queue);
    DEVICE_CONTEXT* pDevContext = DeviceGetContext(device);

    UNREFERENCED_PARAMETER(pDevContext);

    RegDebug(pDevContext, L"MouseLikeTouchPadFilterEvtIoDEFAULT runtimes", NULL, Status);

    Filter_DispatchPassThrough(Request, WdfDeviceGetIoTarget(device));

};

VOID
MouseLikeTouchPadFilterEvtIoResume(
    _In_
    WDFQUEUE Queue,
    _In_
    WDFREQUEST Request
)
{
    //��������MouseLikeTouchPadFilterEvtIoResumeû��ִ�й�
    //MessageBox(NULL, L"ʧ����ֹ", L"��ֹ", 0);

    NTSTATUS Status = STATUS_SUCCESS;
    WDFDEVICE device = WdfIoQueueGetDevice(Queue);
    DEVICE_CONTEXT* pDevContext = DeviceGetContext(device);

    UNREFERENCED_PARAMETER(pDevContext);

    RegDebug(pDevContext, L"MouseLikeTouchPadFilterEvtIoResume runtimes", NULL, Status);

    Filter_DispatchPassThrough(Request, WdfDeviceGetIoTarget(device));

};


VOID
Filter_DispatchPassThrough(
    _In_ WDFREQUEST Request,
    _In_ WDFIOTARGET Target
)
{
    //RegDebug(pDevContext, L"Filter_DispatchPassThrough", NULL, 0);

    WDF_REQUEST_SEND_OPTIONS options;
    BOOLEAN ret;
    NTSTATUS status = STATUS_SUCCESS;
    WDFDEVICE device = WdfIoQueueGetDevice(WdfRequestGetIoQueue(Request));
    DEVICE_CONTEXT* pDevContext = DeviceGetContext(device);

    UNREFERENCED_PARAMETER(pDevContext);
    //
    // We are not interested in post processing the IRP so 
    // fire and forget.

    WDF_REQUEST_SEND_OPTIONS_INIT(&options,
        WDF_REQUEST_SEND_OPTION_SEND_AND_FORGET);//WDF_REQUEST_SEND_OPTION_TIMEOUT  //WDF_REQUEST_SEND_OPTION_SEND_AND_FORGET

    //��I/O�����͵��²��豸ǰ������Ӧ�����ݴ���//�������������ʱ�����������п���
    //WdfRequestFormatRequestUsingCurrentType(Request);


    //�� I/O �����͵��²��豸
    ret = WdfRequestSend(Request, Target, &options);//WDF_NO_SEND_OPTIONS
    if (ret == FALSE) {
        status = WdfRequestGetStatus(Request);
        RegDebug(pDevContext, L"WdfRequestSend failed", NULL, status);
        WdfRequestComplete(Request, status);
    }

    RegDebug(pDevContext, L"Filter_DispatchPassThrough WdfRequestSend ok", 0, status);

    return;
}