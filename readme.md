# thread pool and channel

## src structure
- tool: 用mutex封装的`safe_queue`
- channel: 在`safe_queue`基础上，封装了输入输出，在生产者消费者模型中，可以用于处理同步问题
- thp: 线程池，内含`safe_queue`，执行器，以及`submit`函数
