#include <boost/thread.hpp>
#include <boost/thread/tss.hpp>
#include <boost/bind.hpp>

#include <iostream>

class del{
public:
	int dat;
	del(int i):dat(i){}
	~del(){std::cout << "deleted " << dat<< std::endl;}
};

boost::thread_specific_ptr<del> tls;
void thread(int num){
	tls.reset(new del(num));
}

int main(void){
	tls.reset(new del(0));
	{
		boost::thread a1(boost::bind(&thread, 1));
		boost::thread a2(boost::bind(&thread, 2));
		boost::thread a3(boost::bind(&thread, 3));	
		a1.join();a2.join();a3.join();
	}
	std::cout << "end" << std::endl;
}
