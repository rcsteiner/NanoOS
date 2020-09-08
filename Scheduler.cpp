// 
// 
// 

#include "pch.h"

Task Scheduler::_queue[MAX_QUEUE];
byte Scheduler::_head;
byte Scheduler::_count;
byte Scheduler::_free;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Remove.
// </summary>
// <param name="i">  The i.</param>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::Remove(int i)
{
	if (i == _head)
	{
		_head = _queue[i].Next();
		_queue[_head].Prev(0);
	}
	else
	{
		auto p = _queue[i].Prev();
		auto n = _queue[i].Next();

		if (p != 0)
		{
			_queue[p].Next(n);
		}
		if (n != 0)
		{
			_queue[n].Prev(p);
		}
	}
	_queue[i].Clear();
	--_count;
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: allocate Entry.
// </summary>
// <returns>
//  The integer value.
// </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Scheduler::AllocEntry()
{
	int n = _free;
	if (_free != END)
	{
		_free = _queue[_free].Next();
	}
	else
	{
	//	Serial.println("ERROR - Scheduler queue is too small.");
	}
	return n;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Insert Before.
// </summary>
// <param name="atPosition">  The at Position.</param>
// <param name="newEntry">    The new Entry.</param>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::InsertBefore(int atPosition, int newEntry)
{
	auto p = _queue[atPosition].Prev();
	auto n = _queue[atPosition].Next();
	if (p != END)
	{
		_queue[p].Next(newEntry);
	}

	_queue[newEntry].Prev(p);
	_queue[newEntry].Next(atPosition);
	_queue[atPosition].Prev(newEntry);
	++_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Insert After.
// </summary>
// <param name="atPosition">  The at Position.</param>
// <param name="newEntry">    The new Entry.</param>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::InsertAfter(int atPosition, byte newEntry)
{
	auto p = _queue[atPosition].Prev();
	auto n = _queue[atPosition].Next();
	if (n != END)
	{
		_queue[n].Prev(newEntry);
	}

	_queue[newEntry].Prev(atPosition);
	_queue[newEntry].Next(n);
	_queue[atPosition].Next(newEntry);
	++_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Schedule.
// </summary>
// <param name="fun">  The fun.</param>
// <param name="ms">   The milliseconds.</param>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::Schedule(TaskFun fun, int32_t ms)
{
	int i = _head;

	int n = AllocEntry();

	if (i == END)
	{
		_queue[n].Set(END, _head, fun, ms);
		_head = n;
		return;
	}

	while (ms > 0 && i != END)
	{
		if (_queue[n].Subtract(ms))
		{
			_queue[n].Set(fun, ms);
			InsertBefore(i, n);
			break;
		}
		auto n = _queue[i].Next();
		if (n == END)
		{
			_queue[n].Set(fun, ms);
			InsertAfter(i, n);
			break;
		}
		i = n;
	}

	// not found so its at the end
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Run.
// </summary>
// <param name="delta">  The delta.</param>
// <returns>
//  True if successful
// </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Scheduler::Run(int32_t delta)
{
	int h = _head;

	while (delta >= 0 && h != END)
	{
		if (_queue[h].Adjust(delta))
		{
			_queue[h].Fun();
			Remove(h);
		}
		h = _queue[h].Next();
	}
	return _count != 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: initialize.
// </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler::init()
{
	_head  = END;
	_count = 0;
	_free  = 0;

	memset(_queue, 0, sizeof(_queue));

	for (int i = 0; i < MAX_QUEUE; ++i)
	{
		_queue[i].Prev(i - 1); // -1 on first which is correct.
		_queue[i].Next(i + 1);
	}
	_queue[MAX_QUEUE - 1].Next(END); // fix last entry
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Adjust.
// </summary>
// <param name="delta">  The delta.</param>
// <returns>
//  True if successful
// </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SchedEntry2::Adjust(int32_t& delta)
{
	auto v = ms;
	if (v < delta)
	{
		v = 0;
		delta -= v;
		ms =v;
		return false;
	}

	delta -= v;
	ms = 0;

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Subtract.
// </summary>
// <param name="delayTime">  The delay Time.</param>
// <returns>
//  True if successful
// </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SchedEntry2::Subtract(int32_t& delayTime)
{
	auto v = ms;
	if (delayTime > v)
	{
		delayTime -= v;
		return false;
	}

	ms = (v - delayTime);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Set.
// </summary>
// <param name="prev">  The previous.</param>
// <param name="next">  The next.</param>
// <param name="fun">   The fun.</param>
// <param name="ms">    The milliseconds.</param>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SchedEntry2::Set(SchedEntry2* p, SchedEntry2* n, TaskFun fn, int32_t msDelay)
{
	prev=p;
	next=n;
	fun=fn;
	ms = msDelay;
	
}



SchedEntry2  Scheduler2::_queue[MAX_QUEUE];
SchedEntry2* Scheduler2::_head;
int Scheduler2::_count;
SchedEntry2* Scheduler2::_free;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Remove.
// </summary>
// <param name="i">  The i.</param>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler2::Remove(SchedEntry2* i)
{
	if (i == _head)
	{
		_head =i->next;
		_head->prev=nullptr;
	}
	else
	{
		auto p = i->prev;
		auto n = i->next;

		if (p != 0)
		{
			p->next = (n);
		}
		if (n != 0)
		{
			n->prev = (p);
		}
	}
	i->Clear();
	--_count;
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: allocate Entry.
// </summary>
// <returns>
//  The integer value.
// </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SchedEntry2* Scheduler2::AllocEntry()
{
	auto n = _free;
	if (_free != nullptr)
	{
		_free = _free->next;
	}
	else
	{
	//	Serial.println("ERROR - Scheduler queue is too small.");
	}
	return n;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Insert Before.
// </summary>
// <param name="atPosition">  The at Position.</param>
// <param name="newEntry">    The new Entry.</param>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler2::InsertBefore(SchedEntry2* atPosition, SchedEntry2* newEntry)
{
	auto p =atPosition->prev;
	auto n =atPosition->next;
	if (p != nullptr)
	{
		p->next = (newEntry);
	}

	newEntry->prev =(p);
	newEntry->next = (atPosition);
	atPosition->prev = (newEntry);
	++_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Insert After.
// </summary>
// <param name="atPosition">  The at Position.</param>
// <param name="newEntry">    The new Entry.</param>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler2::InsertAfter(SchedEntry2* atPosition, SchedEntry2* newEntry)
{
	auto p = atPosition->prev;
	auto n = atPosition->next;
	if (n != nullptr)
	{
		n->prev = (newEntry);
	}

	newEntry->prev = (atPosition);
	newEntry->next = (n);
	atPosition->next = (newEntry);
	++_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Schedule.
// </summary>
// <param name="fun">  The fun.</param>
// <param name="ms">   The milliseconds.</param>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler2::Schedule(TaskFun fn, int32_t msDelay)
{
	auto i = _head;
	auto n = AllocEntry();

	if (i == nullptr)
	{
		n->Set(nullptr, _head, fn, msDelay);
		_head = n;
		return;
	}

	while (msDelay > 0 && i != nullptr)
	{
		if (n->Subtract(msDelay))
		{
			n->Set(fn, msDelay);
			InsertBefore(i, n);
			break;
		}
		auto n = i->next;
		if (n == nullptr)
		{
			n->Set(fn, msDelay);
			InsertAfter(i, n);
			break;
		}
		i = n;
	}

	// not found so its at the end
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: Run.
// </summary>
// <param name="delta">  The delta.</param>
// <returns>
//  True if successful
// </returns>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Scheduler2::Run(int32_t delta)
{
	auto h = _head;

	while (delta >= 0 && h != nullptr)
	{
		if (h->Adjust(delta))
		{
			h->fun();
			Remove(h);
		}
		h = h->next;
	}
	return _count != 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// <summary>
//  Method: initialize.
// </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Scheduler2::init()
{
	_head = nullptr;
	_count = 0;
	_free = _queue;

	memset(_queue, 0, sizeof(_queue));

	for (int i = 1; i < MAX_QUEUE; ++i)
	{
		_queue[i-1].next = _queue + (i); // -1 on first which is correct.
	}
	_queue[MAX_QUEUE - 1].next = nullptr; // fix last entry
}
