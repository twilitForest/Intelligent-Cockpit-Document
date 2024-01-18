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
 * soa_background_worker.hpp
 *
 *  Created on: Aug 16, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_SOA_UTILS_SOA_BACKGROUND_WORKER_HPP_
#define INCLUDE_SOA_UTILS_SOA_BACKGROUND_WORKER_HPP_

#include <mutex>
#include <atomic>
#include <thread-util/MonotonicCondVar.hpp>
#include <memory>
#include <utility>
#include <string>
#include <type_traits>
#include <functional>
#include <thread-util/ThreadAnnotations.hpp>
#include <soa/utils/soa_frameworks_logging.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Thread task interface class. SoaThread uses SoaThreadTask::executemethod to execute the task.
 */
class SoaTask {
public:
    /**
     * Shared pointer type for this class
     */
    typedef std::shared_ptr<SoaTask> SharedPtr;

    /**
     * Constructor
     * @param name A task name
     */
    explicit SoaTask(const std::string & name);
    SoaTask() = delete;
    virtual ~SoaTask() = default;
    /**
     * Executes the task
     */
    virtual void execute() = 0;
    /**
     * Returns the task name provided in the constructor (name)
     * @return const char *
     */
    const char * getName();
private:
    const std::string k_taskNameContainer; //SoaTask in construct by string, so it needs to store here.
    const char * const k_taskName;
};

/**
 * Basic SoaTask implementation.
 * Allows to create tasks by providing an std::function<void()> method to execute.
 * Can be created using lambdas.
 */
class SoaTaskFunc: public SoaTask {
public:
    /**
     * Creates a shared pointer
     * @param func A function
     * @return SoaTaskFunc::SharedPtr
     */
    static SharedPtr makeShared(const std::function<void()> & func);
    /**
     * Constructor, creates an object with default log tag: SoaTaskFunc
     * @param func A function to execute
     */
    explicit SoaTaskFunc(const std::function<void()> & func);

    /**
     * Executes the task
     */
    void execute() override;
protected:
    /**
     * Constructor
     * @param name A task name
     * @param func A function to execute
     */
    SoaTaskFunc(const std::string & name, const std::function<void()> & func);
private:
    std::function<void()> m_func;
    std::string m_className;
};

/**
 * A task container interface defining base methods needed for SoaThread to pull and execute tasks.
 * Concrete SoaThreadQueue implementations should allow to execute tasks in a desired order.
 * Declares pure virtual method waitForNextTask and some common functionality.
 * Derived classes should implement a method to add tasks
 */
class SoaTaskQueue {
public:
    /**
     * Shared pointer type for this class
     */
    typedef std::shared_ptr<SoaTaskQueue> SharedPtr;

    SoaTaskQueue() = delete;
    /**
     * Constructor
     * @param logTag A log tag
     */
    explicit SoaTaskQueue(const char * logTag): m_stop(false), k_className(logTag) {}

    virtual ~SoaTaskQueue() = default;

    /**
     * Waits for the next task or until the queue is being notified to stop
     * @return SoaTask::SharedPtr or a nullptr if the method is interrupted by calling SoaTaskQueue::stop()
     */
    virtual SoaTask::SharedPtr waitForNextTask() = 0;

    /**
     * Remove all pending tasks from the queue
     */
    virtual void purge() = 0;

    /**
     * Sends a stop notification
     * Unblocks SoaTaskQueue::waitForNextTask() method
     */
    void stop();

    /**
     * Returns true if SoaTaskQueue::stop() was called
     * @return bool
     */
    bool stopped() REQUIRES(m_queueMutex);

    /**
     * Queue size
     * @return size_t
     */
    virtual size_t size() = 0;
protected:
    typedef std::unique_lock<std::mutex> LOCK;
    typedef std::lock_guard<std::mutex> LOCKGuard;
    std::atomic_bool m_stop GUARDED_BY(m_queueMutex);
    std::mutex m_queueMutex;
    fnv::base::MonotonicCondVar m_queueChangeCondVar;
    const char * const k_className;
};

/**
 * Base class allowing executing tasks (SoaTask) in a background thread.
 * The worker executes all the tasks in a single thread in a queue.
 * Provides common functionality:
 *  1. Creating a thread in the constructor.
 *  2. Stop and delete the thread in the destructor.
 *  3. Method to pass to the thread.
 * Aggregates a task queue (SoaThreadQueue), pulls and runs the tasks.
 */
class SoaBackgroundWorker {
public:
    /**
     * DEPRECATED * Constructor for named thread - use the 3 argument version
     * @param name A worker name
     * @param queue A shared pointer to a task queue
     */
    SoaBackgroundWorker(const char *name, const SoaTaskQueue::SharedPtr & queue);

    /**
     * Constructor for named thread
     * @param name A worker name
     * @param queue A shared pointer to a task queue
     * @param shortName A short worker name tag for each log line - will be truncated to 4 characters
     */
    SoaBackgroundWorker(const char *name, const SoaTaskQueue::SharedPtr & queue, const std::string & shortName);
    /**
     * Constructor for unnamed thread
     */
    SoaBackgroundWorker() = delete;
    /**
     * destructor
     */
    virtual ~SoaBackgroundWorker();

    /**
     * Waits for a thread to finish its execution
     */
    void join();

    /**
     * Stops the thread
     */
    void stop();

    /**
     * Clear the thread queue
     */
    void purge();

    /**
     * Return the number of tasks in the queue.
     * @return number of tasks in the task queue, of -1 if indeterminate
     */
    int32_t getQueueSize() { return m_queue->size(); }
    /**
     * Get thread id
     */
    pthread_t getThreadId() const { return m_thread; };
protected:
    char m_shortName[5];
    SoaTaskQueue::SharedPtr m_queue;
    pthread_t m_thread;
    /**
     * Executes a task in the main thread loop
     * Override in derived classes to change functionality
     * @param task A task
     */
    virtual void executeTask(const SoaTask::SharedPtr & task);
    /**
     * scheduler - runs enqueued functions on the thread in order
     */
    void mainLoop();
    /*
     * C-style binding compatible with pthread_create()
     */
    static void * exec(void * instance);
    const char * const k_className;

};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_UTILS_SOA_BACKGROUND_WORKER_HPP_ */
