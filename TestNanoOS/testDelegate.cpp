#include "pch.h"

class MyClass 
{
public:

	int Test=0;
	
	void SimpleFunction() { Test =1;}
	
};

int Test2=100;

void FreeFunction() {Test2=20;}


TEST(SimpleClass, TestCall)
{
	MyClass c;
	Delegate d;

	d.Bind<MyClass,&MyClass::SimpleFunction>(&c);

	EXPECT_EQ(0,c.Test);
	d.Invoke();
  EXPECT_EQ(1, c.Test);
}


TEST(FreeFunction, TestCall)
{
	Delegate d;

	d.Bind<&FreeFunction>();

	EXPECT_EQ(100, Test2);
	d.Invoke();
	EXPECT_EQ(20, Test2);
}

