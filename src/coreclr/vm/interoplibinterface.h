// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

//
// Interface between the VM and Interop library.
//

#ifndef _INTEROPLIBINTERFACE_H_
#define _INTEROPLIBINTERFACE_H_

#ifdef FEATURE_COMWRAPPERS

namespace InteropLibInterface
{
    // Base definition of the external object context.
    struct ExternalObjectContextBase
    {
        PTR_VOID Identity;
        DWORD SyncBlockIndex;
    };
}

// Native calls for the managed ComWrappers API
class ComWrappersNative
{
public:
    static const INT64 InvalidWrapperId = 0;

public: // Native QCalls for the abstract ComWrappers managed type.
    static void QCALLTYPE GetIUnknownImpl(
        _Out_ void** fpQueryInterface,
        _Out_ void** fpAddRef,
        _Out_ void** fpRelease);

    static BOOL QCALLTYPE TryGetOrCreateComInterfaceForObject(
        _In_ QCall::ObjectHandleOnStack comWrappersImpl,
        _In_ INT64 wrapperId,
        _In_ QCall::ObjectHandleOnStack instance,
        _In_ INT32 flags,
        _Outptr_ void** wrapperRaw);

    static BOOL QCALLTYPE TryGetOrCreateObjectForComInstance(
        _In_ QCall::ObjectHandleOnStack comWrappersImpl,
        _In_ INT64 wrapperId,
        _In_ void* externalComObject,
        _In_opt_ void* innerMaybe,
        _In_ INT32 flags,
        _In_ QCall::ObjectHandleOnStack wrapperMaybe,
        _Inout_ QCall::ObjectHandleOnStack retValue);

public: // Lifetime management for COM Wrappers
    static void DestroyManagedObjectComWrapper(_In_ void* wrapper);
    static void DestroyExternalComObjectContext(_In_ void* context);
    static void MarkExternalComObjectContextCollected(_In_ void* context);

public: // COM activation
    static void MarkWrapperAsComActivated(_In_ IUnknown* wrapperMaybe);

public: // Unwrapping support
    static IUnknown* GetIdentityForObject(_In_ OBJECTREF* objectPROTECTED, _In_ REFIID riid, _Out_ INT64* wrapperId);
    static bool HasManagedObjectComWrapper(_In_ OBJECTREF object, _Out_ bool* isActive);

public: // GC interaction
    static void OnBackgroundGCStarted();
    static void OnBackgroundGCFinished();
};

class GlobalComWrappersForMarshalling
{
public:
    // Native QCall for the ComWrappers managed type to indicate a global instance
    // is registered for marshalling. This should be set if the private static member
    // representing the global instance for marshalling on ComWrappers is non-null.
    static void QCALLTYPE SetGlobalInstanceRegisteredForMarshalling(_In_ INT64 id);

public: // Functions operating on a registered global instance for marshalling
    static bool IsRegisteredInstance(_In_ INT64 id);

    static bool TryGetOrCreateComInterfaceForObject(
        _In_ OBJECTREF instance,
        _Outptr_ void** wrapperRaw);

    static bool TryGetOrCreateObjectForComInstance(
        _In_ IUnknown* externalComObject,
        _In_ INT32 objFromComIPFlags,
        _Out_ OBJECTREF* objRef);
};

class GlobalComWrappersForTrackerSupport
{
public:
    // Native QCall for the ComWrappers managed type to indicate a global instance
    // is registered for tracker support. This should be set if the private static member
    // representing the global instance for tracker support on ComWrappers is non-null.
    static void QCALLTYPE SetGlobalInstanceRegisteredForTrackerSupport(_In_ INT64 id);

public: // Functions operating on a registered global instance for tracker support
    static bool IsRegisteredInstance(_In_ INT64 id);

    static bool TryGetOrCreateComInterfaceForObject(
        _In_ OBJECTREF instance,
        _Outptr_ void** wrapperRaw);

    static bool TryGetOrCreateObjectForComInstance(
        _In_ IUnknown* externalComObject,
        _Out_ OBJECTREF* objRef);
};

#endif // FEATURE_COMWRAPPERS

#ifdef FEATURE_OBJCBRIDGE

class ObjCBridgeNative
{
public:
    // See MsgSendFunction in Bridge.cs
    enum MsgSendFunction
    {
        MsgSendFunction_ObjCMsgSend = 0,
        MsgSendFunction_ObjCMsgSendFpret = 1,
        MsgSendFunction_ObjCMsgSendStret = 2,
        MsgSendFunction_ObjCMsgSendSuper = 3,
        MsgSendFunction_ObjCMsgSendSuperStret = 4,
        Last = MsgSendFunction_ObjCMsgSendSuperStret,
    };

public: // static
    static BOOL QCALLTYPE TrySetGlobalMessageSendCallback(
        _In_ MsgSendFunction msgSendFunction,
        _In_ void* fptr);

    static void QCALLTYPE GetLifetimeMethods(
        _Out_ void** allocImpl,
        _Out_ void** deallocImpl);
};

#endif // FEATURE_OBJCBRIDGE

class Interop
{
public:
    // Notify when GC started
    static void OnGCStarted(_In_ int nCondemnedGeneration);

    // Notify when GC finished
    static void OnGCFinished(_In_ int nCondemnedGeneration);
};

#endif // _INTEROPLIBINTERFACE_H_
