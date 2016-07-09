#pragma once

#include "bx/allocator.h"

#ifndef T_THREAD_SAFE
#  define T_THREAD_SAFE
#endif

#ifdef termite_SHARED_LIB
#ifdef termite_EXPORTS
#   if BX_COMPILER_MSVC
#       define TERMITE_API extern "C" __declspec(dllexport) 
#   else
#       define TERMITE_API extern "C" __attribute__ ((visibility("default")))
#   endif
#else
#   if BX_COMPILER_MSVC
#       define TERMITE_API extern "C" __declspec(dllimport)
#   else
#       define TERMITE_API extern "C" __attribute__ ((visibility("default")))
#   endif
#endif
#else
#   define TERMITE_API extern "C"
#endif

namespace termite
{
    typedef void(*JobCallback)(int jobIndex, void* userParam);

    enum class JobPriority : int
    {
        High = 0,
        Normal,
        Low,
        Count
    };

    struct jobDesc
    {
        JobCallback callback;
        JobPriority priority;
        void* userParam;

        explicit jobDesc(JobCallback _callback, void* _userParam = nullptr, JobPriority _priority = JobPriority::Normal)
        {
            callback = _callback;
            userParam = _userParam;
            priority = _priority;
        }
    };

    typedef volatile int32_t JobCounter;
    typedef JobCounter* JobHandle;
    
    TERMITE_API JobHandle dispatchSmallJobs(const jobDesc* jobs, uint16_t numJobs) T_THREAD_SAFE;
    TERMITE_API JobHandle dispatchBigJobs(const jobDesc* jobs, uint16_t numJobs) T_THREAD_SAFE;
    TERMITE_API void waitJobs(JobHandle handle) T_THREAD_SAFE;

    int initJobDispatcher(bx::AllocatorI* alloc, 
                          uint16_t maxSmallFibers = 0, uint32_t smallFiberStackSize = 0, 
                          uint16_t maxBigFibers = 0, uint32_t bigFiberStackSize = 0,
                          bool lockThreadsToCores = true, uint8_t numWorkerThreads = UINT8_MAX);
    void shutdownJobDispatcher();
    
} // namespace termite


