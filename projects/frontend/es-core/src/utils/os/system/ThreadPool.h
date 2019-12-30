#pragma once

#include <vector>
#include <sys/sysinfo.h>
#include <utils/Log.h>
#include "Thread.h"
#include "Mutex.h"
#include "IThreadPoolWorkerInterface.h"

template<class FeedObject, class ResultObject> class ThreadPool
{
  private:
    /*!
     * @brief Indexed result (e.g: Result + injection index)
     */
    struct IndexedResult
    {
      public:
        int          Index;  //!< Index
        ResultObject Result; //!< Result object

        /*!
         * @brief Constructor
         * @param index Index
         * @param result Result
         */
        IndexedResult(int index, ResultObject result)
          : Index(index),
            Result(result)
        {
        }
    };

    /*!
     * @brief Indexed feed (e.g: Feed + injection index)
     */
    struct IndexedFeed
    {
      public:
        int        Index;    //!< Index
        int        Priority; //!< Priority - Lowest priority are processed first
        FeedObject Feed;     //!< Source object

        /*!
         * @brief Constructor
         * @param index Index
         * @param feed Result
         */
        IndexedFeed(int index, FeedObject feed, int priority = 0)
          : Index(index),
            Priority(priority),
            Feed(feed)
        {
        }

        IndexedFeed()
          : Index(0),
            Priority(0),
            Feed()
        {
        }
    };

    /*!
     * @brief Real worker threads
     */
    class WorkerThread: public Thread
    {
      private:
        //! Signal to start new jobs
        Mutex       mSignal;
        //! Manager
        ThreadPool& mParent;

      public:
        explicit WorkerThread(ThreadPool& parent)
          : mParent(parent)
        {
        }

        void Run() override;

        /*!
         * @brief Set the signal to wxake up the thread
         */
        void Signal() { mSignal.Signal(); }
    };

    //! Name
    std::string                mThreadPoolName;
    //! Worker threads
    std::vector<WorkerThread*> mThreads;
    //! Queue of waiting jobs
    std::vector<IndexedFeed>   mQueue;
    //! Queue of results
    std::vector<IndexedResult> mResults;
    //! Queues protection
    Mutex                      mStackMutex;
    //! Interface
    IThreadPoolWorkerInterface<FeedObject, ResultObject>* mInterface;
    //! Maximum workers
    int                        mWorkers;
    //! Global index (also total queue-ed object count)
    int                        mIndex;
    //! Total completed
    volatile int               mTotalCompleted;
    //! Tick duration
    int                        mTickDuration;
    //! Permanent workers
    volatile bool              mPermanent;

    /*!
     * @brief Pop next feed object
     * @return Feed object or null if the queue is empty
     */
    bool PopFeed(IndexedFeed& result)
    {
      mStackMutex.Lock();
      bool ok = !mQueue.empty();
      if (ok)
      {
        result = mQueue.back();
        mQueue.pop_back();
        if (mQueue.empty())
          for (auto& thread : mThreads)
            thread->Signal();
      }
      mStackMutex.UnLock();
      return ok;
    }

    /*!
     * @brief Push resulty object
     * @param result Result object
     */
    void PushResult(IndexedResult& result)
    {
      mStackMutex.Lock();
      mResults.push_back(result);
      mStackMutex.UnLock();
    }

  public:
    /*!
     * @brief Constructor
     * @param maxthreads MAximum thread allocated to this thread pool.
     * Using 0 set the actual value to ncpu * 2
     * @param parmanent if set to True, all workers do no die after job completion.
     * They wait for next queued job instead
     */
    ThreadPool(IThreadPoolWorkerInterface<FeedObject, ResultObject>* interface, const std::string& name, int maxthreads, bool permanent, int tickduration = 0)
      : mThreadPoolName(name),
        mInterface(interface),
        mWorkers(maxthreads),
        mIndex(0),
        mTotalCompleted(0),
        mTickDuration(tickduration),
        mPermanent(permanent)
    {
      if (maxthreads <= 0)
        mWorkers = get_nprocs_conf() * (maxthreads == 0 ? 1 : -maxthreads);
      LOG(LogDebug) << "Creating new threadpool '" << name << "' using " << mWorkers << " workers " << (permanent ? "permanently" : "one-shoot");
    }

    ~ThreadPool()
    {
      mPermanent = false;
      for(auto& thread : mThreads)
      {
        thread->Stop();
        thread->Signal();
      }

      for (auto& thread : mThreads)
        thread->Join();

      for(auto& thread : mThreads)
        delete thread;

      mThreads.clear();
    }

    int PendingJobs()
    {
      mStackMutex.Lock();
      int count = mQueue.size();
      mStackMutex.UnLock();
      return count;
    }

    /*!
     * @brief Populate working queue with feed objects
     * @param feed Feed object
     * @param Priority: higher priorities are executed first
     */
    void PushFeed(FeedObject feed, int priority = 0)
    {
      mStackMutex.Lock();
      mQueue.push_back(IndexedFeed(mIndex++, feed, priority));
      mStackMutex.UnLock();
      for(auto& thread : mThreads)
        thread->Signal();
    }

    /*!
     * @brief Pop result object
     * @param result Result object, filled in only if the method returns true
     * @return True if there is still a result object to fill result with, false otherwise
     */
    bool PopResult(ResultObject& result, int& index)
    {
      mStackMutex.Lock();
      bool ok = !mResults.empty();
      if (ok)
      {
        result = mResults.back().Result;
        index  = mResults.back().Index;
        mResults.pop_back();
      }
      mStackMutex.UnLock();
      return ok;
    }

    void QuickSortAscending(std::vector<IndexedFeed>& items, int low, int high)
    {
      int Low = low, High = high;
      const IndexedFeed& pivot = items[(Low + High) >> 1];
      do
      {
        while(items[Low].Priority  < pivot.Priority) Low++;
        while(items[High].Priority > pivot.Priority) High--;
        if (Low <= High)
        {
          IndexedFeed Tmp = items[Low]; items[Low] = items[High]; items[High] = Tmp;
          Low++; High--;
        }
      }while(Low <= High);
      if (High > low) QuickSortAscending(items, low, High);
      if (Low < high) QuickSortAscending(items, Low, high);
    }

    /*!
     * @brief Run the current thread pool
     * @param async Do not wait for end of all thread, exit immediately.
     * if async is used, WaitForCompletion must be used to ensure all worker thread have finished.
     */
    void Run(bool async);

    /*!
     * @brief Wait for completion of all worker threads
     * If the threadpool is permanent, wait indefinitely
     */
    void WaitForCompletion()
    {
      int completed = 0;
      if (mTickDuration > 0)
        for(;;)
        {
          usleep(mTickDuration * 1000); // 20ms
          if (completed != mTotalCompleted)
          {
            mInterface->ThreadPoolTick(mTotalCompleted, mIndex);
            completed = mTotalCompleted;
          }
          if (!mPermanent)
            if ((int)mResults.size() == mIndex) break;
        }

      // Wait for thread to die
      for (auto& thread : mThreads)
        thread->Join();
    }

    /*!
     * @brief Get the worker count
     * @return Worker count
     */
    int WorkerCount() const { return mWorkers; }

    /*!
     * @brief Tell if the threadpool is parmanent or not
     * @return True if it is permanent, false otherwise
     */
    bool IsPermanent() const { return mPermanent; }
};

