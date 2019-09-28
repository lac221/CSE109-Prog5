/*
CSE 109: Fall 2018
Lydia Cornwell
Lac221
constructs, gets/sets/ nodes
Program #5
*/
#include"Node.h"
#include<stdlib.h>

using namespace std;

Node_t::Node_t(const string& name, void* data, size_t size, Node_t* next)
{
	this->name.assign(name);
	if(data != NULL)
	{
		this->data = malloc(size);
		for(size_t i = 0; i < size; i ++)
		{
			((char*)this->data)[i] = ((char*)data)[i];
		}
	}
	else
	{
		this->data = NULL;
	}

	this->size = size;
	this->next = next;
}

Node_t::~Node_t()
{
	this->name.assign("");
	free(data);
	data = NULL;
	this->size = 0;
	this->next = NULL;
}

string Node_t::getName() const
{
	return this->name;
}

void* Node_t::getData() const
{
	return this->data;
}

size_t Node_t::getNodeSize() const
{
	return this->size;
}

Node_t* Node_t::getNext() const
{
	return this->next;
}

void Node_t::setName(string name)
{
	this->name.assign(name);
}

void Node_t::setData(void* data, size_t size)
{
	this->size = size;
	free(this->data);
	this->data = NULL;

	if(data != NULL)
	{
		this->data = malloc(size);
		for(size_t i = 0; i < size; i ++)
		{
			((char*)this->data)[i] = ((char*)data)[i];
		}
	}
}

void Node_t::setNext(Node_t* next)
{
	this->next = next;
}
