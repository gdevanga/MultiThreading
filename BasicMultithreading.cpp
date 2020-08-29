//
//  main.cpp
//  BasicThreading
//
//  Created by Girish Devanga on 29/08/20.
//  Copyright © 2020 Girish Devanga. All rights reserved.
//

#include <iostream>
#include <thread>

void ThreadStartPoint(void)
{
    std::cout << "Hello, I am born!" << std::endl;
}

int main(int argc, const char * argv[]) {
    std::thread t1 (ThreadStartPoint);

    // case 1:
    //t1.join(); // main thread will wait until t1 finishes.
    
    // case 2:
    //t1.detach(); // t1 will continue as deamon, main thread may finish.
    
    // case 3:
    //t1.join();
    //t1.detach();
    // SIGABRT, if u call detach now, since t1 is already finished.
    
    // case 4:
    t1.detach();
    t1.join();
    // SIGABRT, if u call after t1 is detached.
    
    return 0;
}
