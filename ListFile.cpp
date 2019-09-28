/*
CSE 109: Fall 2018
Lydia Cornwell
LAC221
creates a linked list of nodes
Program #5
*/
#include"ListFile.h"
#include"Node.h"
#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<cstring>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<iostream>
#include<errno.h>

using namespace std;

ListFile_t::ListFile_t()
{
	this->head = NULL;
	this->size = 0;
}

ListFile_t::ListFile_t(const ListFile_t& src)
{
	head = NULL;
	size = 0;

	Node_t* temp = src.head;
	while(temp != NULL)
	{
		insert(temp->getName(), temp->getData(), temp->getNodeSize());
		temp = (*temp).getNext();
	}
}

ListFile_t& ListFile_t::operator=(const ListFile_t& rhs)
{
	clear();

	head = NULL;
	size = 0;

	Node_t* temp = rhs.head;
	while(temp != NULL)
	{
		insert(temp->getName(), temp->getData(), temp->getNodeSize());
		temp = (*temp).getNext();
	}
	//*this = move(ListFile_t(rhs));
	return *this;
}

ListFile_t::~ListFile_t()
{
	Node_t* current = this->head;

	while(current != NULL)
	{
		Node_t* temp = current->getNext();
		delete current;
		current = temp;
	}

	this->head = NULL;
	this->size = 0;
}

int ListFile_t::readFile(const string& filename)
{
	int file = open(filename.c_str(), O_RDONLY);
	if(file < 0)
	{
		//cerr << "Error opening file (" << errno << "): " << strerror(errno) << endl;
		return -1;
	}
	close(file);

	ListFile_t newList;

	if(newList.appendFromFile(filename) == -1)
	{
		return -1;
	}

	this->clear();
	(*this) = (newList);

	return 0;
	//check that you can open
}

ssize_t ListFile_t::appendFromFile(const string& filename)
{
	int file = open(filename.c_str(), O_RDONLY);
	if(file < 0)
	{
		//cerr << "Error opening file (" << errno << "): " << strerror(errno) << endl;
		return -1;
	}

	ListFile_t newList(*this);

	int count = 0;

	size_t listSize = 0;
	int retval = read(file, &listSize, 8);
	if(retval <= 0)
	{
		//cerr << "Error reading from file (" << errno << "): " << strerror(errno) << endl;
		close(file);
		return -1;
	}

	//cerr << "Reading " << listSize << " nodes from file" << endl;
	for(size_t i = 0; i < listSize; i++)
	{
		size_t nameLength = 0;
		size_t dataLength = 0;

		retval = read(file, &nameLength, 8);
		if(retval <= 0)
		{
			//cerr << "Error reading from file (" << errno << "): " << strerror(errno) << endl;
			close(file);
			return -1;
		}

		retval = read(file, &dataLength, 8);
		if(retval <= 0)
		{
			//cerr << "Error reading from file (" << errno << "): " << strerror(errno) << endl;
			close(file);
			return -1;
		}

		char* name = (char*)malloc((nameLength + 1) * sizeof(char));
		void* data = malloc(dataLength);

		retval = read(file, name, nameLength);
		if(retval <= 0)
		{
			//cerr << "Error reading from file (" << errno << "): " << strerror(errno) << endl;
			close(file);
			free(name);
			free(data);
			return -1;
		}
		name[nameLength] = '\0';

		retval = read(file, data, dataLength);
		if(retval <= 0)
		{
			//cerr << "Error reading from file (" << errno << "): " << strerror(errno) << endl;
			close(file);
			free(name);
			free(data);
			return -1;
		}

		count += newList.insert(name, data, dataLength);

		free(name);
		name = NULL;
		free(data);
		data = NULL;

		//cerr << "Read Node " << i << endl;
	}

	close(file);

	this->clear();
	(*this) = move(newList);

	return count;
}

