#pragma once

#include<cstdlib>
#include<string>

using std::string;

class Node_t
{
public:
	Node_t() = delete;
	Node_t(const string& name, void* data, size_t size, Node_t* next);
	~Node_t();

	string getName() const;
	void* getData() const;
	size_t getNodeSize() const;
	Node_t* getNext() const;

	void setName(string);
	void setData(void*, size_t);
	void setNext(Node_t*);

private:
	string name;
	void* data;
	size_t size;
	Node_t* next;
};
