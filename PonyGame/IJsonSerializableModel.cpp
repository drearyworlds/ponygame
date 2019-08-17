#include "IJsonSerializableModel.h"
#include "IJsonSerializableModelTemplates.h"

using namespace SmartCore::Common;

IJsonSerializableModel::IJsonSerializableModel() {
}

IJsonSerializableModel::~IJsonSerializableModel() {
}

bool IJsonSerializableModel::DeserializeFromFile(const std::wstring& /*filePath*/) {
    return /*Deserialize(Utilities::ReadFile(filePath))*/ true;
}

bool IJsonSerializableModel::SafeJsonParse(const std::wstring& jsonString, web::json::value& jsonValue) {
    bool returnStatus = true;

    // If the string is not empty
    if (!jsonString.empty()) {
        // Make sure it is a valid json string
        std::error_code errorCode;
        jsonValue = web::json::value::parse(jsonString, errorCode);

        // If it is not, return an error
        //if (errorCode.value() != JSON_PARSE_SUCCESS) {
            jsonValue = web::json::value::null();
            returnStatus = false;
        //}
    } else {
        // The string is empty, but shouldn't be
        jsonValue = web::json::value::null();
        returnStatus = false;
    }

    return returnStatus;
}

bool IJsonSerializableModel::ParseAndValidateField(const std::wstring& jsonString, const std::wstring& fieldName, web::json::value& jsonOutput) {
    web::json::value field;
    bool returnStatus = SafeJsonParse(jsonString, field);

    if ((returnStatus)) {
        if (field.has_field(fieldName) && !field.at(fieldName).is_null()) {
            jsonOutput = field.at(fieldName);
            returnStatus = true;
        } else {
            returnStatus = false;
        }
    }

    return returnStatus;
}

bool IJsonSerializableModel::ValidateJson(const std::wstring& jsonString) {
    bool returnValue = false;

    web::json::value jsonValue;
    if (jsonString.empty()) {
        returnValue = true;
    } else if ((SafeJsonParse(jsonString, jsonValue))) {
        returnValue = ValidateJson(jsonValue);
    }

    return returnValue;
}

bool IJsonSerializableModel::ValidateJson(const web::json::value& jsonValue) {
    bool returnValue = true;
    if (jsonValue.is_object()) {
        for each (auto value in jsonValue.as_object()) {
            returnValue &= ValidateJson(value.second);
        }
    } else {
        if (jsonValue.is_string() /*&& StringUtilities::ContainsInvalidCharacters(jsonValue.as_string())*/) {
            return false;
        }
    }

    return returnValue;
}

bool IJsonSerializableModel::SerializeJsonStringAsJsonValue(const std::wstring& fieldName, const std::wstring& jsonString, web::json::value& jsonOutput) {
    bool result = true;

    // If the string and the field are not empty
    if (!jsonString.empty() && !fieldName.empty()) {
        // Make sure it is a valid json string
        result = SafeJsonParse(jsonString, jsonOutput[fieldName]);
    } else {
        // The string is empty, but shouldn't be
        result = false;
        jsonOutput = web::json::value::null();
    }

    return result;
}

bool IJsonSerializableModel::SerializeJsonString(const std::wstring& fieldName, const std::wstring& stringValue, web::json::value& jsonOutput) {
    bool returnValue = true;

    try {
        std::wstring stringToSerialize = stringValue;

        if (!stringValue.empty() && (stringValue[stringValue.length() - 1] == L'\0')) {
            stringToSerialize = stringValue.substr(0, stringValue.length() - 1);
        }

        jsonOutput[fieldName] = web::json::value(stringToSerialize);
    } catch (web::json::json_exception&) {
        returnValue = false;
    }

    return returnValue;
}

bool IJsonSerializableModel::SerializeJsonBoolean(const std::wstring& fieldName, const bool boolValue, web::json::value& jsonOutput) {
    bool returnValue = true;

    try {
        jsonOutput[fieldName] = web::json::value(boolValue);
    } catch (web::json::json_exception&) {
        returnValue = false;
    }

    return returnValue;
}

