#pragma once
#include "Class_Job.h"
#include "Syn_Q.h"

class JobQueue : public SyncQueue<Job>
{
    public:
        bool remove(uint32_t jobid) {
            std::unique_lock<std::mutex> lock(mtx);
            for (uint32_t i = 0; i < queue.size(); i++) {
                if (queue[i].jobid == jobid) {
                    queue.erase(queue.begin()+i);
                    printf("Se deleteo: %hhu\n", jobid);
                    return true;
                }
            }
            return false;
        }

        ESTADO getStatus(uint32_t jobid) {
            std::unique_lock<std::mutex> lock(mtx);
            for (uint32_t i = 0; i < queue.size(); i++) {
                if (queue[i].jobid == jobid){
                    return queue[i].estado;
                }
            }
            return ESTADO::NOT_FOUND;
        }

        int size() const {
            std::unique_lock<std::mutex> lock{mtx};
            return queue.size();
        }

        Job getJobForRunning() {
            std::unique_lock<std::mutex> lck{mtx};        // acquire mutex
            do_wait:
            while(queue.size() == 0 || queue[queue.size()-1].estado != ESTADO::QUEUED)
                cv.wait(lck);                   

            for (uint32_t i = 0; i < queue.size(); i++) {
                if (queue[i].estado == ESTADO::QUEUED) {
                    queue[i].estado = ESTADO::RUNNING;
                    return queue[i];
                }
            }
            goto do_wait;
        }

        bool setJobState(uint32_t jobid, ESTADO st) {
            std::unique_lock<std::mutex> lock(mtx);
            for (uint32_t i = 0; i < queue.size(); i++) {
                if (queue[i].jobid == jobid){
                    queue[i].estado = st;
                    return true;
                }
            }
            return false;
        }
};
