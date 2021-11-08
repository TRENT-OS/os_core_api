# OS Core API

## Overview

The OS Core API is the central definition of the TRENTOS APIs. It includes the
following APIs:

* CertParser
* ConfigService
* Crypto
* FileSystem
* Keystore
* Logger
* Socket
* TLS

Additionally all TRENTOS CAmkES interfaces can be found here, as they are as
well considered part of the OS Core API.

### Concepts

Each of the APIs is standalone, with their implementation linked via a separate
library.

See library specific documentation for more details.

### Linking of `os_core_api` in CMake

For proper usage, include `os_core_api` and implementation libraries in the
project:

```CMake
target_link_libraries(${PROJECT_NAME}
    INTERFACE
        os_core_api
        <implementationLib1>
        <implementationLib2>
)
```

## Example

Assuming a CAmkES component wants to use the `os_filesystem` API, the following
must be configured:

### Linking the `os_filesystem` API in CMake

```CMake
DeclareCAmkESComponent(
    FileReaderWriter
    SOURCES
        "FileReaderWriter.c"
    C_FLAGS
        ...
    LIBS
        os_core_api
        os_filesystem # The implementation must be linked as well!
)
```

### Using the `os_filesystem` API in C

Now `OS_FileSystem.h` can be used as follows:

```C
#include "OS_FileSystem.h"

static OS_FileSystem_Config_t cfgFs =
{
    .type = OS_FileSystem_Type_FATFS,
    .size = OS_FileSystem_STORAGE_MAX,
    .storage = IF_OS_STORAGE_ASSIGN(
        storage_rpc,
        storage_port),
};

int run()
{
    OS_FileSystem_Handle_t hFs;

    OS_Error_t err = OS_FileSystem_init(&hFs, &cfgFs);
    if (OS_SUCCESS != err)
    {
        Debug_LOG_ERROR("OS_FileSystem_init failed with error code %d!", err);
        return err;
    }
}
```
