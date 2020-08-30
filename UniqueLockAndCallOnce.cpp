//
//  main.cpp
//  UniqueLock
//
//  Created by Girish Devanga on 30/08/20.
//  Copyright © 2020 Girish Devanga. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>


class LogFile
{
    std::mutex m_mutex;
    std::mutex m_mutex_open;
    std::once_flag m_flag;
    
    std::ofstream f;
public:
    LogFile ()
    {
        // f.open ("logFile.txt"); -> Case 0; lazy initialization
    }
    ~LogFile ()
    {
        f.close();
    }
    void shared_print (std::string s, int i)
    {
        // case 0 : Lazy initilazation
        // open ofstream f only when required.
        
        // old way of doing: double checker
        /*if (!f.is_open())
        {
            std::lock_guard<std::mutex> lockerOpen(m_mutex_open);
            if (!f.is_open())
            {
                f.open ("logFile.txt");
            }
        }*/
        
        // New Way : m_mutex_open not needed
        std::call_once(m_flag, [&]() {f.open("logFile.txt");});
        
        
        // Unique Lock vs Lock Guard
        // Lock Guard : Simple to use, high performance
        // Cant have more controls like unlock()
        // Unique Lock : Little more performance overhead
        // more controls: Unlock(), defer_lock , move()
        
        // case 1:
        /*std::unique_lock<std::mutex> locker(m_mutex);
        std::cout << s << i << std::endl;
        locker.unlock();*/
        
        // some more code
        // locker.lock()
        // critical code
        // locker.unlock()
        
        // case 2: defer locking
        std::unique_lock<std::mutex> locker(m_mutex, std::defer_lock); // Didnt lock
        locker.lock();
        std::cout << s << i << std::endl;
        locker.unlock();
        
        // case 3:
        std::unique_lock<std::mutex> locker2 = std::move (locker);
        locker2.lock();
        // some more code
        locker2.unlock();
        
        
        
    }
};

void func1 (LogFile& logger)
{
    for (int i = 0; i > -100; --i)
    {
        logger.shared_print ("logger: ", i);
    }
}

int main(int argc, const char * argv[]) {
    LogFile logger;
    std::thread t1 (func1, std::ref (logger)); // passing logger as reference.
    
    for (int i = 0; i < 100; ++i)
    {
        logger.shared_print ("main: ", i);
    }
    
    t1.join ();
    
    return 0;
}

