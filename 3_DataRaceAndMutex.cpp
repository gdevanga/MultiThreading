//
//  main.cpp
//  RaceConditionAndMutex
//
//  Created by Girish Devanga on 29/08/20.
//  Copyright © 2020 Girish Devanga. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>

std::mutex mu;


void func1 ()
{
    for (int i = 0; i > -100; --i)
    {
        std::cout << "func1: " << i << std::endl;
    }
}

void sharedPrinter (std::string s, int i)
{
    mu.lock();
    // ISSUE : what happens if below code fials before unlocks
    // it will remain always as locked.
    std::cout << s << i << std::endl;
    mu.unlock();
}

void sharedPrinter3 (std::string s, int i)
{
    // RAII -> once below code crashes, lock variable is gone, so no more lock
    // Resource Acquisition Is Initialization
    // https://en.cppreference.com/w/cpp/language/raii
    
    std::lock_guard<std::mutex> lock(mu);
    
    // But: what if, this cout is used by some one else
    // so better to stick together mutex and critical resourse in single class
    // thats our approach 4!!
    std::cout << s << i << std::endl;
}


void func2 ()
{
    for (int i = 0; i > -100; --i)
    {
        sharedPrinter ("func2: ", i);
    }
}

void func3 ()
{
    for (int i = 0; i > -100; --i)
    {
        sharedPrinter3 ("func3: ", i);
    }
}


class LogFile
{
    std::mutex m_mutex;
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
        std::lock_guard<std::mutex> lock(m_mutex);
        // no body can access f now, until they clear the lock mechanism!!
        f << s << i << std::endl;
    }
    
    // things to consider:
    // never return f to outside world!!
    // eg : ofstream& getStream()  { return f; }
    //
    
};

void func4 (LogFile& logger)
{
    for (int i = 0; i > -100; --i)
    {
        logger.shared_print ("logger: ", i);
    }
}



int main(int argc, const char * argv[]) {
    
    
    // case 1: without locking - mutex
    // without locking cout
    // here cout is critical resource.
    
    std::thread t1 (func1);
    
    for (int i = 0; i < 100; ++i)
    {
        std::cout << "main: " << i << std::endl;
    }
    
    t1.join ();
    
    // sample output:
    // main: 0
    // main: 22
    // main: func1: 23
    // main: 24
    
    
    // case 2: lets add mutex
    std::thread t2 (func2);
    
    for (int i = 0; i < 100; ++i)
    {
        sharedPrinter ("main: ", i);
    }
    
    t2.join ();
    
    // case 3: lets add mutex - handling lock, unlock with RAII
    std::thread t3 (func3);
    
    for (int i = 0; i < 100; ++i)
    {
        sharedPrinter3 ("main: ", i);
    }
    
    t3.join ();
    
    
    // case 4: lets add mutex - with log class (mutex + cout) in single object
    LogFile logger;
    std::thread t4 (func4, std::ref (logger)); // passing logger as reference.
    
    for (int i = 0; i < 100; ++i)
    {
        logger.shared_print ("main: ", i);
    }
    
    t4.join ();
    
    return 0;
}
