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
 * soa_fifo_thread.hpp
 *
 *  Created on: Aug 18, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_SOA_UTILS_SOA_FIFO_WORKER_HPP_
#define INCLUDE_SOA_UTILS_SOA_FIFO_WORKER_HPP_

#include <queue>
#include <soa/utils/soa_background_worker.hpp>

namespace fnv {
namespace soa {
namespace framework {
/**
 * FIFO SoaThreadQueue implementation. Ensures the task are executed in FIFO order.
 */
class SimpleTaskQueue: public SoaTaskQueue {
public:
	/**
	 * Default constructor
	 */
	SimpleTaskQueue();

	/**
	 * Waits for the next task or until the queue is being notified to stop
	 * @return SoaTask::SharedPtr or a nullptr if the method is interrupted by calling SoaTaskQueue::stop()
	 */
	 SoaTask::SharedPtr waitForNextTask() override EXCLUDES(m_queueMutex);

    /**
     * Remove all pending tasks from the queue
     */
    void purge() override EXCLUDES(m_queueMutex);

    /**
	 * Queue size
	 * @return size_t
	 */
	size_t size() override REQUIRES(m_queueMutex);

	/*
	 * Pushes a new task to the queue
	 * @param task A task
	 */
	void push(const SoaTask::SharedPtr & task);
private:
	std::queue<SoaTask::SharedPtr> m_queue;
};

/**
 * FIFO SoaThread implementation.
 * Aggregates SimpleTaskQueue to ensure the tasks are executed in FIFO order.
 */
class SoaFifoWorker: public SoaBackgroundWorker {
public:
    /**
     * Shared pointer type for this class
     */
    typedef std::shared_ptr<SoaFifoWorker> SharedPtr;
	/**
	 * Constructor
	 */
	SoaFifoWorker();

	/**
	 * Pushes a new task to the queue
	 * @param task A task
	 */
	void push(const SoaTask::SharedPtr & task);
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_UTILS_SOA_FIFO_WORKER_HPP_ */
