# Thread pool and channel

## Project structure
```shell
├── CMakeLists.txt
├── LICENSE
├── readme.md
└── src
    ├── channel     # 在`safe_queue`基础上，封装了输入输出，在生产者消费者模型中，可以用于处理同步问题
    ├── main.cpp    # demo
    ├── thp         # 线程池，内含`safe_queue`，执行器，以及`submit`函数
    └── tool        # 用mutex封装的`safe_queue`
```

----

## Demo
```cpp
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
```


