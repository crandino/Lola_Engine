#ifndef __JSON_PARSER_H__
#define __JSON_PARSER_H__

#include "Parson\parson.h"

enum JSON_TYPES
{
	JSON_OBJECT,
	JSON_VALUE, 
	JSON_ARRAY,
	JSON_BOOLEAN,
	JSON_STRING
};

class JSONParser
{

private:

	JSON_Value *value_root = nullptr;
	JSON_Object *root = nullptr;
	JSON_Array *array_root = nullptr;

public:

	JSONParser();
	JSONParser(JSON_Object* node);
	JSONParser(const char *string);
	~JSONParser();

	// Sets
	JSONParser AddNode(const char *name_node);
	bool AddArray(const char *name_array);
	bool AddArray(const JSONParser &block);

	bool AddBoolean(const char *node_name, bool boolean);
	bool AddInt(const char *int_name, int value);
	bool AddString(const char *string_name, const char *string_value);
	bool AppendBoolean(const char *name_boolean, bool boolean);

	// Gets
	JSONParser GetNode(const char *name_node) const;
	JSONParser GetArray(const char *array_name, int index = -1) const;
	int GetArrayCount(const char *array_name) const;

	bool GetBoolean(const char *name_boolean) const;
	const char *GetString(const char *string_name) const;

	// Save
	void Save(char **buf);

	// Free
	void FreeBuffer(char *buff);

};

#endif __JSON_PARSER_H__
