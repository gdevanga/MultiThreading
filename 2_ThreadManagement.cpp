//
//  main.cpp
//  threadMgmnt
//
//  Created by Girish Devanga on 29/08/20.
//  Copyright © 2020 Girish Devanga. All rights reserved.
//

#include <iostream>
#include <thread>

void function1 ()
{
    std::cout << "child thread:" << std::endl;
}

class Functor
{
public:
    void operator() ()
    {
        std::cout << "functor " << std::endl;
    }
};

class Functor2
{
public:
    void operator() (std::string msg)
    {
        msg += " modified by child!";
        std::cout << "functor2 " << msg << std::endl;
        
    }
};

class Functor3
{
public:
    void operator() (std::string& msg)
    {
        msg += " modified by child!";
        std::cout << "functor3 " << msg << std::endl;
        
    }
};

int main(int argc, const char * argv[]) {
    
    // Topic 1 : How to write join()
    std::thread t1 (function1);
    
    // main is doing some work.
    // always enclose in try catch block. since main may crash
    // catch will do the join and forward the exception
    
    try {
        for (int i = 0; i < 100; ++i)
        {
            std::cout << i << " ";
        }
    }
    catch (...)
    {
        t1.join();
        throw;
    }
    
    t1.join();
    
    // Topic 2: Thread can be started using any callable objects -
    // a) Functions
    // b) Lamda fns
    // c) functor
    
    // case 2.1 : calling an functor
    Functor f1;
    std::thread t2 (f1);
    t2.join();
    
    // case 2.2 : calling an functor in single line
    std::thread t3 ( (Functor()) );
    // Note: braces are needed around eg:  (Functor())
    // else compilation error since this will be treated as function declaration
    // eg :  std::thread t3 ( Functor() );
    t3.join();
    
    // case 2.3 : passing an argument via value
    std::string s1 = "s1 string";
    std::thread t4 ( (Functor2()), s1 );
    t4.join();
    std::cout << "unmodified string : " << s1 << std::endl;
    
    
    
    // case 2.4 : passing an argument via reference -> shared memory
    std::string s2 = "s2 string";
    // std::thread t5 ( (Functor3()), s2 ); -> gives compilation issues
    std::thread t5 ( (Functor3()), std::ref (s2) );
    t5.join();
    std::cout << "modified string : " << s2 << std::endl;
    
    
    // case 2.5 : passing an argument via value -> safe way!!
    std::string s3 = "s3 string";
    std::thread t6 ( (Functor2()), std::move (s3) );
    
    // case 2.6 : moving thread object. t6 becomes empty
    std::thread t7 = std::move (t6);
   
    
    // case 2.7 : print this thread id
    std::cout << std::this_thread::get_id() << std::endl;
    
    // case 2.8 : print t7 thread id
    std::cout << t7.get_id() << std::endl;
    
    t7.join();
    
    
    // case 2.9 :
    // oversubscription : more number of threads than available cpu cores.
    // too many context switch is bad.
    
    // to print available cores.
    std::cout << std::thread::hardware_concurrency() << std::endl;
 
    return 0;
}
