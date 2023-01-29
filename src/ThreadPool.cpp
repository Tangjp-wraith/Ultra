#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t size):stop(false) {
  for(size_t i=0;i<size;++i){
    threads.emplace_back(std::thread([this](){ 
      // 定义每个线程的工作函数
      while(true){
        std::function<void()> task;
        // 在这个{}作用域内对std::mutex加锁，出了作用域会自动解锁，不需要调用unlock()
        {
          std::unique_lock<std::mutex> lock(tasks_mtx);
          //等待条件变量，条件为任务队列不为空或线程池停止
          cv.wait(lock,[this](){
            return stop || !tasks.empty();
          });
          //任务队列为空并且线程池停止，退出线程
          if(stop && tasks.empty()) {
            return ;
          }
          //从任务队列头取出一个任务
          task=tasks.front();
          tasks.pop();
        }
        //执行任务
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(tasks_mtx);
    stop=true;
  }
  cv.notify_all();
  for(std::thread &th : threads){
    if(th.joinable()){
      th.join();
    }
  }
}

void ThreadPool::add(std::function<void()> _task) {
  {
    std::unique_lock<std::mutex> lock(tasks_mtx);
    if(stop){
      throw std::runtime_error("TheadPool already stop, can't add task any more");
    }
    tasks.emplace(_task);
  }
  cv.notify_one();
}
