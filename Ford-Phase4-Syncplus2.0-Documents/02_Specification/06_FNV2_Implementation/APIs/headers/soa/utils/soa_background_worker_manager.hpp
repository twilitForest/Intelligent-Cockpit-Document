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
 * soa_background_worker_manager.hpp
 *
 *  Created on: Aug 16, 2017
 *      Author: ASERGEE2
 */

#ifndef INCLUDE_SOA_UTILS_SOA_BACKGROUND_WORKER_MANAGER_HPP_
#define INCLUDE_SOA_UTILS_SOA_BACKGROUND_WORKER_MANAGER_HPP_

#include <soa/utils/soa_priority_worker.hpp>
#include <soa/utils/soa_singleton_factory.hpp>

namespace fnv {
namespace soa {
namespace framework {

/**
 * Class container for SOA workers
 */
class SoaBackgroundWorkerManager {
public:
    /**
     * SoaBackgroundWorkerManager singleton implementation.
     * @return SoaBackgroundWorkerManager
     */
    static SoaBackgroundWorkerManager & instance(void);
    explicit SoaBackgroundWorkerManager(const SoaBackgroundWorkerManager &) = delete;
    void  operator=(SoaBackgroundWorkerManager) = delete;
    SoaPriorityWorker & getTimerThread(void);
private:
    friend class SingletonFactory<SoaBackgroundWorkerManager>;
    SoaBackgroundWorkerManager(void);
    SoaPriorityWorker m_timerThread;
};

} /* namespace framework */
} /* namespace soa */
} /* namespace fnv */

#endif /* INCLUDE_SOA_UTILS_SOA_BACKGROUND_WORKER_MANAGER_HPP_ */
