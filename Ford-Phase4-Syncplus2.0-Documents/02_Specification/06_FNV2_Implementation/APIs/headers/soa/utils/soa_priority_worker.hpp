//
// CONFIDENTIAL - FORD MOTOR COMPANY
//
// This is an unpublished work, which is a trade secret, created in
// 2017.  Ford Motor Company owns all rights to this work and intends
// to maintain it in confidence to preserve its trade secret status.
// Ford Motor Company reserves the right to protect this work as an
// unpublished copyrighted work in the event of an inadvertent or
// deliberate unauthorized publication.  Ford Motor Company also
// reserves its rights under the copyright laws to protect this work
// as a published work.  Those having access to this work may not copy
// it, use it, or disclose the information contained in it without
// the written authorization of Ford Motor Company.
//
/*
 * soa_priority_worker.hpp
 *
 *  Created on: Aug 16, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_SOA_UTILS_SOA_PRIORITY_WORKER_HPP_
#define INCLUDE_SOA_UTILS_SOA_PRIORITY_WORKER_HPP_

#include <chrono>
#include <memory>
#include <mutex>
#include <queue>
#include <chrono>
#include <string>
#include <soa/utils/soa_background_worker.hpp>
#include <soa/utils/soa_fifo_worker.hpp>
#include <thread-util/ThreadAnnotations.hpp>

namespace fnv {
namespace soa {
namespace framework {

typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;

/**
 * An SoaTask wrapper.
 * The class implements a task with a time stamp. Time stamp is a time when the task should be executed.
 * Time stamp defines a task position in PriorityTaskQueue. The most recent task is always on top.
 * Time stamps are based on std::chrono::steady_clock.
 * Task time stamp generation: std::chrono::steady_clock::now() + std::chrono::milliseconds(interval_mSec),
 *      where interval_mSec is an "interval_mSec" in mSec provided in the constructor.
 * SoaTimePriorityTaskWrapper can be created as a single execution or a periodic task.
 * Single execution tasks are executed one time.
 *
 * Periodic tasks are periodically executed until SoaTimePriorityTaskWrapper::cancel() method is called.
 * After executing of a periodic task the task time stamp is incremented by std::chrono::milliseconds(m_inteval_mSec)
 *
 * Tasks can be canceled by calling SoaTimePriorityTaskWrapper::cancel() method.
 * Canceled tasks are not executed.
 */
class SoaTimePriorityTaskWrapper {
public:
    /**
     * Shared pointer type for this class
     */
    typedef std::shared_ptr<SoaTimePriorityTaskWrapper> SharedPtr;
    typedef std::weak_ptr<SoaTimePriorityTaskWrapper> WeakPtr;

    class Compare
    {
    public:
        bool operator() (const SoaTimePriorityTaskWrapper::SharedPtr & first, const SoaTimePriorityTaskWrapper::SharedPtr & second) const;
    };

    /**
     * Factory method
     * @param task A task to execute
     * @param interval_mSec An event interval in mSec
     * @param periodic Set true to make the event periodic.
     * @return SoaTimePriorityTaskWrapper::SharedPtr
     */

    static SharedPtr makeShared(const SoaTask::SharedPtr & task, const std::chrono::milliseconds & interval_mSec, bool periodic);
    /**
     * Constructor
     * @param task A task to execute
     * @param interval_mSec An event interval in mSec
     * @param periodic Set true to make the event periodic.
     */
    SoaTimePriorityTaskWrapper(const SoaTask::SharedPtr & task, const std::chrono::milliseconds & interval_mSec, bool periodic);

    /**
     * Returns the time for an ongoing even
     * @return TimePoint
     */
    TimePoint getEventTime() const;
    /**
     * Advances the event time for periodic handlers by time interval provided in the constructor.
     * Next call to getEventTime() will return the advanced time
     */
    void advanceEventTime();
    /**
     * Returns true if the handler should handle periodic events
     * @return bool
     */
    bool periodic() const;

    /**
     * Cancels the task
     */
    void cancel();

    /**
     * Returns true if the task is canceled
     * @return bool
     */
    bool cancelled() const;


    /**
     * Returns an associated task
     * @return SoaThreadTask::SharedPtr
     */
    SoaTask::SharedPtr getTask() const;

private:
    const bool k_periodic;
    const SoaTask::SharedPtr k_timeoutTask;
    std::chrono::milliseconds m_inteval_mSec;
    TimePoint m_eventTime;
    std::atomic<bool> m_cancelled;
    bool m_timedout;
    const char * const k_className;

};