template<class FeedObject, class ResultObject>
void ThreadPool<FeedObject, ResultObject>::Run(bool async)
{
  // Sort by priority
  if (mQueue.size() > 1)
    QuickSortAscending(mQueue, 0, mQueue.size() - 1);

  // Create and run threads
  for(int i = mWorkers; --i >= 0; )
  {
    WorkerThread* worker = new WorkerThread(*this);
    std::string name = mThreadPoolName;
    name += '#';
    name += std::to_string(i);
    mThreads.push_back(worker);
    worker->Start(name);
  }

  // If not async wait for all job to be completed
  if (!async)
    WaitForCompletion();
}

template<class FeedObject, class ResultObject>
void ThreadPool<FeedObject, ResultObject>::WorkerThread::Run()
{
  int loop = -1;
  while(IsRunning())
  {
    // Wait start signal
    if (++loop != 0)
      mSignal.WaitSignal();

    // Get feed
    IndexedFeed feed;
    while(mParent.PopFeed(feed))
      if (mParent.mInterface != nullptr)
      {
        // Start job
        mParent.mInterface->ThreadPoolJobStart(feed.Feed);

        // Run job
        IndexedResult result(feed.Index, mParent.mInterface->ThreadPoolRunJob(feed.Feed));
        mParent.mTotalCompleted++;
        mParent.PushResult(result);

        // Stop job
        mParent.mInterface->ThreadPoolJobCompleted(feed.Feed, mParent.PendingJobs());
      }

    // Not permanent?
    if (!mParent.mPermanent)
      break;
  }
}
