#pragma once

#include <cstdint>

namespace termite
{
    /// Job Callback function 
    /// @param jobIndex if you dispatch N jobs, this parameter will be between (0 ~ N-1)
    /// @param userParam user data that is passed to dispatch
    typedef void(*JobCallback)(int jobIndex, void* userParam);

    struct JobPriority
    {
        enum Enum
        {
            High = 0,
            Normal,
            Low,
            Count
        };
    };

    struct JobDesc
    {
        JobCallback callback;           /// Job Callback function, any work should be done in this function
        JobPriority::Enum priority;     /// Job Priority, Higher priority jobs gets done sooner
        void* userParam;                /// User data, user should keep the data valid until job is complete

        JobDesc()
        {
            callback = nullptr;
            priority = JobPriority::Normal;
            userParam = nullptr;
        }

        explicit JobDesc(JobCallback _callback, void* _userParam = nullptr, JobPriority::Enum _priority = JobPriority::Normal)
        {
            callback = _callback;
            userParam = _userParam;
            priority = _priority;
        }
    };

    typedef volatile int32_t JobCounter;
    typedef JobCounter* JobHandle;
    
    /// Initialize job dispatcher, must call this function before any dispatch
    /// @param maxSmallFibers Sets the maximum number of small-stack fibers (workers) that can be queued, default is 128
    /// @param smallFiberStackSize Small fibers stack size in bytes, default is 64kb
    /// @param maxBigFibers Sets the maximum number of small-stack fibers (workers) that can be queued, default is 32
    /// @param smallFiberStackSize Big fibers stack size in bytes, default is 512kb
    /// @param numWorkerThreads Number of worker threads in thread-pool, if you provide UINT8_MAX, the job system will initialize (NumCpuCores-1) threads
    /// @param lockThreadsToCores Locks initialized thread pool to each core, Not available on all platforms. this option is not recommended unless you are on a console
    bool initJobDispatcher(uint16_t maxSmallFibers = 0, uint32_t smallFiberStackSize = 0,
                           uint16_t maxBigFibers = 0, uint32_t bigFiberStackSize = 0,
                           uint8_t numWorkerThreads = UINT8_MAX, bool lockThreadsToCores = false);

    /// Shuts down job dispatcher
    void shutdownJobDispatcher();

    /// Dispatch Small stack jobs, your stack size should not exceed 'smallFiberStackSize' 
    /// @return handle to created jobs that you can wait on later calls
    JobHandle dispatchSmallJobs(const JobDesc* jobs, uint16_t numJobs);

    /// Dispatch Big stack jobs
    /// @return handle to created jobs that you can wait on later calls
    JobHandle dispatchBigJobs(const JobDesc* jobs, uint16_t numJobs);

    /// Wait on dispatched jobs until they are complete
    void waitJobs(JobHandle handle);

    /// Returns number of worker threads in the thread pool
	uint8_t getNumWorkerThreads();    
} // namespace termite


