#include "JSONParser.h"

JSONParser::JSONParser()
{
	value_root = json_value_init_object();
	root = json_value_get_object(value_root);
}

JSONParser::JSONParser(JSON_Object* node) : root(node)
{ }

JSONParser::JSONParser(const char *string)
{
	value_root = json_parse_string(string);
	root = json_value_get_object(value_root);
}

JSONParser::~JSONParser()
{
	if (value_root != nullptr)
		json_value_free(value_root);
}

JSONParser JSONParser::AddNode(const char *node_name)
{
	json_object_set_value(root, node_name, json_value_init_object());
	return GetNode(node_name);
}

bool JSONParser::AddBoolean(const char *name_boolean, bool boolean)
{
	return json_object_set_boolean(root, name_boolean, boolean) == JSONSuccess;
}

JSONParser JSONParser::GetNode(const char *name_node) const
{
	return JSONParser(json_object_get_object(root, name_node));
}

bool JSONParser::GetBoolean(const char *name_boolean) const
{
	return (json_object_get_boolean(root, name_boolean) != 0) ? true : false;
}

// Buffer must be deleted calling FreeBuffer(char **buff)
void JSONParser::Save(char **buff)
{	
	size_t bytes_size = json_serialization_size(value_root);
	*buff = new char[bytes_size];
	*buff = json_serialize_to_string_pretty(value_root);
	//json_serialize_to_buffer_pretty(value_root, *buff, bytes_size);
}

void JSONParser::FreeBuffer(char *buff)
{
	if (buff != nullptr)
	{
		delete[] buff;
		buff = nullptr;
	}
}
