#pragma once


typedef void (*TaskFun)();
typedef uint8_t byte;

struct Task : public Delegate
{
public:


	uint32_t	   ms;
	uint16_t       next;
	uint16_t       prev;

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
	void Clear() {ms=0; next=prev=0;}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: Set.
	// </summary>
	// <param name="prev">  The previous.</param>
	// <param name="next">  The next.</param>
	// <param name="fun">   The fun.</param>
	// <param name="ms">    The milliseconds.</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Set(byte prev, byte next, TaskFun fun, int32_t ms);

	void Set(TaskFun fun, int32_t msDelay)
	{
		Bind<TaskFun>(fun);
		ms(msDelay);
	}
};

struct TaskOld : public Delegate
{
public:
	const int32_t V_SHIFT = 24;
	const int32_t V_MASK = (1 << 24) - 1;


	uint32_t       d1 = 0;
	uint32_t       d2 = 0;

	byte	Next() { return d1 >> V_SHIFT; }
	void    Next(byte v) { d1 = (d1 & V_MASK) | (v << V_SHIFT); }

	byte    Prev() { return d2 >> V_SHIFT; }
	void    Prev(byte v) { d2 = (d2 & V_MASK) | (v << V_SHIFT); }

	uint32_t ms() { return d1 & V_MASK; }
	void    ms(uint32_t msDelay) { d1 = (d1 & ~V_MASK) | msDelay; }

	TaskFun Fun() { return (TaskFun)(d2 & V_MASK); }
	void     Fun(TaskFun fun) { d2 = (d2 & ~V_MASK | (uint32_t)fun); }


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
	void Clear() { d1 = d2 = (0xff << V_SHIFT); }

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// <summary>
	//  Method: Set.
	// </summary>
	// <param name="prev">  The previous.</param>
	// <param name="next">  The next.</param>
	// <param name="fun">   The fun.</param>
	// <param name="ms">    The milliseconds.</param>
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Set(byte prev, byte next, TaskFun fun, int32_t ms);

	void Set(TaskFun fun, int32_t msDelay)
	{
		Fun(fun);
		ms(msDelay);
	}
};


//
//struct Task : public TaskBase
//{
//	
//};
//
//struct TaskMember : public TaskBase
//{
//	
//};


