#include "main.h"
#include "Cache.h"
Cache::Cache(SearchEngine* s,ReplacementPolicy* r):rp(r),s_engine(s) {}
Cache::~Cache(){}
//Heap implemtation func
int LFU::getSize()
{
    //return heap.size();
    return this->count;
}

Elem* LFU::insert(Elem* ele){
    minHeapEle* tmpNew = new minHeapEle(ele, 1);
    if(this->count >= MAXSIZE){
        Elem* tmp = this->deleteMin();
        minHeapPtr[count-1] = tmpNew;
        reheapUp(count-1);
        return tmp;
    }
    else{
        minHeapPtr[count] = tmpNew;
        reheapUp(count);
        this->count++;
        return NULL;
    }
}
/*
 * Delete Minimum Element
 */
Elem* LFU::deleteMin(){
    if(this->count == 0) return NULL;
    minHeapEle* tmp = minHeapPtr[0]; 
    minHeapPtr[0] = minHeapPtr[this->count-1];
    //this->count--;
    reheapDown(0);
    return tmp->ele;
}

/*
 * Return Left Child
 */
int LFU::left(int parent)
{
    int l = 2 * parent + 1;
    if(l < this->count)
        return l;
    else
        return -1;
}
 
/*
 * Return Right Child
 */
int LFU::right(int parent)
{
    int r = 2 * parent + 2;
    if(r < this->count)
        return r;
    else
        return -1;
}
 
/*
 * Return Parent
 */
int LFU::parent(int child)
{
    int p = (child - 1)/2;
    if(child == 0)
        return -1;
    else
        return p;
}
 
/*
 * Heapify- Maintain Heap Structure bottom up
 */
void LFU::reheapUp(int id){
    if(id >= 0 && parent(id) >= 0 && minHeapPtr[parent(id)]->freq > minHeapPtr[id]->freq){
        minHeapEle* tmp = minHeapPtr[id];
        minHeapPtr[id] = minHeapPtr[parent(id)];
        minHeapPtr[parent(id)] = tmp;
        reheapUp(parent(id));
    }
}

/*
 * Heapify- Maintain Heap Structure top down
 */

void LFU::reheapDown(int id){
    int child = left(id);
    int child1 = right(id);
    if(child >= 0 && child1 >= 0 && minHeapPtr[child]->freq > minHeapPtr[child1]->freq){
        child = child1;
    }
    if(child > 0){
        if(minHeapPtr[id]->freq >= minHeapPtr[child]->freq){
            minHeapEle* tmp1 = minHeapPtr[id];
            minHeapPtr[id] = minHeapPtr[child];
            minHeapPtr[child] = tmp1;
            reheapDown(child);
        }
    }
}


void LFU::addFreq(int addr){
	for(int i=0; i<this->count;i++){
		if(minHeapPtr[i]->ele->addr == addr){
			minHeapPtr[i]->freq += 1;
            reheapDown(i);
            return;
        }
	}
}

void LFU::replace(int addr, Data* cont){
	for(int i=0; i<this->count;i++){
		if(minHeapPtr[i]->ele->addr == addr){
			minHeapPtr[i]->ele->data = cont;
			minHeapPtr[i]->ele->sync =  false;
		}			
	}
}

void LFU::print(){
	for(int i=0; i<this->count;i++){
		// cout << minHeapPtr[i]->ele->addr << " " << minHeapPtr[i]->ele->data->getValue() << " " << (minHeapPtr[i]->ele->sync?"true":"false") << endl;
		this->minHeapPtr[i]->ele->print();
	}
}

Elem* MRU::insert(Elem* ele){
		selfListNode* tmp = new selfListNode(ele);
		if(this->count == 0){
			head = tail = tmp;
			this->count++;
			return NULL;
		}
		else if(this->count >= MAXSIZE){
			selfListNode* ptr = head;
			tmp->next = head->next;
			head = tmp;
			ptr->next = NULL;
			return ptr->elem;
		}
		else{
			tmp->next = head;
			head = tmp;
			this->count++;
			return NULL;
		}
}

