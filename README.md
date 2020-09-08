# NanoOS
This is an ultra-lightweight operating sysytem featuring a simple task and scheduler.  The initial release of this OS is to support microprocessors and minimum RAM.  The scheduler is designed for millisecond operations.  If sub-millisecond operation is needed, then it is best handled by low-level timing code. (for example bit-banging i/o at >1K).

## Primary Features

### Task
A basic association of a method and the links needed to use it on a delta queue.  
Two types of Tasks are provided for:
* static void Functions *(stores the function reference)*.
* class methods *(requires the storage of the instance and the method reference)*.

### Scheduler
Provides the basic loop implementation and runs the tasks based on a schedule.
* **schedule**(task,timeMilliSeconds)
* **cancel**(task)
* **ready**(task, time=0)   *(makes it ready, normally in interrupt, will be picked up by scheduler)*

# Operation
The basic operation is:
* Create Tasks
* Schedule Tasks (if any)
* Interrupts - make Tasks Ready.


