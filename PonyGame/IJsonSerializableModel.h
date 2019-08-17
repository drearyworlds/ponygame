#pragma once

#include <nowarn/vector>
#include <nowarn/string>
#include <nowarn/map>
#include <nowarn/rpc.h>

namespace web {
    namespace json {
        class value;
    }
}

namespace SmartCore {
    namespace Common {
        /// <summary>Interface which describes standard JSON serialization behavior </summary>
        class IJsonSerializableModel {
        private:
            /// Validates the json value for any invalid characters
            static bool ValidateJson(const web::json::value& jsonValue);

        protected:
            /// <summary>Constructor</summary>
            IJsonSerializableModel();

            /// <summary>Virtual destructor</summary>
            virtual ~IJsonSerializableModel();

            /// Private helper to handle invalid fields or json that can't be parsed
            static bool ParseAndValidateField(const std::wstring& jsonString, const std::wstring& fieldName, web::json::value& jsonOutput);

            /// Validates the json string for any invalid characters
            static bool ValidateJson(const std::wstring& jsonString);

        public:
            /// Helper function to parse JSON safely and not throw an exception
            static bool SafeJsonParse(const std::wstring& jsonString, web::json::value& jsonValue);

            /// <summary>Returns this object serialized as a JSON string</summary>
            /// <return>The JSON string that represents this object</return>
            virtual std::wstring Serialize() const = 0;

            /// <summary>Deserializes the string passed and sets the properties of this object</summary>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            virtual bool Deserialize(const std::wstring& jsonString) = 0;

            /// <summary>Loads json from a file and deserializes it into this object</summary>
            /// <param name="filePath">The path of the file to load</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            bool DeserializeFromFile(const std::wstring& filePath);

            /// <summary>
            /// Validate the object.  It's up to the object that implements this interface to override this method
            /// </summary>
            /// <param name="status">bool object</param>
            /// <returns>false unless overridden by implementing class</returns>
            virtual bool Validate(bool& status) const = 0;

            static bool SerializeJsonStringAsJsonValue(const std::wstring& fieldName, const std::wstring& jsonString, web::json::value& jsonOutput);

            /// <summary>Takes in a field name and an string, serializes it, and puts it on the output JSON object</summary>
            /// <param name="fieldName">Json field name to set</param>
            /// <param name="stringValue">The value to set it to</param>
            /// <param name="jsonOutput">The json output to add this serialized string to</param>
            /// <returns>bool true if successful, or an SL error code otherwise</returns>
            static bool SerializeJsonString(const std::wstring& fieldName, const std::wstring& stringValue, web::json::value& jsonOutput);

            /// <summary>Takes in a field name and an int, serializes it, and puts it on the output JSON object</summary>
            /// <param name="fieldName">Json field name to set</param>
            /// <param name="intValue">The value to set it to</param>
            /// <param name="jsonOutput">The json output to add this serialized int to</param>
            /// <returns>bool true if successful, or an SL error code otherwise</returns>
            template <typename T>
            static bool SerializeJsonInteger(const std::wstring& fieldName, const T intValue, web::json::value& jsonOutput);

            /// <summary>Takes in a field name and a bool, serializes it, and puts it on the output JSON object</summary>
            /// <param name="fieldName">Json field name to set</param>
            /// <param name="boolValue">The value to set it to</param>
            /// <param name="jsonOutput">The json output to add this serialized bool to</param>
            /// <returns>bool true if successful, or an SL error code otherwise</returns>
            static bool SerializeJsonBoolean(const std::wstring& fieldName, const bool boolValue, web::json::value& jsonOutput);

            /// <summary>Takes in a field name and a UUID, serializes it, and puts it on the output JSON object</summary>
            /// <param name="fieldName">Json field name to set</param>
            /// <param name="uuidValue">The value to set it to</param>
            /// <param name="jsonOutput">The json output to add this serialized UUID to</param>
            /// <param name="okToBeInvalid">This will be passed into UniqueIdToString to suppress warnings from invalid UUIDs</param>
            /// <returns>bool true if successful, or an SL error code otherwise</returns>
            static bool SerializeJsonUuid(const std::wstring& fieldName, const UUID& uuidValue, web::json::value& jsonOutput, bool okToBeInvalid = false);

            /// <summary>Takes in a field name and an IJsonSerializable Object, serializes it, and puts it on the output JSON object</summary>
            /// <param name="fieldName">Json field name to set</param>
            /// <param name="jsonModel">The object to serialize and to set it to</param>
            /// <param name="jsonOutput">The json output to add this serialized object to</param>
            /// <param name="okToBeInvalid">If it's ok to be invalid, we will serialize an empty object for invalid models</param>
            /// <returns>bool true if successful, or an SL error code otherwise</returns>
            static bool SerializeJsonModel(const std::wstring& fieldName, const IJsonSerializableModel& jsonModel, web::json::value& jsonOutput, bool okToBeInvalid = true);

            /// <summary>Takes in a field name and a generic value, serializes it, and puts it on the output JSON object</summary>
            /// <param name="fieldName">Json field name to set</param>
            /// <param name="value">The value to set it to</param>
            /// <param name="jsonOutput">The json output to add this serialized string to</param>
            /// <returns>bool true if successful, or an SL error code otherwise</returns>
            template <typename T>
            static bool SerializeJsonValue(const std::wstring& fieldName, const T& value, web::json::value& jsonOutput);

            /// <summary>Takes in a field name and a vector of primitives, serializes the values inside, and puts it on the output JSON object</summary>
            /// <param name="fieldName">Json field name to set</param>
            /// <param name="outputVector">The vector to serialize and to set it to</param>
            /// <param name="jsonOutput">The json output to add this serialized vector to</param>
            /// <returns>bool true if successful, or an SL error code otherwise</returns>
            template <typename T>
            static bool SerializeJsonVector(const std::wstring& fieldName, const std::vector<T>& outputVector, web::json::value& jsonOutput);

