#include "..\Includes.h"

COM::ParamName PARAM_NAME;
CNvm NV_SHARED_MEM = CNvm(L"IPC_IO", sizeof(IPC_LASER::SharedMem));
CNvm NV_MEMORY = CNvm(L"MEMORY", sizeof(COM::NvMemory));
IPC_LASER::SharedMem& SHARED_MEM = *(IPC_LASER::SharedMem*)NV_SHARED_MEM.GetAddr();
COM::NvMemory& NV = *(COM::NvMemory*)NV_MEMORY.GetAddr();