#pragma once
#include "Model.h"
#include <functional>

template<typename T>
using UpdateFunction = std::function<void(T&, float)>;
template<typename T>
class Agent
{
public:
	Agent(T&);
	Agent(T&, UpdateFunction<T>);
	const void update(float);
private:
	const UpdateFunction<T> updateFunc;
	T& act;
};

template<typename T>
Agent<T>::Agent(T& m, UpdateFunction<T> uf)
	:updateFunc(uf), act(m) {
}

template<typename T>
Agent<T>::Agent(T& m)
	: updateFunc([](T& a, float time) {}), act(m) {
}

template<typename T>
const void Agent<T>::update(float time)
{
	updateFunc(act, time);
}

