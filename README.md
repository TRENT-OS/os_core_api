# OS Core API

## Overview

The OS Core API is a central repository for the TRENTOS APIs, which includes
the following sub modules:

* CertParser
* ConfigService
* Crypto
* FileSystem
* Keystore
* Logger
* Network
* TLS

Additionally all TRENTOS CAmkES interfaces can be found here, as they are as
well considered the Core API.

### Concepts

Each of the sub modules is standalone, with implementation linked via a separate
library.

See library specific documentation for more details.

### Declaration of API library in CMake

For proper usage, include the `os_core_api` and implementation libraries in the
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

Assuming our CAmkES component wants to use `os_filesystem` API, the following
must be configured:

### Instantiation of API in CMake

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

### Using the API in C

Now one can use `OS_FileSystem.h` as follows:

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
