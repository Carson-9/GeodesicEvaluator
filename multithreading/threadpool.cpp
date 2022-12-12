#include "threadpool.hpp"

threadpool* threadpool_init(int n) {
	threadpool* new_threadpool = new threadpool{ new std::thread * [n], n, 0 };
	for (int i = 0; i < n; i++) new_threadpool->threadpool[i] = NULL;
	return new_threadpool;
}

void launch_newThread(threadpool* pool, void(*function)(void*), void* params) {
	if (pool->currentSize == pool->maxSize) {
		printf("Warning, not enough space for a new thread!\n");
		return;
	}
	pool->threadpool[pool->currentSize++] = new std::thread(function, params);
}

void threadpool_join(threadpool* to_join) {
	for (int i = 0; i < to_join->currentSize; i++) {
		to_join->threadpool[i]->join();
		delete to_join->threadpool[i];
	}
	delete[] to_join->threadpool;
}