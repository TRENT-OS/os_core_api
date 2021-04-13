
#include "OS_Error.h"

static const char* success_codes[] =
{
    "OS_SUCCESS"
};

static const char* general_error_codes[] =
{
    "OS_ERROR_IN_PROGRESS",
    "OS_ERROR_TIMEOUT",
    "OS_ERROR_IO",
    "OS_ERROR_EXISTS",
    "OS_ERROR_BUFFER_FULL",
    "OS_ERROR_BUFFER_EMPTY",
    "OS_ERROR_NO_DATA",
    "OS_ERROR_NOT_INITIALIZED",
    "OS_ERROR_TRY_AGAIN",
    "OS_ERROR_WOULD_BLOCK",
    "OS_ERROR_OUT_OF_BOUNDS",
    "OS_ERROR_CONNECTION_CLOSED",
    "OS_ERROR_OVERFLOW_DETECTED",
    "OS_ERROR_INSUFFICIENT_SPACE",
    "OS_ERROR_BUFFER_TOO_SMALL",
    "OS_ERROR_ABORTED",
    "OS_ERROR_OPERATION_DENIED",
    "OS_ERROR_ACCESS_DENIED",
    "OS_ERROR_NOT_FOUND",
    "OS_ERROR_INVALID_HANDLE",
    "OS_ERROR_INVALID_NAME",
    "OS_ERROR_INVALID_PARAMETER",
    "OS_ERROR_INVALID_STATE",
    "OS_ERROR_NOT_SUPPORTED",
    "OS_ERROR_NOT_IMPLEMENTED",
    "OS_ERROR_GENERIC"
};

static const char* configuration_server_specific_error_codes[] =
{
    "OS_ERROR_CONFIG_DOMAIN_NOT_FOUND",
    "OS_ERROR_CONFIG_PARAMETER_NOT_FOUND",
    "OS_ERROR_CONFIG_TYPE_MISMATCH"
};

static const char* file_system_specific_error_codes[] =
{
    "OS_ERROR_FS_NO_FREE_HANDLE",
    "OS_ERROR_FS_DELETE_HANDLE",
    "OS_ERROR_FS_NO_DISK",
    "OS_ERROR_FS_INIT",
    "OS_ERROR_FS_REGISTER",
    "OS_ERROR_FS_CREATE_FS",
    "OS_ERROR_FS_FORMAT_FS",
    "OS_ERROR_FS_PARTITION_READ",
    "OS_ERROR_FS_OPEN",
    "OS_ERROR_FS_CLOSE",
    "OS_ERROR_FS_MOUNT",
    "OS_ERROR_FS_UNMOUNT",
    "OS_ERROR_FS_FILE_NOT_FOUND",
    "OS_ERROR_FS_OPERATION_DENIED",
    "OS_ERROR_FS_INSUFFICIENT_STORAGE_CAPACITY",
    "OS_ERROR_FS_STRUCTURE",
    "OS_ERROR_FS_RESOLVE_HANDLE",
    "OS_ERROR_FS_DELETE_RESOLVE_HANDLE",
    "OS_ERROR_FS_LIB",
    "OS_ERROR_FS_DATABUFFER_OVERLOW",
    "OS_ERROR_FS_INVALID_PARTITION_MODE",
    "OS_ERROR_FS_PARTITION_NOT_READY",
    "OS_ERROR_FS_INVALID_FILESYSTEM"
};

static const char* device_specific_error_codes[] =
{
    "OS_ERROR_DEVICE_INVALID",
    "OS_ERROR_DEVICE_NOT_PRESENT",
    "OS_ERROR_DEVICE_BUSY"
};

static const char* network_specific_error_codes[] =
{
    "OS_ERROR_NETWORK_NO_SUPPORT",
    "OS_ERROR_NETWORK_OP_NO_SUPPORT",
    "OS_ERROR_NETWORK_DOWN",
    "OS_ERROR_NETWORK_UNREACHABLE",
    "OS_ERROR_NETWORK_NO_ROUTE",
    "OS_ERROR_NETWORK_PROTO",
    "OS_ERROR_NETWORK_PROTO_NO_SUPPORT",
    "OS_ERROR_NETWORK_PROTO_OPT_NO_SUPPORT",
    "OS_ERROR_NETWORK_ADDR_IN_USE",
    "OS_ERROR_NETWORK_ADDR_NOT_AVAILABLE",
    "OS_ERROR_NETWORK_CONN_RESET",
    "OS_ERROR_NETWORK_CONN_ALREADY_BOUND",
    "OS_ERROR_NETWORK_CONN_NONE",
    "OS_ERROR_NETWORK_CONN_SHUTDOWN",
    "OS_ERROR_NETWORK_CONN_REFUSED",
    "OS_ERROR_NETWORK_HOST_DOWN",
    "OS_ERROR_NETWORK_HOST_UNREACHABLE"
};

const char*
OS_Error_toString(
    OS_Error_t err)
{
    if ((0 <= err) && (err < 1))
    {
        return success_codes[err];
    }
    else if    ((-1 >= err) && (err >= -26))
    {
        return general_error_codes[(-1) - err];
    }
    else if ((-1000 >= err) && (err >= -1002))
    {
        return configuration_server_specific_error_codes[(-1000) - err];
    }
    else if ((-1100 >= err) && (err >= -1122))
    {
        return file_system_specific_error_codes[ (-1100) - err];
    }
    else if ((-1300 >= err) && (err >= -1316))
    {
        return network_specific_error_codes[(-1300) - err];
    }
    else
    {
        return "OS_ERROR_???";
    }
}
