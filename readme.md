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
这个Demo大概模拟的是一个，生产者(post)-消费者(get)模型。
post会快速产生内容，需要get处理。
如果是常规的单线程，post的内容产生速率，会因为单个get速度而受限。
利用线程池后，空闲的线程会自动拉取任务来处理，post/get变成了异步的操作，
提升了post速率，处理get的线程数也会自动变化，从而提高整体速率，
再通过`channel`，将信息安全的传递
`post >> ch >> get`，来保证两者的同步问题

