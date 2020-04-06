#include <log/Logger.h>
#include <iostream>

using namespace std;

int main()
{
   cout << "Little Bee Socket System Test" << endl;
   LOG(WARNING) << "Logger WARNING";
   LOG(TRACE) << "Logger TRACE";
   LOG(DEBUG) << "Logger DEBUG";
   LOG(ERROR) << "Logger ERROR";
}