void MRU::reArrangeList(int addr){
		selfListNode* curr = head;
		selfListNode* pre = NULL;
		while(curr != NULL){
			if(curr->elem->addr == addr){
				if(pre != NULL){
					if(curr->next == NULL){
						tail = pre;
						pre->next = NULL;
						curr->next = head;
						head = curr;
					}
					else{
						pre->next = curr->next;
						curr->next = head;
						head = curr;
					}
				}
				return;
			}
			pre = curr;
			curr = curr->next;
		}
		return;
}

void MRU::print(){
		selfListNode* ptr = head;
		while(ptr != NULL){
			// cout << ptr->elem->addr << " " << ptr->elem->data->getValue() << " " << (ptr->elem->sync?"true":"false") << endl;
			ptr->elem->print();
			ptr = ptr->next;
		}
}

void MRU::replace(int addr, Data* cont){
		selfListNode* ptr = head;
		while(ptr != NULL){
			if(ptr->elem->addr == addr){
				ptr->elem->data = cont;
				ptr->elem->sync = false;
			}
			ptr = ptr->next;
		}
}

int MRU::getSize(){return this->count;}


Elem* LRU::insert(Elem* ele){
		selfListNode* tmp = new selfListNode(ele);
		if(this->count == 0){
			head = tail = tmp;
			this->count++;
			return NULL;
		}
		else if(this->count >= MAXSIZE){
			selfListNode* ptr = tail;
			selfListNode* pre = head;
			tmp->next = head;
			head = tmp;
			for(int i=0; i<MAXSIZE-2; i++)
				pre = pre->next;
			pre->next = NULL;
			tail = pre;
			return ptr->elem;
		}
		else{
			tmp->next = head;
			head = tmp;
			this->count++;
			return NULL;
		}
}

void LRU::reArrangeList(int addr){
		selfListNode* curr = head;
		selfListNode* pre = NULL;
		while(curr != NULL){
			if(curr->elem->addr == addr){
				if(pre != NULL){
					if(curr->next == NULL){
						tail = pre;
						pre->next = NULL;
						curr->next = head;
						head = curr;
					}
					else{
						pre->next = curr->next;
						curr->next = head;
						head = curr;
					}
				}
				return;
			}
			pre = curr;
			curr = curr->next;
		}
		return;
}

void LRU::print(){
		selfListNode* ptr = head;
		while(ptr != NULL){
			// cout << ptr->elem->addr << " " << ptr->elem->data->getValue() << " " << (ptr->elem->sync?"true":"false") << endl;
			ptr->elem->print();
			ptr = ptr->next;
		}
}

void LRU::replace(int addr, Data* cont){
		selfListNode* ptr = head;
		while(ptr != NULL){
			if(ptr->elem->addr == addr){
				ptr->elem->data = cont;
				ptr->elem->sync = false;
			}
			ptr = ptr->next;
		}
}

int LRU::getSize(){return this->count;}

int AVL::height(avl_node *&temp)
{
	int h = 0;
	if (temp != NULL)
	{
		int l_height = height(temp->left);
		int r_height = height(temp->right);
		int max_height = max(l_height, r_height);
		h = max_height + 1;
	}
	return h;
}

int AVL::diff(avl_node *&temp)
{
	int l_height = height(temp->left);
	int r_height = height(temp->right);
	int b_factor = l_height - r_height;
	return b_factor;
}

avl_node *AVL::rr_rotation(avl_node *&parent)
{
	avl_node *temp;
	temp = parent->right;
	parent->right = temp->left;
	temp->left = parent;
	return temp;
}

avl_node *AVL::ll_rotation(avl_node *&parent)
{
	avl_node *temp;
	temp = parent->left;
	parent->left = temp->right;
	temp->right = parent;
	return temp;
}

avl_node *AVL::lr_rotation(avl_node *&parent)
{
	avl_node *temp;
	temp = parent->left;
	parent->left = rr_rotation(temp);
	return ll_rotation(parent);
}

avl_node *AVL::rl_rotation(avl_node *&parent)
{
	avl_node *temp;
	temp = parent->right;
	parent->right = ll_rotation(temp);
	return rr_rotation(parent);
}