            /// <summary>Takes in a field name and a vector of IJsonSerializableModel-implementing objects, serializes the values inside, and puts it on the output JSON object</summary>
            /// <param name="fieldName">Json field name to set</param>
            /// <param name="modelVector">The vector to serialize and to set it to</param>
            /// <param name="jsonOutput">The json output to add this serialized vector to</param>
            /// <returns>bool true if successful, or an SL error code otherwise</returns>
            template <typename T>
            static bool SerializeJsonModelVector(const std::wstring& fieldName, const std::vector<T>& modelVector, web::json::value& jsonOutput);

            template <typename V>
            static bool SerializeJsonKeyValuePairVector(const std::wstring& fieldName, const std::wstring& keyFieldName, const std::wstring& valueFieldName, const std::map<std::wstring, V>& map, web::json::value& json);

            template <typename V>
            static bool SerializeJsonMap(const std::wstring& fieldName, const std::map<std::wstring, V>& map, web::json::value& jsonOutput);

            template <typename T>
            static std::wstring SerializeJsonModelVectorToString(const std::vector<T>& modelVector) {
                std::wstring jsonString = L"";

                if (!modelVector.empty()) {
                    jsonString = L"[";

                    for (size_t i = 0; i < modelVector.size(); i++) {
                        jsonString += (i != 0 ? L"," : L"");
                        T customField = modelVector.at(i);
                        jsonString += customField.Serialize();
                    }

                    jsonString += L"]";
                }

                return jsonString;
            }

            /// <summary>Takes in a field name and a vector of UUID structs, serializes the values inside, and puts it on the output JSON object</summary>
            /// <param name="fieldName">Json field name to set</param>
            /// <param name="uuidVector">The vector to serialize and to set it to</param>
            /// <param name="jsonOutput">The json output to add this serialized vector to</param>
            /// <returns>bool true if successful, or an SL error code otherwise</returns>
            static bool SerializeJsonUuidVector(const std::wstring& fieldName, const std::vector<UUID>& uuidVector, web::json::value& jsonOutput, bool required = true);

            /// <summary>Reads a JSON value for a top-level field of a generic type</summary>
            /// <param name="jsonString">JSON string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            template <typename T>
            static bool GetJsonValue(const std::wstring& jsonString, const std::wstring& fieldName, T& outputValue, bool required = true);

            /// <summary>Reads a JSON string for a top-level field of the type string</summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            static bool GetJsonString(const std::wstring& jsonString, const std::wstring& fieldName, std::wstring& outputValue, bool required = true);

            /// <summary>Reads a JSON string for a top-level field of the type int</summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            template <typename T>
            static bool GetJsonInteger(const std::wstring& jsonString, const std::wstring& fieldName, T& outputValue, bool required = true);

            /// <summary>Reads a JSON string for a top-level field of the type bool</summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            static bool GetJsonBoolean(const std::wstring& jsonString, const std::wstring& fieldName, bool& outputValue, bool required = true);

            /// <summary>Reads a JSON string for a top-level field of the type UUID</summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            static bool GetJsonUuid(const std::wstring& jsonString, const std::wstring& fieldName, UUID& uuid, bool required = true, bool okToBeInvalid = false);

            /// <summary>Reads a JSON string for a top-level field of the type UUID</summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            static bool GetJsonUuidVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<UUID>& uuidVector, bool required = true, bool okToBeInvalid = false);

            /// <summary>Reads a JSON string for a top-level field of the type web::json::object
            /// and puts it into the IJsonSerializableModel passed in</summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            static bool GetJsonModel(const std::wstring& jsonString, const std::wstring& fieldName, IJsonSerializableModel& model, bool required = true);

            /// <summary>Reads a JSON string for a top-level field of the type web::json::array
            /// and puts it into a vector of web::json::value objects</summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            static bool GetJsonVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<web::json::value>& outputValue, bool required = true);

            /// <summary>Reads a JSON string for a top-level field of the type web::json::array
            /// and puts it into a vector of std::wstring</summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            static bool GetJsonStringVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<std::wstring>& outputValue, bool required = true);
            /// <summary>Reads a JSON string for a top-level field of the type web::json::array
            /// and puts it into a vector of int</summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            static bool GetJsonIntegerVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<int>& outputValue, bool required = true);
            /// <summary>Reads a JSON string for a top-level field of the type web::json::array
            /// and puts it into a vector of web::json::value objects</summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            template<typename T>
            static bool GetJsonModelVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<T>& outputValue, bool required = true);

            static bool GetJsonMap(const std::wstring& jsonString, const std::wstring& fieldName, std::map<std::wstring, std::wstring>& outputValue, bool required);

            template<typename T>
            static bool GetJsonNumberVector(const std::wstring& jsonString, const std::wstring& fieldName, std::vector<T>& outputValue, bool required = true);

            /// <summary>Reads a JSON string for a top-level field of the type web::json::object
            /// and puts it into a std::map of <std::wstring, std::wstring></summary>
            /// <param name="jsonString">Json string to read from</param>
            /// <param name="fieldName">Field name to look for</param>
            /// <param name="outputValue">value to set if found</param>
            /// <returns>bool true if successful, or SL error code otherwise</returns>
            static bool GetJsonKeyValuePairVector(const std::wstring& jsonString, const std::wstring& fieldName, const std::wstring& keyFieldName, const std::wstring& valueFieldName, std::map<std::wstring, std::wstring>& outputValue, bool required = true);
        };
    }
}
