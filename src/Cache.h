#ifndef CACHE_H
#define CACHE_H

#include "main.h"

enum STATUS_TYPE {NIL, NON_EMPTY, DELETED};

//implement min-heap structure for LFU  replacement method in cache
class minHeapEle{
public:
    Elem* ele;
    int freq;
    minHeapEle(Elem* ele, int freq){
        this->ele = ele;
		this->freq = freq;
    }
};

/*
 * Return Heap Size
 */

//AVL tree
class avl_node
{
public:
	Elem* data;
	avl_node *left;
	avl_node *right;
    avl_node(Elem* &val){
        data = val;
        left = right = NULL;
    }
};

class selfListNode{
	public:
	Elem* elem;
	selfListNode* next;
	selfListNode(): elem(NULL), next(NULL) {};
	selfListNode(Elem* elem){
		this->elem = elem;
		next = NULL;
	}
};


class ReplacementPolicy {
	public:
	//int count;
	virtual Elem* insert(Elem* e) = 0;//insert e into arr[idx] if idx != -1 else into the position by replacement policy
	virtual void print()=0;
	virtual void addFreq(int addr)=0;
	virtual void reArrangeList(int addr)=0;
	virtual void replace(int addr, Data* cont)=0;
	virtual int getSize()=0;
};

class SearchEngine {
	public:
	int count;
	 virtual void insert(Elem *&value)=0;
	 virtual void deleteNode(Elem *&value)=0;
	 virtual Elem* searchAdrr(int addr)=0;
	 virtual void print()=0;
};

class FIFO : public ReplacementPolicy {
	Elem** arr;
	public:
	int count;
	FIFO(){this->count = 0; arr = new Elem*[MAXSIZE];}
	~FIFO(){
		for(int i=0; i<MAXSIZE; i++){
			delete arr[i];
		}
	}
	Elem* insert(Elem* e);
	void replace(int addr, Data* cont);
	void print();
	void addFreq(int addr){return;};
	void reArrangeList(int addr){return;};
	int getSize();
};

class MRU : public ReplacementPolicy {
    selfListNode* head;
	selfListNode* tail;
	int count;
	public:
	MRU(){
        head = tail = NULL;
		this->count = 0;
    }
	~MRU(){
		selfListNode* ptr = head;
		while(ptr != NULL){
			selfListNode* next = ptr->next;
			delete ptr;
			ptr = next;
		}
	}
	Elem* insert(Elem* e);
	void reArrangeList(int addr);
	void replace(int addr, Data* cont);
	void print();
	void addFreq(int addr){return;};
	int getSize();
};

class LRU: public ReplacementPolicy {
    selfListNode* head;
	selfListNode* tail;
	int count;
	public:
	LRU(){
        head = tail = NULL;
		this->count = 0;
    }
	~LRU(){
		selfListNode* ptr = head;
		while(ptr != NULL){
			selfListNode* next = ptr->next;
			delete ptr;
			ptr = next;
		}
	}
	Elem* insert(Elem* e);
	void reArrangeList(int addr);
	void replace(int addr, Data* cont);
	void print();
	void addFreq(int addr){return;};
	int getSize();
};

class LFU: public ReplacementPolicy {
    minHeapEle** minHeapPtr;
    int count;
    int left(int parent);
    int right(int parent);
    int parent(int child);
    void reheapUp(int id);
    void reheapDown(int id);
    Elem* deleteMin();
	public:
	LFU(){
        minHeapPtr = new minHeapEle*[MAXSIZE];
        count = 0;
    }
	~LFU(){
		for(int i=0; i<this->count; i++){
			delete minHeapPtr[i];
		}
	}
	Elem* insert(Elem* e);
	void replace(int addr, Data* cont);
	void reArrangeList(int addr){return;};
	void addFreq(int addr);
	void print();
	int getSize();
};

class DBHashing: public SearchEngine {
	private:
		int (*h1)(int);
		int (*h2)(int);
		STATUS_TYPE* status;
		Elem** data;
		int size;
	public:
	DBHashing(int (*h1)(int),int (*h2)(int),int s){
		this->h1 = h1;
		this->h2 = h2;
		this->size = s;
		this->data = new Elem*[s];
		this->status = new STATUS_TYPE[s];
		for (int i = 0; i < s; i++) {
            this->status[i] = NIL;
		}
	}
	~DBHashing(){
		delete this->status;
		for(int i=0; i<this->size; i++){
			delete this->data[i];
		}
	}
	void insert(Elem *&value);
	void deleteNode(Elem *&value);
	void print();
	Elem* searchAdrr(int addr);
};

class AVL : public SearchEngine {
private:
    avl_node *root;
protected:
	int height(avl_node *&);
	int diff(avl_node *&);
	avl_node *rr_rotation(avl_node *&);
	avl_node *ll_rotation(avl_node *&);
	avl_node *lr_rotation(avl_node *&);
	avl_node *rl_rotation(avl_node *&);
	avl_node* balance(avl_node *&);
	avl_node* insertRec(avl_node *&, Elem *&ele);
	void inorder(avl_node *&);
	void preorder(avl_node *&);
	void postorder(avl_node *&);
	avl_node* remove(avl_node*& t, Elem *&ele);
	avl_node* findMin(avl_node*&);
	avl_node* findMax(avl_node*&);
	avl_node* searchNode(avl_node*&, int addr);	
	void treeDestructor(avl_node *&node);//delete avl node
	public:
		AVL(){
			this->root = NULL;
		}
		~AVL();
		void insert(Elem *&value);
		void deleteNode(Elem *&value);
		Elem* searchAdrr(int addr);
		void print();
		void printInorder();
		void printPreorder();
		void printPostorder();
};

#endif