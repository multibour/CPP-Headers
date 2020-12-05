#pragma once
#include <list>
#include <unordered_map>
#include <stdexcept>


template <typename KeyType, typename ValueType, typename Hash = std::hash<KeyType>>
class lru_cache_map {
	typedef std::pair<KeyType, ValueType> KeyValuePairType;
	typedef std::list<KeyValuePairType> CacheBufferType;
	typedef typename CacheBufferType::iterator CacheBufferIteratorType;

protected:
	std::size_t maxSize;
	std::size_t currentSize;
	CacheBufferType cache;
	std::unordered_map<KeyType, CacheBufferIteratorType, Hash> cacheMap;

public:
	lru_cache_map(std::size_t size) : maxSize(size), currentSize(0) {
		if (maxSize == 0)
			throw std::invalid_argument("Maximum size cannot be zero.");
	}

	void insert(const KeyType& key, const ValueType& value) {
		if (contains(key)) {
			cacheMap.at(key)->second = value;
			mention(key);
			return;
		}

		cache.push_front(std::make_pair(key, value));
		++currentSize;

		if (currentSize > maxSize) {
			cacheMap.erase(cache.back().first);
			cache.pop_back();
			--currentSize;
		}

		cacheMap.insert({key, cache.begin()});
	}	

	ValueType& access(const KeyType& key) {
		auto it = cacheMap.at(key);
		mention(key);

		return it->second;
	}

	ValueType& operator[](const KeyType& key) {
		return access(key);
	}

	void remove(const KeyType& key) {
		cache.erase(cacheMap.at(key));
		cacheMap.erase(key);
		--currentSize;
	}

	const KeyValuePairType& getMostUsed() {
		return cache.front();
	}

	const KeyValuePairType& getLeastUsed() {
		return cache.back();
	}

	// updates the value with given key as the most recently viewed
	bool mention(const KeyType& key) {
		if (!contains(key))
			return false;

		cache.splice(cache.begin(), cache, cacheMap.at(key));

		return true;
	}

	bool contains(const KeyType& key) {
		return cacheMap.find(key) != cacheMap.end();
	}

	bool empty() {
		return cache.empty();
	}

	void clear() {
		cacheMap.clear();
		cache.clear();
		currentSize = 0;
	}

	std::size_t size() {
		return currentSize;
	}

	void resize(std::size_t newMaxSize) {
		if (newMaxSize == 0)
			throw std::invalid_argument("Maximum size cannot be zero.");
		
		else if (newMaxSize < currentSize) {
			for (std::size_t i = 0; i < currentSize - newMaxSize; ++i) {
				KeyType key = getLeastUsed().first;
				
				cache.erase(cacheMap.at(key));
				cacheMap.erase(key);
			}
			currentSize = newMaxSize;
		}

		maxSize = newMaxSize;
	}
};
