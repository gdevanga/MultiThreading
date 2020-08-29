//
//  main.cpp
//  Deadlock
//
//  Created by Girish Devanga on 29/08/20.
//  Copyright © 2020 Girish Devanga. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>


class LogFile
{
    std::mutex m_mutex;
    std::mutex m_mutex2;
    std::ofstream f;
public:
    LogFile ()
    {
        f.open ("logFile.txt");
    }
    ~LogFile ()
    {
        f.close();
    }
    void shared_print (std::string s, int i)
    {
        // case 1, 2, 3
        /*std::lock_guard<std::mutex> lock(m_mutex);
        std::lock_guard<std::mutex> lock2(m_mutex2);
        // no body can access f now, until they clear the lock mechanism!!
        std::cout << s << i << std::endl;*/
        
         // case 4: use std::lock -> this will lock all the mutexes passed to it.
        std::lock (m_mutex, m_mutex2); // locks all
        std::lock_guard<std::mutex> lock(m_mutex, std::adopt_lock);
        std::lock_guard<std::mutex> lock2(m_mutex2, std::adopt_lock);
            // no body can access f now, until they clear the lock mechanism!!
        std::cout << s << i << std::endl;
        
    }
    void shared_print2 (std::string s, int i)
    {
        // case 1: DEADLOCK: order in which locking is happenning
        // uncomment the code and execute.
        /*{
            std::lock_guard<std::mutex> lock2(m_mutex2);
            std::lock_guard<std::mutex> lock(m_mutex);
            
            std::cout << s << i << std::endl;
        }*/
        
        // case 2: Same order so no deadlock : PREFERRED WAY!!
        /*{
            std::lock_guard<std::mutex> lock(m_mutex);
            std::lock_guard<std::mutex> lock2(m_mutex2);
            
            std::cout << s << i << std::endl;
        }*/
        
        // case 3: Same order so no deadlock : PREFERRED WAY!!
        /*{
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                // do something - Granular locking
                
                // unlocked here
            }
            {
                std::lock_guard<std::mutex> lock2(m_mutex2);
                // do something - Granular locking
                
                std::cout << s << i << std::endl;
                // unlocked here
            }
        }*/
        
        // case 4: use std::lock -> this will lock all the mutexes passed to it.
        std::lock (m_mutex, m_mutex2); // locks all
        std::lock_guard<std::mutex> lock2(m_mutex2, std::adopt_lock);
        std::lock_guard<std::mutex> lock(m_mutex, std::adopt_lock);
            // no body can access f now, until they clear the lock mechanism!!
        std::cout << s << i << std::endl;
        
        
        
        
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
        logger.shared_print2 ("main: ", i);
    }
    
    t1.join ();
    
    return 0;
}
