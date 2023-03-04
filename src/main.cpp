#include <future>
#include <iostream>
#include <random>
#include <chrono>

#include "fmt/core.h"
#include "thp"
#include "channel"

using namespace std::chrono_literals;
using fmt::print;
std::random_device rd; // real random generator seed
std::mt19937 mt(rd()); // random generator

void delay_rnd_time(int time_ms, int offset){
    std::uniform_int_distribution<int> dist(-offset, offset); // distribute function
    auto rnd = std::bind(dist, mt);
    std::this_thread::sleep_for(std::chrono::milliseconds(time_ms + rnd()));
}



int main(int argc, char* argv[]){
    using namespace th_pool;
    using namespace ch;
    thread_pool pool(100);
    channel<int> ch;

    std::uniform_int_distribution<int> dist(0, 1000);
    auto rnd = std::bind(dist, mt);

    auto post = [&ch, &rnd](int round){
        int msg = rnd();
        ch << round << msg;
        delay_rnd_time(150, 50);
        print("Post:{} {}\n",round , msg);
        return msg;
    };
    auto get = [&ch](){
        int msg, round;
        ch >> round >> msg;
        delay_rnd_time(3500, 500);
        print("\tGet :{} {}\n", round, msg);
    };

    for(auto i=0; true; i++){
        auto f1=pool.submit(post, i);
        pool.submit(get);
        if(f1.get() <= 20)
            break;
    }

}
