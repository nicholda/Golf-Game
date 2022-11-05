#pragma once

#include <iostream>
#include <cassert>
#include <queue>
#include <bitset>
#include <array>
#include <unordered_map>
#include <set>

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

class ComponentManager {
private:
	std::unordered_map<const char*, ComponentType> m_componentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_componentArrays{};

	ComponentType m_nextComponentType{};

	template <typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		const char* typeName = typeid(T).name();

		assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
	}

public:
	template <typename T>
	void RegisterComponent() {
		const char* typeName = typeid(T).name();
		
		assert(m_componentTypes.find(typeName) == m_componentTypes.end() && "Registering compnent type more than once.");

		// add the component type to the component type map
		m_componentTypes.insert({ typeName, m_nextComponentType });
		// create a ComponentArray pointer and add it to the component arrays map
		m_componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		// increment the value so that the next component registered will be different
		++m_nextComponentType;
	}

	template <typename T>
	ComponentType GetComponentType() {
		const char* typeName = typeid(T).name();

		assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

		return m_componentTypes[typeName];
	}

	template <typename T>
	void AddComponent(Entity entity, T component) {
		// add a component array for an entity
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template <typename T>
	void RemoveComponent(Entity entity) {
		// remove a component from the array for an entity
		GetComponentArray<T>()->RemoveData(entity);
	}

	template <typename T>
	T& GetComponent(Entity entity) {
		// get a reference to a component from the array for an entity
		return GetComponentArray<T>()->GetData();
	}

	void EntityDestroyed(Entity entity) {
		// notify each component array
		// if it has a component for that entity, remove it
		for (auto const& pair : m_componentArrays) {
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}
};

class System {
public:
	std::set<Entity> m_entities;
};

class SystemManager {
private:
	// map from system type string pointer to a signature
	std::unordered_map<const char*, Signature> m_signatures{};
	// map from system type string pointer to a system pointer
	std::unordered_map<const char*, std::shared_ptr<System>> m_systems{};

public:
	template <typename T>
	std::shared_ptr<T> RegisterSystem() {
		const char* typeName = typeid(T).name();

		assert(m_systems.find(typeName) == m_systems.end() && "Registering system more than once.");

		// create a pointer to the system and return it so it can be used externally
		auto system = std::make_shared<T>();
		m_systems.insert({ typeName, system });
		return system;
	}

	template <typename T>
	void SetSignature(Signature signature) {
		const char* typeName = typeid(T).name();

		assert(m_systems.find(typeName) != m_systems.end() && "System used before registered.");

		// set the signature for this system
		m_signatures.insert({ typeName, signature });
	}

	void EntityDestroyed(Entity entity) {
		// erase a destroyed entity from all system lists
		// m_entities is a set so no check needed
		for (auto const& pair : m_systems) {
			auto const& system = pair.second;

			system->m_entities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature) {
		// notify each ssytem that an entity's signature changed
		for (auto const& pair : m_systems) {
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = m_signatures[type];

			// entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature) {
				system->m_entities.insert(entity);
			} // entity signature does not match system signature - erase from set
			else {
				system->m_entities.erase(entity);
			}
		}
	}
};

class Coordinator {
private:
	std::unique_ptr<ComponentManager> m_componentManager;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<SystemManager> m_systemManager;

public:
	void Init()
	{
		// Create pointers to each manager
		m_componentManager = std::make_unique<ComponentManager>();
		m_entityManager = std::make_unique<EntityManager>();
		m_systemManager = std::make_unique<SystemManager>();
	}


	// Entity methods
	Entity CreateEntity()
	{
		return m_entityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		m_entityManager->DestroyEntity(entity);

		m_componentManager->EntityDestroyed(entity);

		m_systemManager->EntityDestroyed(entity);
	}


	// Component methods
	template<typename T>
	void RegisterComponent()
	{
		m_componentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		m_componentManager->AddComponent<T>(entity, component);

		auto signature = m_entityManager->GetSignature(entity);
		signature.set(m_componentManager->GetComponentType<T>(), true);
		m_entityManager->SetSignature(entity, signature);

		m_systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		m_componentManager->RemoveComponent<T>(entity);

		auto signature = m_entityManager->GetSignature(entity);
		signature.set(m_componentManager->GetComponentType<T>(), false);
		m_entityManager->SetSignature(entity, signature);

		m_systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return m_componentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return m_componentManager->GetComponentType<T>();
	}


	// System methods
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return m_systemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		m_systemManager->SetSignature<T>(signature);
	}
};

class ComponentManager {
private:
	std::unordered_map<const char*, ComponentType> m_componentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_componentArrays{};

	ComponentType m_nextComponentType{};

	template <typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray() {
		const char* typeName = typeid(T).name();

		assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
	}

public:
	template <typename T>
	void RegisterComponent() {
		const char* typeName = typeid(T).name();
		
		assert(m_componentTypes.find(typeName) == m_componentTypes.end() && "Registering compnent type more than once.");

		// add the component type to the component type map
		m_componentTypes.insert({ typeName, m_nextComponentType });
		// create a ComponentArray pointer and add it to the component arrays map
		m_componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		// increment the value so that the next component registered will be different
		++m_nextComponentType;
	}

	template <typename T>
	ComponentType GetComponentType() {
		const char* typeName = typeid(T).name();

		assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

		return m_componentTypes[typeName];
	}

	template <typename T>
	void AddComponent(Entity entity, T component) {
		// add a component array for an entity
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template <typename T>
	void RemoveComponent(Entity entity) {
		// remove a component from the array for an entity
		GetComponentArray<T>()->RemoveData(entity);
	}

	template <typename T>
	T& GetComponent(Entity entity) {
		// get a reference to a component from the array for an entity
		return GetComponentArray<T>()->GetData();
	}

	void EntityDestroyed(Entity entity) {
		// notify each component array
		// if it has a component for that entity, remove it
		for (auto const& pair : m_componentArrays) {
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}
};

class System {
public:
	std::set<Entity> m_entities;
};

class SystemManager {
private:
	// map from system type string pointer to a signature
	std::unordered_map<const char*, Signature> m_signatures{};
	// map from system type string pointer to a system pointer
	std::unordered_map<const char*, std::shared_ptr<System>> m_systems{};

public:
	template <typename T>
	std::shared_ptr<T> RegisterSystem() {
		const char* typeName = typeid(T).name();

		assert(m_systems.find(typeName) == m_systems.end() && "Registering system more than once.");

		// create a pointer to the system and return it so it can be used externally
		auto system = std::make_shared<T>();
		m_systems.insert({ typeName, system });
		return system;
	}

	template <typename T>
	void SetSignature(Signature signature) {
		const char* typeName = typeid(T).name();

		assert(m_systems.find(typeName) != m_systems.end() && "System used before registered.");

		// set the signature for this system
		m_signatures.insert({ typeName, signature });
	}

	void EntityDestroyed(Entity entity) {
		// erase a destroyed entity from all system lists
		// m_entities is a set so no check needed
		for (auto const& pair : m_systems) {
			auto const& system = pair.second;

			system->m_entities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature) {
		// notify each ssytem that an entity's signature changed
		for (auto const& pair : m_systems) {
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = m_signatures[type];

			// entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature) {
				system->m_entities.insert(entity);
			} // entity signature does not match system signature - erase from set
			else {
				system->m_entities.erase(entity);
			}
		}
	}
};