/**
 * Priority SoaTaskQueue implementation.
 * Tasks are placed in the queue based on time stamp priority.
 * Tasks can be pushed in different order but the queue ensures that the most recent task is always on top.
 *
 * waitForNextTask() method of this class returns a task ready to execute i.e. a task with a time stamp <= current time.
 * it iterates a loop:
 *  1. waits for a task to be added to the queue.
 *  2. waits until the top task is ready to execute. If a new task is added during the wait the
 *     loop iterates again.
 * when task is ready to execute waitForNextTask() method pops it from the queue and returns to the thread.
 * If the task is canceled waitForNextTask() method returns nullptr.
 * If the task is periodic waitForNextTask() method updates the task time stamp (SoaTimePriorityTaskWrapper::advanceEventTime())
 * and pushes the task back to the queue.
 */
class PriorityTaskQueue: public SoaTaskQueue {
public:
    /**
     * Default constructor
     */
    PriorityTaskQueue();

    /**
     * Waits for the next task or until the queue is being notified to stop
     * PriorityTaskQueue specific implementation:
     * Waits for the next SoaTimePriorityTaskWrapper.
     * it iterates a loop:
     *  1. waits for a task to be added to the queue.
     *  2. waits until the top task is ready to execute. If a new task is added during the wait the
     *     loop iterates again.
     * @return SoaTask::SharedPtr or a nullptr if the method is interrupted by calling SoaTaskQueue::stop()
     */
    virtual SoaTask::SharedPtr waitForNextTask() override EXCLUDES(m_queueMutex);

    /**
     * Remove all pending tasks from the queue
     * This method is provided for unit tests to allow for shutting down
     * any outstanding and not needed timeout thread that may be lingering
     * after the test ends.
     */
    virtual void purge() override EXCLUDES(m_queueMutex);

    /**
     * Queue size
     * @return size_t
     */
    virtual size_t size() override REQUIRES(m_queueMutex);
    /**
     * Creates and pushes a new SoaTimePriorityTaskWrapper task.
     * The method returns a pointer to newly created SoaTimePriorityTaskWrapper. The pointer can be used to cancel the task (SoaTimePriorityTaskWrapper::cancel()).
     * @param task A task to execute
     * @param interval_mSec A task execution interval.
     * @param periodic True to make the task periodic: the task will be executed regularly with "interval_mSec" interval
     * @return SoaTimePriorityTaskWrapper::SharedPtr
     */
    SoaTimePriorityTaskWrapper::SharedPtr push(const std::shared_ptr<SoaTask> & task, const std::chrono::milliseconds & interval_mSec, bool periodic);

private:
    typedef std::priority_queue <SoaTimePriorityTaskWrapper::SharedPtr, std::vector<SoaTimePriorityTaskWrapper::SharedPtr>, SoaTimePriorityTaskWrapper::Compare > SoaPriorityQueue;
    SoaPriorityQueue m_queue GUARDED_BY(m_queueMutex);
    fnv::soa::framework::SoaTimePriorityTaskWrapper::SharedPtr m_topTask GUARDED_BY(m_queueMutex);
    void pushInternal(const SoaTimePriorityTaskWrapper::SharedPtr & task);
};

/**
 * Time based SoaThread implementation.
 * Schedules tasks execution based on tasks (SoaTimePriorityTaskWrapper) time stamp.
 * Uses PriorityTaskQueue implementation of SoaTaskQueue.
 */
class SoaPriorityWorker: public SoaBackgroundWorker {
public:
    SoaPriorityWorker();
    /**
     * Creates and pushes a new SoaTimePriorityTaskWrapper task.
     * The method returns a pointer to newly created SoaTimePriorityTaskWrapper. The pointer can be used to cancel the task (SoaTimePriorityTaskWrapper::cancel()).
     * @param task A task to execute
     * @param interval_mSec A task execution interval.
     * @param periodic True to make the task periodic: the task will be executed regularly with "interval_mSec" interval
     * @return SoaTimePriorityTaskWrapper::SharedPtr
     */
    SoaTimePriorityTaskWrapper::SharedPtr pushTask(const std::shared_ptr<SoaTask> & task, const std::chrono::milliseconds & interval_mSec, bool periodic);
protected:
    /**
     * Schedules task execution in a separate thread
     * @param task A task
     */
    virtual void executeTask(const SoaTask::SharedPtr & task);
private:
    SoaFifoWorker m_taskThread;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_UTILS_SOA_PRIORITY_WORKER_HPP_ */
