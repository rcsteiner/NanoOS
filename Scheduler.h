// Scheduler.h

#ifndef _SCHEDULER_h
#define _SCHEDULER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

typedef void (*SchedFun)();

const int MAX_QUEUE = 16;
const byte END = 0xff;

struct SchedEntry;

struct SchedEntry
{
public:
	const int32_t V_SHIFT = 24;
	const int32_t V_MASK  = (1<<24)-1;
	
	
	uint32_t       d1;
	uint32_t       d2;

	byte	Next() {return d1>>V_SHIFT;}
	void    Next(byte v) { d1 = (d1 & V_MASK) | (v<<V_SHIFT);}
	
	byte    Prev() {return d2>>V_SHIFT;}
	void    Prev(byte v) { d2 = (d2 & V_MASK) | (v<<V_SHIFT);}

	uint32_t ms() {return d1 & V_MASK;}
	void    ms(uint32_t msDelay) { d1 = (d1 & ~V_MASK) | msDelay; }
	
	SchedFun Fun() {return (SchedFun)(d2 & V_MASK);}
	void     Fun(SchedFun fun) { d2 = (d2 & ~V_MASK | (uint32_t) fun);}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: adjust by delta.  If the delta is > the amount of time left, delta is adjusted. if it is <= then it is set
	//  to zero.
	// </summary>
	// <param name="delta">  The delta to adjust by, this value is updated if delta is > than the time on this entry.</param>
	// <returns>
	//  True if delta will zero out the value.
	// </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Adjust(int32_t& delta);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: subtract s entry from delay time if possible. else subtract delay time from entry
	//  and return true (this is the insertion point)
	// </summary>
	// <param name="delayTime">  The delay Time.</param>
	// <returns>
	//  True if delayTime still has some value left.
	// </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Subtract(int32_t& delayTime);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: Clear this object to its initial state..
	// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clear() { d1 = d2 = (0xff<< V_SHIFT); }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: Set.
	// </summary>
	// <param name="prev">  The previous.</param>
	// <param name="next">  The next.</param>
	// <param name="fun">   The fun.</param>
	// <param name="ms">    The milliseconds.</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Set(byte prev, byte next, SchedFun fun, int32_t ms);

	void Set(SchedFun fun, int32_t msDelay)
	{
		Fun(fun);
		ms(msDelay);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  The Scheduler Class definition.
// </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Scheduler
{
 protected:
	static SchedEntry _queue[MAX_QUEUE];
		   
	static byte _head;
	static byte _count;
	static byte _free;

 public:
	 void init();

	 void Remove(int i);

	 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 // <summary>
	 //  Method: allocate Entry.
	 // </summary>
	 // <returns>
	 //  The integer value of the entry or END if there is no room. (increase the size of queue)
	 // </returns>
	 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 int AllocEntry();

	 void InsertBefore(int atPosition, int newEntry);

	 void InsertAfter(int atPosition, byte newEntry);

	 void Schedule(SchedFun fun, int32_t ms);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: Run scheduler.  If the delta is > the amount of time left, delta is adjusted. if it is <= then it is set
	//  to zero.
	// </summary>
	// <param name="delta">  The delta to adjust by, this value is updated if delta is > than the time on this entry.</param>
	// <returns>
	//  True if still values left on schedule.
	// </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 bool Run(int32_t delta);
};


struct SchedEntry2
{
public:
	uint32_t		ms;
	SchedFun        fun;

	SchedEntry2*	prev;
	SchedEntry2*	next;
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: adjust by delta.  If the delta is > the amount of time left, delta is adjusted. if it is <= then it is set
	//  to zero.
	// </summary>
	// <param name="delta">  The delta to adjust by, this value is updated if delta is > than the time on this entry.</param>
	// <returns>
	//  True if delta will zero out the value.
	// </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Adjust(int32_t& delta);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: subtract s entry from delay time if possible. else subtract delay time from entry
	//  and return true (this is the insertion point)
	// </summary>
	// <param name="delayTime">  The delay Time.</param>
	// <returns>
	//  True if delayTime still has some value left.
	// </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Subtract(int32_t& delayTime);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: Clear this object to its initial state..
	// </summary>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clear() { memset(this,0,sizeof(SchedEntry2)); }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: Set.
	// </summary>
	// <param name="prev">  The previous.</param>
	// <param name="next">  The next.</param>
	// <param name="fun">   The fun.</param>
	// <param name="ms">    The milliseconds.</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Set(SchedEntry2* prev, SchedEntry2* next, SchedFun fun, int32_t ms);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: Set.
	// </summary>
	// <param name="fun">      The fun.</param>
	// <param name="msDelay">  The milliseconds Delay.</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Set(SchedFun fn, int32_t msDelay)
	{
		fun = fn;
		ms = msDelay;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  The Scheduler Class definition.
// </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Scheduler2
{
protected:
	static SchedEntry2 _queue[MAX_QUEUE];

	static SchedEntry2* _head;
	static SchedEntry2* _free;
	static int _count;

public:
	void init();

	void Remove(SchedEntry2* p);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: allocate Entry.
	// </summary>
	// <returns>
	//  The integer value of the entry or END if there is no room. (increase the size of queue)
	// </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SchedEntry2* AllocEntry();

	void InsertBefore(SchedEntry2* atPosition, SchedEntry2* newEntry);

	void InsertAfter(SchedEntry2* atPosition, SchedEntry2* newEntry);

	void Schedule(SchedFun fn, int32_t msDelay);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: Run scheduler.  If the delta is > the amount of time left, delta is adjusted. if it is <= then it is set
	//  to zero.
	// </summary>
	// <param name="delta">  The delta to adjust by, this value is updated if delta is > than the time on this entry.</param>
	// <returns>
	//  True if still values left on schedule.
	// </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Run(int32_t delta);
};



#endif

