#pragma once
#include <thread>

typedef struct {
	std::thread** threadpool;
	int maxSize;
	int currentSize;
} threadpool;

threadpool* threadpool_init(int n);
void threadpool_join(threadpool* to_join);
void launch_newThread(threadpool* pool, void(*function)(void*), void* params);