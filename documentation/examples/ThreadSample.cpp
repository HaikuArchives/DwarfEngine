#include <list>
#include <queue>

#include "Dwarf.h"
#include "CompositeDirectory.h"

using namespace dwarf;

class MyThread : public thread<int32>
{
public:
	MyThread(const int32 &param): thread<int32>(param, TEXT("")) {}
	~MyThread() {}

private:
	virtual int32 run(const int32 &param)
	{
		logger(TEXT("This is thread #%d, starting up"), param);
		sleep(3);
		logger(TEXT("Thread %d quitting"), param);

		return 0;
	}
};


int32 DwarfMain(Array<String> &commandline)
{
	
	MyThread thread1(1);
	MyThread thread2(2);
	MyThread thread3(3);

	thread1.resume();
	sleep(.3);
	thread2.resume();
	sleep(1);
	thread3.resume();

	logger(TEXT("main thread is now waiting for thread 3 to quit."));
	thread3.wait();
	logger(TEXT("done, quitting the application."));

	return 0;
}