avl_node *AVL::balance(avl_node *&temp)
{
	int bal_factor = diff(temp);
	if (bal_factor > 1)
	{
		if (diff(temp->left) > 0)
			temp = ll_rotation(temp);
		else
			temp = lr_rotation(temp);
	}
	else if (bal_factor < -1)
	{
		if (diff(temp->right) > 0)
			temp = rl_rotation(temp);
		else
			temp = rr_rotation(temp);
	}
	return temp;
}

avl_node *AVL::insertRec(avl_node *&root, Elem *&ele)
{
	if (root == NULL)
	{   
		avl_node* node = new avl_node(ele);
		root = node;
		return root;
	}
	else if (ele->addr < root->data->addr)
	{
		root->left = insertRec(root->left, ele);
		root = balance(root);
	}
	else if (ele->addr >= root->data->addr)
	{
		root->right = insertRec(root->right, ele);
		root = balance(root);
	}
	return root;
}

void AVL::preorder(avl_node *&tree)
{
	if (tree == NULL)
		return;
	cout << tree->data->addr << " " << tree->data->data->getValue() << " "
    << (tree->data->sync?"true":"false") << endl;
	preorder(tree->left);
	preorder(tree->right);
}

void AVL::inorder(avl_node *&tree)
{
	if (tree == NULL)
		return;
	inorder(tree->left);
	cout << tree->data->addr << " " << tree->data->data->getValue() << " "
    << (tree->data->sync?"true":"false") << endl;
	inorder(tree->right);
}

void AVL::postorder(avl_node *&tree)
{
	if (tree == NULL)
		return;
	postorder(tree->left);
	postorder(tree->right);
	cout << tree->data->addr << " " << tree->data->data->getValue() << " "
    << (tree->data->sync?"true":"false") << endl;
}


avl_node* AVL::remove(avl_node*& node, Elem *&ele){
	avl_node* temp;

	if (node == NULL) return NULL;

	else if (ele->addr < node->data->addr) node->left = remove(node->left, ele);
	else if (ele->addr > node->data->addr) node->right = remove(node->right, ele);

	else if (node->left && node->right) {
		temp = findMin(node->right);
		node->data = temp->data;
		node->right = remove(node->right, node->data);
	}

	else {
		temp = node;
		if (node->left == NULL) node = node->right;
		else if (node->right == NULL) node = node->left;
		delete temp;
	}
	if (node == NULL) return node;
	node = balance(node);
    return node;	
}

avl_node* AVL::findMin(avl_node*& t) {
	if (t == NULL) return NULL;
	else if (t->left == NULL) return t;
	else return findMin(t->left);
}
avl_node* AVL:: findMax(avl_node*& t) {
	if (t == NULL) return NULL;
	else if (t->right == NULL) return t;
	else return findMax(t->right);
}

void AVL::insert(Elem *&value)
{	
    insertRec(this->root, value);
}

void AVL::deleteNode(Elem *&value){
	remove(this->root, value);
}

avl_node* AVL::searchNode(avl_node *&node, int addr){
    if(node == NULL)
        return NULL;
    if(addr < node->data->addr)
        return searchNode(node->left, addr);
    else if(addr > node->data->addr)
        return searchNode(node->right, addr);
    else{
        return node;
        }
}

Elem* AVL::searchAdrr(int addr){
	if(searchNode(this->root, addr) == NULL)
		return NULL;
	else
    	return searchNode(this->root, addr)->data;
}

void AVL::printInorder(){
	inorder(this->root);
}

void AVL::printPreorder(){
	preorder(this->root);
}

void AVL::printPostorder(){
	postorder(this->root);
}

void AVL::print(){
	cout << "Print AVL in inorder:\n";
	printInorder();
	cout << "Print AVL in preorder:\n";
	printPreorder();
}

void AVL::treeDestructor(avl_node *&node){
	if(node){
		//cout << "~node destructor" << endl;
		treeDestructor(node->left);
		treeDestructor(node->right);
		delete node;
	}
}
AVL::~AVL(){
	treeDestructor(root);
}

