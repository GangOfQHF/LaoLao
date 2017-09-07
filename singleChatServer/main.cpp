//#include "mainwindow.h"
//#include <QApplication>
#include "singlechatserver.h"
#include <boost/thread.hpp>
#include <boost/thread/scoped_thread.hpp>

//using boost::thread;

//void myThread(int endpoint_num1,SingleChatServer singlechat)
//{
//    singlechat.analysis(endpoint_num1);
//}

int main()
{
    SingleChatServer *singlechat = new SingleChatServer();
    singlechat->run();
//    boost::scoped_thread<> t1(myThread,6688,singlechat);
//    boost::scoped_thread<> t2(myThread,7788,singlechat);
//    boost::scoped_thread<> t3(myThread,8899,singlechat);
    return 0;
}
