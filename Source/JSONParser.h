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

public:

	JSONParser();
	JSONParser(JSON_Object* node);
	JSONParser(const char *string);
	~JSONParser();

	// Sets
	JSONParser AddNode(const char *name_node);

	bool AddBoolean(const char *name_node, bool boolean);

	// Gets
	JSONParser GetNode(const char *name_node) const;

	bool GetBoolean(const char *name_boolean) const;


	// Save
	void Save(char **buf);

	// Free
	void FreeBuffer(char *buff);

};




#endif __JSON_PARSER_H__
