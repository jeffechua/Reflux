#pragma once
#include <stdexcept>
#include <unordered_map>
#include <stack>

namespace Reflux::Engine::TestUtils {

	template<typename TKey, typename TValue>
	class mapstack {
	private:
		std::unordered_map<TKey, TValue> map;
		std::stack<TKey> stack;
	public:
		mapstack(size_t size);
		void emplace(TKey key, TValue value);
		size_t size();
		TValue& at(TKey key);
		TValue& operator[](TKey key);
		bool contains(TKey key);
		std::pair<TKey, TValue> top();
		void pop();
		void pop_to(size_t size);
		void clear();
	};

	// IMPLEMENTATION

	template<typename TKey, typename TValue>
	mapstack<TKey, TValue>::mapstack(size_t size) : map(), stack() {
		map.reserve(size);
	}

	template<typename TKey, typename TValue>
	void mapstack<TKey, TValue>::mapstack::emplace(TKey key, TValue value) {
		if (map.contains(key)) {
			throw std::runtime_error("Key already present in mapstack.");
		}
		map.emplace(key, value);
		stack.push(key);
	}

	template<typename TKey, typename TValue>
	size_t mapstack<TKey, TValue>::size() {
		return stack.size();
	}

	template<typename TKey, typename TValue>
	TValue& mapstack<TKey, TValue>::at(TKey key) {
		return map[key];
	}

	template<typename TKey, typename TValue>
	TValue& mapstack<TKey, TValue>::operator[](TKey key) {
		return map[key];
	}

	template<typename TKey, typename TValue>
	bool mapstack<TKey, TValue>::contains(TKey key) {
		return map.contains(key);
	}

	template<typename TKey, typename TValue>
	std::pair<TKey, TValue> mapstack<TKey, TValue>::top() {
		return *map.find(stack.top());
	}

	template<typename TKey, typename TValue>
	void mapstack<TKey, TValue>::pop() {
		map.erase(stack.top());
		stack.pop();
	}

	template<typename TKey, typename TValue>
	void mapstack<TKey, TValue>::pop_to(size_t size) {
		while (this->size() > size) {
			pop();
		}
	}

	template<typename TKey, typename TValue>
	void mapstack<TKey, TValue>::clear() {
		map.clear();
		stack = std::stack<TKey>();
	}

}