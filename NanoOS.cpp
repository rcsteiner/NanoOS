#include "pch.h"

#include <cstdio>


_API NanoOS OS;


void NanoOS::Test()
{
	Delegate d;

	d.Bind<NanoOS, &NanoOS::TestDelegate>(&OS);
	d.Invoke();

}


void NanoOS::TestDelegate()
{
	 
}