int ListFile_t::saveToFile(const string& filename) const
{
	int file = open(filename.c_str(), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
	if(file < 0)
	{
		//cerr << "Error opening file (" << errno << "): " << strerror(errno) << endl;
		return -1;
	}

	size_t count = 0;

	int retval = write(file, &size, 8);
	if(retval < 0)
	{
		//cerr << "Error writing to file (" << errno << "): " << strerror(errno) << endl;
		close(file);
		return -1;
	}

	Node_t* current = this->head;
	while(current != NULL)
	{
		size_t nameLength;
		size_t dataLength;
		nameLength = current->getName().length();
		dataLength = current->getNodeSize();

		retval = write(file, &nameLength, 8);
		if(retval < 0)
		{
			//cerr << "Error writing to file (" << errno << "): " << strerror(errno) << endl;
			close(file);
			return -1;
		}

		retval = write(file, &dataLength, 8);
		if(retval < 0)
		{
			//cerr << "Error writing to file (" << errno << "): " << strerror(errno) << endl;
			close(file);
			return -1;
		}

		retval = write(file, current->getName().c_str(), nameLength);
		if(retval < 0)
		{
			//cerr << "Error writing to file (" << errno << "): " << strerror(errno) << endl;
			close(file);
			return -1;
		}

		retval = write(file, current->getData(), dataLength);
		if(retval < 0)
		{
			//cerr << "Error writing to file (" << errno << "): " << strerror(errno) << endl;
			close(file);
			return -1;
		}

		count++;
		current = current->getNext();
	}

	close(file);

	if(count != size)
	{
		//cerr << "Expected " << size << " nodes. Only found " << count << endl;
		return -1;
	}

	return 1;
}

size_t ListFile_t::getSize() const
{
	return this->size;
}

size_t ListFile_t::getElementSize(size_t index) const
{
	if(index >= size)
	{
		return 0;
	}

	return (*this)[index].getNodeSize();
}

void* ListFile_t::getElementData(size_t index) const
{
	if(index >= size)
	{
		return NULL;
	}

	return (*this)[index].getData();
}

string ListFile_t::getElementName(size_t index) const
{
	if(index >= size)
	{
		return string("");
	}

	return (*this)[index].getName();
	//return temp.getName();
}

const Node_t& ListFile_t::operator[](size_t index) const
{
	Node_t* current = this->head;

	size_t currentIndex = 0;

	while(index > currentIndex)
	{
		currentIndex++;
		current = current->getNext();
	}

	return *current;
}

Node_t& ListFile_t::operator[](size_t index)
{
	Node_t* current = this->head;

	size_t currentIndex = 0;

	while(index > currentIndex)
	{
		currentIndex++;
		current = current->getNext();
	}

	return *current;
}

void ListFile_t::clear()
{
	Node_t* current = this->head;

	while(current != NULL)
	{
		Node_t* temp = current->getNext();
		delete current;
		current = temp;
	}

	this->head = NULL;
	this->size = 0;
}

bool ListFile_t::exists(const string& name) const
{
	Node_t* temp = this->head;
	while(temp != NULL)
	{
		if(temp->getName() == name)
		{
			return true;
		}
		temp = temp->getNext();
	}
	return false;
}

size_t ListFile_t::count(void* data, size_t size) const
{
	Node_t* temp = this->head;
	size_t count = 0;
	while(temp != NULL)
	{
		if(temp->getNodeSize() == size)
		{
			if(memcmp(temp->getData(), data, size) == 0)
			{
				count++;
			}
		}
		temp = temp->getNext();
	}
	return count;
}

int ListFile_t::removeByName(const string& name)
{
	Node_t* current = this->head;
	Node_t* previous = NULL;

	while(current != NULL)
	{
		int compare = strcmp(name.c_str(), current->getName().c_str());
		if(compare == 0)
		{
			if(previous == NULL)
			{
				head = current->getNext();
			}
			else
			{
				previous->setNext(current->getNext());
			}

			delete current;
			(this->size)--;
			return 1;
		}
		if(compare < 0)
		{
			break;
		}

		previous = current;
		current = current->getNext();
	}

	return 0;
}

int ListFile_t::insert(const string& name, void* data, size_t size)
{
	Node_t* current = this->head;
	Node_t* previous = NULL;

	if(head == NULL)
	{
		head = new Node_t(name, data, size, NULL);
		(this->size)++;
		return 1;
	}

	while(current != NULL)
	{
		int compare = strcmp(name.c_str(), current->getName().c_str());
		if(compare == 0)
		{
			return 0;
		}
		if(compare < 0)
		{
			Node_t* newNode = new Node_t(name, data, size, current);
			if(previous == NULL)
			{
				head = newNode;
			}
			else
			{
				previous->setNext(newNode);
			}
			(this->size)++;
			return 1;
		}

		previous = current;
		current = current->getNext();
	}

	Node_t* newNode = new Node_t(name, data, size, NULL);
	previous->setNext(newNode);
	(this->size)++;
	return 1;
}

void ListFile_t::insertInternal(Node_t* data)
{
}