bool IJsonSerializableModel::SerializeJsonUuid(const std::wstring& fieldName, const UUID& /*uuidValue*/, web::json::value& jsonOutput, bool /*okToBeInvalid*/) {
    bool returnValue = true;

    try {
        jsonOutput[fieldName] = web::json::value(/*StringUtilities::UniqueIdToString(uuidValue, okToBeInvalid)*/);
    } catch (web::json::json_exception&) {
        returnValue = false;
    }

    return returnValue;
}

bool IJsonSerializableModel::SerializeJsonModel(const std::wstring& fieldName, const IJsonSerializableModel& jsonModel, web::json::value& jsonOutput, bool okToBeInvalid) {
    bool returnValue = true;

    try {
        // Only serialize models that are valid
        bool status;
        // Check both until all models implement validate with the status
        if (jsonModel.Validate(status) && (status)) {
            // Serialize model that is valid
            IJsonSerializableModel::SafeJsonParse(jsonModel.Serialize(), jsonOutput[fieldName]);
        } else if (okToBeInvalid) {
            // Empty object for model that is required but invalid
            jsonOutput[fieldName] = web::json::value::object();
        }
    } catch (web::json::json_exception&) {
        returnValue = false;
    }

    return returnValue;
}

bool IJsonSerializableModel::SerializeJsonUuidVector(const std::wstring& fieldName, const std::vector<UUID>& uuidVector, web::json::value& jsonOutput, bool required) {
    bool returnValue = true;

    try {
        web::json::value jsonArray;

        if (!uuidVector.empty()) {
            for (unsigned int i = 0; i < uuidVector.size(); i++) {
                jsonArray[i] = web::json::value(/*StringUtilities::UniqueIdToString(uuidVector.at(i))*/);
            }

            jsonOutput[fieldName] = jsonArray;
        } else if (required) {
            // Empty object for model that is required but invalid
            jsonOutput[fieldName] = web::json::value::array();
        }
    } catch (web::json::json_exception&) {
        returnValue = false;
    }

    return returnValue;
}

bool IJsonSerializableModel::GetJsonString(const std::wstring& jsonString, const std::wstring& fieldName, std::wstring& outputString, bool required) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    // Make sure we don't try to convert this unless it's actually found
    if ((result)) {
        if (jsonValue.is_string()) {
            outputString = jsonValue.as_string();
        } else {
            result = false;
        }
    } else if (!required) {
        // If we are missing this field, but it's not required, go ahead and return success
        result = true;
    }

    return result;
}

bool IJsonSerializableModel::GetJsonBoolean(const std::wstring& jsonString, const std::wstring& fieldName, bool& outputValue, bool required) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    // Make sure we don't try to convert this unless it's actually found
    if ((result)) {
        if (jsonValue.is_boolean()) {
            outputValue = jsonValue.as_bool();
        } else {
            result = false;
        }
    } else if (!required) {
        // If we are missing this field, but it's not required, go ahead and return success
        result = true;
    }

    return result;
}

bool IJsonSerializableModel::GetJsonUuid(const std::wstring& jsonString, const std::wstring& fieldName, UUID& /*outputValue*/, bool required, bool /*okToBeInvalid*/) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    // Make sure we don't try to convert this unless it's actually found
    if ((result)) {
        if (jsonValue.is_string()) {
            auto guidString = jsonValue.as_string();
            //outputValue = StringUtilities::StringToUniqueId(guidString, okToBeInvalid);
            //if (outputValue == INVALID_UUID && !okToBeInvalid) {
            //    result = SL_INVALID_UUID;
            //}
        } else {
            result = false;
        }
    } else if (!required) {
        // If we are missing this field, but it's not required, go ahead and return success
        result = true;
    }

    return result;
}

bool IJsonSerializableModel::GetJsonUuidVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<UUID>& /*outputValue*/, bool required, bool /*okToBeInvalid*/) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    // Make sure we don't try to convert this unless it's actually found
    if ((result)) {
        if (jsonValue.is_array()) {
            web::json::array jsonArray = jsonValue.as_array();

            for (unsigned int i = 0; i < jsonArray.size(); i++) {
                auto guidString = jsonArray.at(i).as_string();
                //outputValue.push_back(StringUtilities::StringToUniqueId(guidString, okToBeInvalid));
            }
        } else {
            result = false;
        }
    } else if (!required) {
        // If we are missing this field, but it's not required, go ahead and return success
        result = true;
    }

    return result;
}

