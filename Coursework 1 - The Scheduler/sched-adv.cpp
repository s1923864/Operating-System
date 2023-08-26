/* SPDX-License-Identifier: MIT */

/*
 * kernel/sched-cfs.cpp
 * 
 * A poor implementation of the Completely Fair Scheduling algorithm, from the Linux kernel.
 * 
 * InfOS
 * Copyright (C) University of Edinburgh 2016.  All Rights Reserved.
 * Tom Spink <tspink@inf.ed.ac.uk>
 */
#include <infos/kernel/sched.h>
#include <infos/kernel/thread.h>
#include <infos/kernel/log.h>
#include <infos/util/list.h>
#include <infos/util/lock.h>

using namespace infos::kernel;
using namespace infos::util;

/**
 * A completely fair scheduling algorithm
 */
class AdvancedScheduler : public SchedulingAlgorithm
{
public:
	/**
	 * Returns the friendly name of the algorithm, for debugging and selection purposes.
	 */
	const char* name() const override { return "adv"; }

    /**
     * Called during scheduler initialisation.
     */
    void init()
    {
        // Meh
    }
	
	/**
	 * Called when a scheduling entity becomes eligible for running.
	 * @param entity
	 */
	void add_to_runqueue(SchedulingEntity& entity) override
	{
	
	
	// priority_check is used to check the priority of entity. 
        // only the entity with priority REALTIME or INTERACTIVE or NORMAL or DAEMON 
        //can be added to  runqueue.
        bool priority_check = entity.priority() ==  SchedulingEntityPriority::REALTIME || 
                              entity.priority() ==  SchedulingEntityPriority::INTERACTIVE || 
                              entity.priority() ==  SchedulingEntityPriority::NORMAL || 
                              entity.priority() ==  SchedulingEntityPriority::DAEMON;
                              
     
        assert(priority_check);
        
        
		UniqueIRQLock l;
		runqueue.enqueue(&entity);
	}

	/**
	 * Called when a scheduling entity is no longer eligible for running.
	 * @param entity
	 */
	void remove_from_runqueue(SchedulingEntity& entity) override
	{
		UniqueIRQLock l;
		runqueue.remove(&entity);
	}

	/**
	 * Called every time a scheduling event occurs, to cause the next eligible entity
	 * to be chosen.  The next eligible entity might actually be the same entity, if
	 * e.g. its timeslice has not expired.
	 */
	SchedulingEntity *pick_next_entity() override
	{	
		if (runqueue.count() == 0) return NULL;
		if (runqueue.count() == 1) return runqueue.first();
		
		SchedulingEntity::EntityRuntime min_runtime = 0;
		SchedulingEntity *min_runtime_entity = NULL;
		SchedulingEntity::EntityRuntime current_runtime = 0;
		SchedulingEntity::EntityRuntime zero = 0;
		for (const auto& entity : runqueue) {
			
	if(entity->priority() == SchedulingEntityPriority::REALTIME){
	current_runtime = entity->cpu_runtime();
	}
	

	else if(entity->priority() == SchedulingEntityPriority::INTERACTIVE){
	current_runtime = entity->cpu_runtime() + entity->cpu_runtime();
	}
        
        
        else if(entity->priority() == SchedulingEntityPriority::NORMAL){
	current_runtime = entity->cpu_runtime() + entity->cpu_runtime() + entity->cpu_runtime();
	}
	
	
	else if(entity->priority() == SchedulingEntityPriority::DAEMON){
	current_runtime = entity->cpu_runtime() + entity->cpu_runtime() + entity->cpu_runtime() + entity->cpu_runtime();
	}
	
	
	                    if(min_runtime_entity == NULL){
	                    min_runtime_entity = entity;
		            min_runtime = current_runtime;
	                    }
	                    else if(current_runtime < min_runtime){
	                    min_runtime_entity = entity;
		            min_runtime = current_runtime;
	                    }
		                      
		}
				
		return min_runtime_entity;
	}
	
private:
	List<SchedulingEntity *> runqueue;
};

/* --- DO NOT CHANGE ANYTHING BELOW THIS LINE --- */

RegisterScheduler(AdvancedScheduler);
