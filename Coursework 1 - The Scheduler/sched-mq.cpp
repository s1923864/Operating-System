/*
 * The Priority Task Scheduler
 * SKELETON IMPLEMENTATION TO BE FILLED IN FOR TASK 1
 */

#include <infos/kernel/sched.h>
#include <infos/kernel/thread.h>
#include <infos/kernel/log.h>
#include <infos/util/list.h>
#include <infos/util/lock.h>

using namespace infos::kernel;
using namespace infos::util;

/**
 * A Multiple Queue priority scheduling algorithm
 */
class MultipleQueuePriorityScheduler : public SchedulingAlgorithm
{
public:
    /**
     * Returns the friendly name of the algorithm, for debugging and selection purposes.
     */
    const char* name() const override { return "mq"; }

    /**
     * Called during scheduler initialisation.
     */
    void init()
    {
        // TODO: Implement me!
    }

    /**
     * Called when a scheduling entity becomes eligible for running.
     * @param entity
     */
     
    
    void add_to_runqueue(SchedulingEntity& entity) override
    {
        // TODO: Implement me!
        
        
        // priority_check is used to check the priority of entity. 
        // only the entity with priority REALTIME or INTERACTIVE or NORMAL or DAEMON can be added to runqueue.
        bool priority_check = entity.priority() ==  SchedulingEntityPriority::REALTIME || 
                              entity.priority() ==  SchedulingEntityPriority::INTERACTIVE || 
                              entity.priority() ==  SchedulingEntityPriority::NORMAL || 
                              entity.priority() ==  SchedulingEntityPriority::DAEMON;
                              
     
        assert(priority_check);
        
        
        UniqueIRQLock l;
        
        // runqueue is divided to 4 queues :
        // 1. Realtime_Queue (store the entity with the priority of REALTIME).
        // 2. Interactive_Queue (store the entity with the priority of INTERACTIVE).
        // 3. Normal_Queue(store the entity with the priority of NORMAL).
        // 4. Daemon_Queue (store the entity with the priority of DAEMON).
        
        
        // add entity to corresponding queue. 
	if(entity.priority() == SchedulingEntityPriority::REALTIME){
	Realtime_Queue.enqueue(&entity);
	}
	

	else if(entity.priority() == SchedulingEntityPriority::INTERACTIVE){
	Interactive_Queue.enqueue(&entity);
	}
        
        
        else if(entity.priority() == SchedulingEntityPriority::NORMAL){
	Normal_Queue.enqueue(&entity);
	}
	
	
	else if(entity.priority() == SchedulingEntityPriority::DAEMON){
	Daemon_Queue.enqueue(&entity);
	}
	
	
    }
    
    
    
    

    /**
     * Called when a scheduling entity is no longer eligible for running.
     * @param entity
     */
    void remove_from_runqueue(SchedulingEntity& entity) override
    {
        // TODO: Implement me!
        
        UniqueIRQLock l;
        
        
        // remove entity in corresponding queue.
	if(entity.priority() == SchedulingEntityPriority::REALTIME){

	Realtime_Queue.remove(&entity);

	
	}
	
	else if(entity.priority() == SchedulingEntityPriority::INTERACTIVE){
	
	Interactive_Queue.remove(&entity);

	}
        
        
        else if(entity.priority() == SchedulingEntityPriority::NORMAL){
       
	Normal_Queue.remove(&entity);
	
	}
	
	
	else if(entity.priority() == SchedulingEntityPriority::DAEMON){
        
	Daemon_Queue.remove(&entity);

    }
    
    }

    /**
     * Called every time a scheduling event occurs, to cause the next eligible entity
     * to be chosen.  The next eligible entity might actually be the same entity, if
     * e.g. its timeslice has not expired.
     */
    SchedulingEntity *pick_next_entity() override
    {
        // TODO: Implement me!
        
        // pick the current entity with highest priority in order
        if(Realtime_Queue.count() != 0){
        Realtime_Queue.enqueue(Realtime_Queue.first());  // round-robin fashion (pick entity of the same queue in order).
        return Realtime_Queue.dequeue();
        }
        
        else if(Interactive_Queue.count() != 0){
        Interactive_Queue.enqueue(Interactive_Queue.first());
        return Interactive_Queue.dequeue();
        }
        
        else if(Normal_Queue.count() != 0){
        Normal_Queue.enqueue(Normal_Queue.first());
        return Normal_Queue.dequeue();
        }
        
        else if(Daemon_Queue.count() != 0){
        Daemon_Queue.enqueue(Daemon_Queue.first());
        return Daemon_Queue.dequeue();
        }
        
        else{
        return NULL; // if all queues are empty, return NULL.
        }
        
    }
    
    
    

private :

List<SchedulingEntity *> Realtime_Queue = {};
List<SchedulingEntity *> Interactive_Queue = {};
List<SchedulingEntity *> Normal_Queue = {};
List<SchedulingEntity *> Daemon_Queue = {};




};



/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(MultipleQueuePriorityScheduler);
