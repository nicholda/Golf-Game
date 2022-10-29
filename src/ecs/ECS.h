#pragma once

#include <iostream>
#include <cassert>
#include <queue>
#include <bitset>
#include <array>
#include <unordered_map>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager {
private:
	// all unused IDs
	std::queue<Entity> m_availableEntities{};
	// array of signatures where the index corr
	std::array<Signature, MAX_ENTITIES> m_signatures{};
	uint32_t m_livingEntityCount{};

public:
	EntityManager() {
		// put all possible IDs in the queue
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
			m_availableEntities.push(entity);
		}
	}

	Entity CreateEntity() {
		assert(m_livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

		// takes ID from front of the queue
		Entity id = m_availableEntities.front();
		m_availableEntities.pop();
		++m_livingEntityCount;

		return id;
	}

	void DestroyEntity(Entity entity) {
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// invalidate the destroyed entity's signature then put at back of queue
		m_signatures[entity].reset();
		m_availableEntities.push(entity);
		--m_livingEntityCount;
	}

	void SetSignature(Entity entity, Signature signature) {
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		m_signatures[entity] = signature;
	}

	Signature GetSignature(Entity entity) {
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		return m_signatures[entity];
	}
};

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
private:
	std::array<T, MAX_ENTITIES> m_componentArray;
	// map from Entity ID to array Index
	std::unordered_map<Entity, size_t> m_entityToIndexMap;
	// map from array index to Entity ID
	std::unordered_map<size_t, Entity> m_indexToEntityMap;

	// size of valid enteries
	size_t m_size;

public:
	void InsertData(Entity entity, T component) {
		assert(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end() &&
			"Component added to same entity more than once.");

		// put new entry at the end	and update the maps
		size_t newIndex = m_size;
		m_entityToIndexMap[entity] = newIndex;
		m_indexToEntityMap[newIndex] = entity;
		m_componentArry[newIndex] = component;
		++m_size;
	}

	void RemoveData(Entity entity) {
		assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() &&
			"Removing non-existent component.");

		// copy element at the end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
		size_t indexOfLastElement = m_size - 1;
		m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

		// Update map
		Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
		m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		m_indexToEntityMap[indexOfLastElement] = entityOfLastElement;

		m_entityToIndexMap.erase(entity);
		m_indexToEntityMap.erase(indexOfLastElement);
		
		--m_size;
	}

	T& GetData(Entity entity) {
		assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() &&
			"Retriving non-existent component.");

		// return a reference to the entity's component
		return m_componentArray[m_entityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override {
		// check if exists
		if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end()) {
			RemoveData(entity);
		}
	}
};