bool IJsonSerializableModel::GetJsonModel(const std::wstring& jsonString, const std::wstring& fieldName, IJsonSerializableModel& outputModel, bool required) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    // Make sure we don't try to convert this unless it's actually found
    if ((result)) {
        if (jsonValue.is_object()) {
            const std::wstring& jsonObjectString = jsonValue.serialize();
            result = outputModel.Deserialize(jsonObjectString);
        } else {
            result = false;
        }
    } else if (!required) {
        // If we are missing this field, but it's not required, go ahead and return success
        result = true;
    }

    return result;
}

bool IJsonSerializableModel::GetJsonVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<web::json::value>& outputVector, bool required) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    // Make sure we don't try to convert this unless it's actually found
    if ((result)) {
        if (jsonValue.is_array()) {
            web::json::array jsonArray = jsonValue.as_array();

            for (unsigned int i = 0; i < jsonArray.size(); i++) {
                outputVector.push_back(web::json::value(jsonArray.at(i)));
            }
        } else {
            result = false;
        }
    } else if (!required) {
        // If we are missing this field, but it's not required, go ahead and return success
        result = true;
    }

    return result;
}

bool IJsonSerializableModel::GetJsonStringVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<std::wstring>& outputVector, bool required) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    // Make sure we don't try to convert this unless it's actually found
    if ((result)) {
        if (jsonValue.is_array()) {
            web::json::array jsonArray = jsonValue.as_array();

            for (unsigned int i = 0; i < jsonArray.size(); i++) {
                if (jsonArray.at(i).is_string()) {
                    std::wstring theString = jsonArray.at(i).as_string();
                    outputVector.push_back(theString);
                } else {
                    result = false;
                    break;
                }
            }
        } else {
            result = false;
        }
    } else if (!required) {
        // If we are missing this field, but it's not required, go ahead and return success
        result = true;
    }

    return result;
}

bool IJsonSerializableModel::GetJsonIntegerVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<int>& outputVector, bool required) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    // Make sure we don't try to convert this unless it's actually found
    if ((result)) {
        if (jsonValue.is_array()) {
            web::json::array jsonArray = jsonValue.as_array();

            for (unsigned int i = 0; i < jsonArray.size(); i++) {
                if (jsonArray.at(i).is_string()) {
                    int theInt = jsonArray.at(i).as_integer();
                    outputVector.push_back(theInt);
                } else {
                    result = false;
                    break;
                }
            }
        } else {
            result = false;
        }
    } else if (!required) {
        // If we are missing this field, but it's not required, go ahead and return success
        result = true;
    }

    return result;
}

bool SmartCore::Common::IJsonSerializableModel::GetJsonMap(const std::wstring& jsonString, const std::wstring& fieldName, std::map<std::wstring, std::wstring>& outputValue, bool required) {
    web::json::value jsonValue;
    bool result = ParseAndValidateField(jsonString, fieldName, jsonValue);

    if ((result)) {
        if (jsonValue.is_object()) {
            web::json::object jsonObject = jsonValue.as_object();

            for (auto pair = jsonObject.begin(); pair != jsonObject.end(); pair++) {
                if (pair->second.is_string()) {
                    outputValue[pair->first] = static_cast<std::wstring>(pair->second.as_string());
                }
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

    return result;
}

bool IJsonSerializableModel::GetJsonKeyValuePairVector(const std::wstring& jsonString, const std::wstring& fieldName, const std::wstring& keyFieldName, const std::wstring& valueFieldName, std::map<std::wstring, std::wstring>& outputValue, bool required) {
    bool returnStatus;

    std::vector<web::json::value> jsonVector;
    returnStatus = GetJsonVector(jsonString, fieldName, jsonVector, required);
    if (returnStatus) {
        for (const auto& fieldPair : jsonVector) {
            std::wstring serializedFieldPair = fieldPair.serialize();
            std::wstring key;
            returnStatus = GetJsonString(serializedFieldPair, keyFieldName, key);
            if (returnStatus) {
                std::wstring value;
                returnStatus = GetJsonString(serializedFieldPair, valueFieldName, value);
                if (returnStatus) {
                    outputValue[key] = value;
                }
            }
        }
    }

    return returnStatus;
}