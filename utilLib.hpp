#pragma once


template <typename T>
struct linkedList{
	T item;
	linkedList* next;
};

template <typename T>
void linkedListInsert(linkedList<T>* l);

template <typename T>
void linkedListGetIndex(linkedList<T>* l, T e);

template <typename T>
void linkedListDelete(linkedList<T>* l, T e);

