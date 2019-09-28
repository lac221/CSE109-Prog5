#pragma once

#include"Node.h"
#include<string>
#include<stdexcept>

using std::string;
using std::out_of_range;

class ListFile_t
{
public:
	ListFile_t();
	ListFile_t(const ListFile_t& src);
	ListFile_t& operator=(const ListFile_t& rhs);
	~ListFile_t();

	int readFile(const string& filename);
	ssize_t appendFromFile(const string& filename);
	int saveToFile(const string& filename) const;

	size_t getSize() const;
	size_t getElementSize(size_t index) const;
	void* getElementData(size_t index) const;
	string getElementName(size_t index) const;
	const Node_t& operator[](size_t index) const;
	Node_t& operator[](size_t index);

	void clear();
	bool exists(const string& name) const;
	size_t count(void* data, size_t size) const;
	int removeByName(const string& name);
	int insert(const string& name, void* data, size_t size);

private:
	Node_t* head;
	size_t size;


	void insertInternal(Node_t* data);
};
