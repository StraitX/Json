#pragma once

#include <nlohmann/json.hpp>
#include <core/string.hpp>
#include <core/list.hpp>
#include <core/hash_table.hpp>
#include <core/type_traits.hpp>

using Json = nlohmann::json;

namespace Details {
	std::string ToJsonKey(StringView string) {
		return {string.Data(), string.Size()};
	}

	std::string ToJsonKey(String string) {
		return string;
	}

	std::string ToJsonKey(std::string string) {
		return string;
	}
	
	template<typename T>
	T FromJsonKey(const std::string &key);

	template<>
	String FromJsonKey(const std::string& key) {
		return String::FromStdString(key);
	}

	template<>
	std::string FromJsonKey(const std::string& key) {
		return key;
	}
	
	template<typename T>
	struct IsJsonKey {
		
		template<typename _T>
		static constexpr bool IsKey(...) {
			return false;
		}

		template<typename _T>
		static constexpr bool IsKey(decltype(ToJsonKey(Declval<_T>()))*) {
			return true;
		}

		static constexpr bool Value = IsKey<T>(nullptr);
	};
}

inline void to_json(Json& json, const String& string) {

	json = "";
	Json::string_t *json_string = json.get_ptr<Json::string_t*>();
	*json_string = string;
}

inline void to_json(Json& json, const StringView& string) {
	json = "";
	Json::string_t *json_string = json.get_ptr<Json::string_t*>();
	*json_string = {string.Data(), string.Size()};
}

inline void from_json(const Json& json, String& string) {
	string = String::FromStdString(json);
}

template<typename T, typename AllocatorType>
void to_json(Json& json, const List<T, AllocatorType>& list)
{
	json = Json::array();

	for(const T &element: list){
		json.push_back(Json(element));
	}
}

template<typename T, typename AllocatorType>
void from_json(const Json& json, List<T, AllocatorType>& list)
{
	list.Clear();
	list.Reserve(json.size());

	for (size_t i = 0; i < json.size(); i++) {
		list.Add(json.at(i).get<T>());
	}
}

template<
	typename KeyType, 
	typename ValueType, 
	typename HashType,
	typename = EnableIfType<Details::IsJsonKey<KeyType>::Value>
>
void to_json(Json& json, const HashTable<KeyType, ValueType, HashType>& table) {
	json = Json::object();

	Json::object_t* object = json.get_ptr<Json::object_t*>();

	for (const auto &pair: table) {
		object->emplace(Details::ToJsonKey(pair.first), Json(pair.second));
	}
}

template<
	typename KeyType,
	typename ValueType,
	typename HashType,
	typename = EnableIfType<Details::IsJsonKey<KeyType>::Value>
>
void from_json(const Json& json, HashTable<KeyType, ValueType, HashType>& table) {
	table.Clear();

	SX_ASSERT(json.is_object());

	const Json::object_t* object = json.get_ptr<const Json::object_t*>();

	if(!object)
		return;

    for (const auto& pair : *object)
    {
        table.emplace(Details::FromJsonKey<KeyType>(pair.first), pair.second.get<ValueType>());
    }
}

template<
	typename KeyType, 
	typename ValueType, 
	typename HashType,
	typename = EnableIfType<!Details::IsJsonKey<KeyType>::Value>,
	typename Dummy = void
>
void to_json(Json& json, const HashTable<KeyType, ValueType, HashType>& table) {
	json = Json::array();

	for (const auto &pair: table) {
		Json element = Json::array();

		element.push_back(Json(pair.first));
		element.push_back(Json(pair.second));

		json.push_back(element);
	}
}

template<
	typename KeyType,
	typename ValueType,
	typename HashType,
	typename = EnableIfType<!Details::IsJsonKey<KeyType>::Value>,
	typename Dummy = void
>
void from_json(const Json& json, HashTable<KeyType, ValueType, HashType>& table) {
	table.Clear();

	SX_ASSERT(json.is_array());

	if(!json.is_array())
		return;

    for (const auto& element: json)
    {
        table.emplace(element.at(0).get<KeyType>(), element.at(1).get<ValueType>());
    }
}
