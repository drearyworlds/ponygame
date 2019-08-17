#pragma once

#include "IJsonSerializableModel.h"
#include <nowarn/cpprest/json.h>
#include <vector>

template <typename T>
bool SmartCore::Common::IJsonSerializableModel::SerializeJsonInteger(const std::wstring& fieldName, const T intValue, web::json::value& json) {
    bool returnValue = true;

    try {
        json[fieldName] = web::json::value(intValue);
    } catch (web::json::json_exception&) {
        returnValue = false;
    }

    return returnValue;
}

template <typename T>
bool SmartCore::Common::IJsonSerializableModel::SerializeJsonValue(const std::wstring& fieldName, const T& value, web::json::value& json) {
    bool returnValue = true;

    try {
        json[fieldName] = web::json::value(value);
    } catch (web::json::json_exception&) {
        returnValue = false;
    }

    return returnValue;
}

template <typename T>
bool SmartCore::Common::IJsonSerializableModel::SerializeJsonModelVector(const std::wstring& fieldName, const std::vector<T>& vectorObject, web::json::value& json) {
    bool returnStatus = true;
    web::json::value jsonArray = web::json::value::array();

    for (unsigned int i = 0; i < vectorObject.size(); i++) {
        bool status;
        // Only serialize keys that are valid
        if (vectorObject.at(i).Validate(status) && (status)) {
            returnStatus = IJsonSerializableModel::SafeJsonParse(vectorObject.at(i).Serialize(), jsonArray[i]);
        }
    }

    json[fieldName] = jsonArray;

    return returnStatus;
}

template <typename T>
bool SmartCore::Common::IJsonSerializableModel::SerializeJsonVector(const std::wstring& fieldName, const std::vector<T>& vectorObject, web::json::value& json) {
    bool returnValue = true;

    try {
        web::json::value jsonArray = web::json::value::array();
        for (unsigned int i = 0; i < vectorObject.size(); i++) {
            jsonArray[i] = web::json::value(vectorObject.at(i));
        }

        json[fieldName] = jsonArray;
    } catch (web::json::json_exception&) {
        returnValue = false;
    }

    return returnValue;
}

template <typename V>
bool SmartCore::Common::IJsonSerializableModel::SerializeJsonKeyValuePairVector(const std::wstring& fieldName, const std::wstring& keyFieldName, const std::wstring& valueFieldName, const std::map<std::wstring, V>& map, web::json::value& json) {
    web::json::value jsonValues;
    bool result;

    try {
        size_t i = 0;
        for (const auto& pair : map) {
            web::json::value pairJson = web::json::value::object();
            pairJson[keyFieldName] = web::json::value(pair.first);
            pairJson[valueFieldName] = web::json::value(pair.second);
            jsonValues[i++] = pairJson;
        }

        result = SerializeJsonValue(fieldName, jsonValues, json);
    } catch (web::json::json_exception&) {
        result = false;
    }

    return result;
}

template <typename V>
bool SmartCore::Common::IJsonSerializableModel::SerializeJsonMap(const std::wstring& fieldName, const std::map<std::wstring, V>& map, web::json::value& json) {
    bool returnValue = true;

    try {
        std::vector<std::pair<std::wstring, web::json::value>> objectVector;
        for each (const std::pair<std::wstring, V>& pair in map) {
            std::pair<std::wstring, web::json::value> jsonPair;
            jsonPair.first = pair.first;
            jsonPair.second = static_cast<web::json::value>(pair.second);
            objectVector.push_back(jsonPair);
        }
        json[fieldName] = web::json::value::object(objectVector);
    } catch (web::json::json_exception&) {
        returnValue = false;
    }

    return returnValue;
}

template <typename T>
bool SmartCore::Common::IJsonSerializableModel::GetJsonInteger(const std::wstring& jsonString, const std::wstring& fieldName, T& outputValue, bool required) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);
    try {
        // Make sure we don't try to convert this unless it's actually found
        if ((result) && (required ? result != false : !required)) {
            if (jsonValue.is_integer()) {
                outputValue = static_cast<T>(jsonValue.as_number().to_uint64());
            } else {
                result = false;
            }
        } else if (!required) {
            // If we are missing this field, but it's not required, go ahead and return success
            result = true;
        }
    } catch (web::json::json_exception&) {
        result = false;
    }

    return result;
}

template <typename T>
bool SmartCore::Common::IJsonSerializableModel::GetJsonValue(const std::wstring& jsonString, const std::wstring& fieldName, T& outputValue, bool required) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    try {
        // Make sure we don't try to convert this unless it's actually found
        if ((result) && (required ? result != false : !required)) {
            if (jsonValue.is_integer()) {
                outputValue = static_cast<T>(jsonValue.as_integer());
            } else {
                result = false;
            }
        } else if (!required) {
            // If we are missing this field, but it's not required, go ahead and return success
            result = true;
        }
    } catch (web::json::json_exception&) {
        returnValue = false;
    }

    return result;
}

template <typename T>
bool SmartCore::Common::IJsonSerializableModel::GetJsonModelVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<T>& outputValue, bool required) {
    web::json::value jsonValue;
	//we want the value returned by this function to reflect only the vector in the serialized object
	outputValue.clear();
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    try {
        // Make sure we don't try to convert this unless it's actually found
        if ((result) && (result != false)) {
            if (jsonValue.is_array()) {
                web::json::array jsonArray = jsonValue.as_array();

                for (unsigned int i = 0; i < jsonArray.size(); i++) {
                    T temp;
                    bool didItWork = temp.Deserialize(jsonArray.at(i).serialize());
                    if ((didItWork)) {
                        outputValue.push_back(temp);
                    } else {
                        result = didItWork;
                    }
                }
            } else {
                result = false;
            }
        } else if (!required) {
            // If we are missing this field, but it's not required, go ahead and return success
            result = true;
        }
    } catch (web::json::json_exception&) {
        result = false;
    }

    return result;
}

template <typename T>
bool SmartCore::Common::IJsonSerializableModel::GetJsonNumberVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<T>& outputValue, bool required) {
    web::json::value jsonValue;
	//we want the value returned by this function to reflect only the vector in the serialized object
	outputValue.clear();
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    try {
        // Make sure we don't try to convert this unless it's actually found
        if ((result)) {
            if (jsonValue.is_array()) {
                web::json::array jsonArray = jsonValue.as_array();

                for (unsigned int i = 0; i < jsonArray.size(); i++) {
                    outputValue.push_back(static_cast<T>(jsonArray.at(i).as_number().to_uint64()));
                }
            } else {
                result = false;
            }
        } else if (!required) {
            // If we are missing this field, but it's not required, go ahead and return success
            result = true;
        } else {
            result = false;
        }
    } catch (web::json::json_exception&) {
        result = false;
    }

    return result;
}