void DBHashing::insert(Elem *&value){
	int key = value->addr;
	int i = 0;
	while(i < this->size){
		int j = (this->h1(key) + this->h2(key)*i)%this->size;
        if(this->status[j] == NIL || this->status[j] == DELETED){
            this->data[j] = value;
            status[j] = NON_EMPTY;
            return;
        }
        else
            i += 1;		
	}
}

Elem* DBHashing::searchAdrr(int val){
	int i = 0;
	int j;
	do{
		j = (this->h1(val) + this->h2(val)*i)%(this->size);
		//cout << j;
		if(this->status[j] == NON_EMPTY){
			if(this->data[j]->addr == val)
				return this->data[j];
		}
		i++;		
	}while(this->status[j] != NIL && i<this->size);
	return NULL;
}

void DBHashing::deleteNode(Elem *&value){
	Elem* pos = this->searchAdrr(value->addr);
	if(pos != NULL){
		for(int i=0; i<this->size; i++){
			if(this->status[i] == NON_EMPTY){
				if(this->data[i]->addr == value->addr){
					this->status[i] = DELETED;
					return;
				}
			}
		}
	}
	else
		return;
}

void DBHashing::print(){
	cout << "Prime memory:\n";
	for(int i=0; i<this->size; i++){
		if(this->status[i] == NON_EMPTY){
			this->data[i]->print();
		}
	}
}

Elem* FIFO::insert(Elem* e){
    if(this->count >= MAXSIZE){
        //shift all elements to the left by 1
        Elem* ptr = arr[0];
		for(int i=0; i <= MAXSIZE-2; i++){
            arr[i] = arr[i+1];
        }
        arr[MAXSIZE-1] = e;
		return ptr;
    }
    else{
        this->arr[count] = e;
        this->count++;
		return NULL;
	}
}

void FIFO::print(){
    for(int i=0; i<this->count; i++){
        this->arr[i]->print();
    }
}

void FIFO::replace(int addr, Data* cont){
	for(int i=0; i<this->count; i++){
		if(this->arr[i]->addr == addr){
			this->arr[i]->data =  cont;
			this->arr[i]->sync =  false;
		}			
	}
}

int FIFO::getSize(){
    return this->count;
}

Data* Cache::read(int addr){
	Elem* ptr = this->s_engine->searchAdrr(addr);
	if(ptr == NULL){
        return NULL;
    }
	else{
		this->rp->reArrangeList(addr);
		this->rp->addFreq(addr);
		return ptr->data;
	}
}

Elem* Cache::put(int addr, Data* cont) {
	Elem* ele = new Elem(addr, cont, true);
	Elem* findNode = this->s_engine->searchAdrr(addr);
	if(findNode != NULL)
		findNode->data = cont;
	else{
		if(rp->getSize() > MAXSIZE - 1){
			Elem* tmpDlt = this->rp->insert(ele);
			this->s_engine->deleteNode(tmpDlt);
			this->s_engine->insert(ele);
			return tmpDlt;
		}
		else{
			this->s_engine->insert(ele);
			this->rp->insert(ele);
			//this->rp->count++;
			return NULL;
		}
	}
    return NULL;
}

Elem* Cache::write(int addr, Data* cont) {
	Elem* findNode = this->s_engine->searchAdrr(addr);
	if(findNode != NULL){
		findNode->data = cont;
		findNode->sync = false;
        this->rp->addFreq(addr);
		this->rp->reArrangeList(addr);
		this->rp->replace(addr, cont);
	}
	else{
		Elem* ele = new Elem(addr, cont, false);
		if(this->rp->getSize() > MAXSIZE - 1){
			Elem* tmpDlt = this->rp->insert(ele);
			this->s_engine->deleteNode(tmpDlt);
			this->s_engine->insert(ele);
			return tmpDlt;
		}
		else{
			this->s_engine->insert(ele);
			this->rp->insert(ele);
			//this->rp->count++;
			return NULL;
		}
	}
    return NULL;
}
void Cache::printRP() {
	this->rp->print();
}
void Cache::printSE() {
	this->s_engine->print();
}