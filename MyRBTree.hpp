#ifndef __PROJECT_TWO_RBTREE_HPP
#define __PROJECT_TWO_RBTREE_HPP

#include "runtimeexcept.hpp"
#include <string>
#include <vector>

class ElementNotFoundException : public RuntimeException 
{
public:
	ElementNotFoundException(const std::string & err) : RuntimeException(err) {}
};

enum Color { RED, BLACK };
template<typename Key, typename Value>
struct Node{
	bool color;
	Key key;
	Value val;
	Node<Key, Value> *left, *right, *parent;
};

template<typename Key, typename Value>
class MyRBTree
{
private:
	// fill in private member data here
	// If you need to declare private functions, do so here too.
	Node<Key, Value> *Root = nullptr;
	size_t Size = 0;
public:
	MyRBTree();

	// In general, a copy constructor and assignment operator
	// are good things to have.
	// For this quarter, I am not requiring these. 


	// The destructor is, however, required. 
	~MyRBTree()
	{
		Clear(Root);
	}

	// size() returns the number of distinct keys in the tree.
	size_t size() const noexcept;

	// isEmpty() returns true if and only if the tree has no values in it. 
	bool isEmpty() const noexcept;

	// contains() returns true if and only if there
	//  is a (key, value) pair in the tree
	//	that has the given key as its key.
	bool contains(const Key & k) const; 

	// find returns the value associated with the given key
	// If !contains(k), this will throw an ElementNotFoundException
	// There needs to be a version for const and non-const trees.
	Value & find(const Key & k);
	const Value & find(const Key & k) const;

	// Inserts the given key-value pair into 
	// the tree and performs the Red-Black re-balance
	// operation, as described in lecture. 
	// If the key already exists in the tree, 
	// you may do as you please (no test cases in
	// the grading script will deal with this situation)
	void insert(const Key & k, const Value & v);

	// returns all keys in the tree that are between k1 and k2, inclusive.
	// You must return these *in order*.
	// Your running time should be O(log n + s), where s is the number of elements returned.
	std::vector<Key> reportAllInRange(const Key & k1, const Key & k2) const;


	// in general, a "remove" function would be here.
	// You would need it for line segment intersection,
	// but in Spring 2021, we aren't requiring it.
	
	// The following three functions all return
	// the set of keys in the tree as a standard vector.
	// Each returns them in a different order.
	std::vector<Key> inOrder() const;
	std::vector<Key> preOrder() const;
	std::vector<Key> postOrder() const;

	Node<Key,Value>* InitNode(const Key& k, const Value& v);
	Node<Key,Value>* RBinsert(Node<Key,Value> *root, Node<Key,Value> *node);
	void Balance(Node<Key,Value>*& root,Node<Key,Value>*& node);
	void LeftRotate(Node<Key,Value>*& root, Node<Key,Value>*& node);
	void RightRotate(Node<Key,Value>*& root, Node<Key,Value>*& node);
	void InOrder(Node<Key,Value> *root, std::vector<Key> &traversal) const;
	void PreOrder(Node<Key,Value> *root, std::vector<Key>& traversal) const;
	void PostOrder(Node<Key,Value> *root, std::vector<Key>& traversal) const;
	void Clear(Node<Key,Value> *root);
	void getRange(Node<Key,Value> *root, std::vector<Key>& result, const Key & k1, const Key & k2) const;

};


// Many functions below are stubbed;  you should fix them.
template<typename Key, typename Value>
Node<Key,Value>* MyRBTree<Key, Value>::InitNode(const Key& k, const Value& v)
{
	Node<Key,Value> *node = new Node<Key,Value>;
	node->key = k;
	node->val = v;
	node->color = RED;
	node->right = nullptr;
	node->left = nullptr;
	node->parent = nullptr;
	return node;
}

template<typename Key, typename Value>
MyRBTree<Key,Value>::MyRBTree()
{
}

template<typename Key, typename Value>
size_t MyRBTree<Key, Value>::size() const noexcept
{
	return Size;
}

template<typename Key, typename Value>
bool MyRBTree<Key, Value>::isEmpty() const noexcept
{
	return Size == 0;
}


template<typename Key, typename Value>
bool MyRBTree<Key, Value>::contains(const Key &k) const
{
	Node<Key,Value> *root = Root;
	while(root != nullptr && root->key != k)
	{
		if(k > root->key)
		root = root->right;
		else
		root = root->left;
	}
	return root != nullptr;
}


// needs to throw an ElementNotFoundException if it isn't there. 
template<typename Key, typename Value>
Value & MyRBTree<Key, Value>::find(const Key & k)
{
	if(!contains(k))
	throw ElementNotFoundException("Key not found!");
	// NOTE:  do not catch this within this function!  
	//		If you need to throw, throw it.
	Node<Key,Value> *root = Root;
	while(root != nullptr && root->key != k)
	{
		if(k > root->key)
		root = root->right;
		else
		root = root->left;
	}
	return root->val;
}

template<typename Key, typename Value>
const Value & MyRBTree<Key, Value>::find(const Key & k) const
{
	if(!contains(k))
	throw ElementNotFoundException("Key not found!");

	Node<Key,Value> *root = Root;
	while(root != nullptr && root->key != k)
	{
		if(k > root->key)
		root = root->right;
		else
		root = root->left;
	}
	return root->val;
}

template<typename Key, typename Value>
void MyRBTree<Key, Value>::insert(const Key & k, const Value & v)
{
	Size++;
	Node<Key, Value> *node = InitNode(k,v);
	Root = RBinsert(Root, node);
	Balance(Root, node);
}

template<typename Key, typename Value>
Node<Key,Value>* MyRBTree<Key, Value>::RBinsert(Node<Key,Value> *root,Node<Key,Value> *node)
{
	if(root == nullptr)
	{
		return node;
	}

	if(node->key < root->key)
	{
		root->left = RBinsert(root->left, node);
		root->left->parent = root;
	} else if(node->key > root->key)
	{
		root->right = RBinsert(root->right, node);
		root->right->parent = root;
	}
	return root;
}

template<typename Key, typename Value>
void MyRBTree<Key, Value>::Balance(Node<Key,Value>*& root,Node<Key,Value>*& node)
{
	Node<Key,Value> *parentNode = nullptr;
	Node<Key,Value> *grandParent = nullptr;
	while ((node != root) && (node->color != BLACK) && (node->parent->color == RED))
	{
		parentNode = node->parent;
		grandParent = node->parent->parent;

		if(parentNode == grandParent->left)
		{
			Node<Key,Value> *uncle = grandParent->right;

			//if uncle is RED
			if(uncle != nullptr && uncle->color == RED)
			{
				grandParent->color = RED;
				parentNode->color = BLACK;
				uncle->color = BLACK;
				node = grandParent;
			}
			else
			{
				if(node == parentNode->right)
				{
					LeftRotate(root,parentNode);
					node = parentNode;
					parentNode = node->parent;
				}

				RightRotate(root,grandParent);
				std::swap(parentNode->color, grandParent->color);
				node = parentNode;
			}
		}
		else
		{
			Node<Key,Value> * uncle = grandParent->left;
			if((uncle != nullptr) && (uncle->color == RED))
			{
				grandParent->color = RED;
				parentNode->color = BLACK;
				uncle->color = BLACK;
				node = grandParent;
			}
			else
			{
				if (node == parentNode->left)
				{
					RightRotate(root,parentNode);
					node = parentNode;
					parentNode = node->parent;
				}

				LeftRotate(root,grandParent);
				std::swap(parentNode->color, grandParent->color);
				node = parentNode;
			}
		}// if left or right
	}//While ends
	
	root->color = BLACK;
}

template<typename Key, typename Value>
void MyRBTree<Key, Value>::RightRotate(Node<Key,Value>*& root, Node<Key,Value>*& node)
{
	Node<Key,Value> *parent = node->left;
	node->left = parent->right;
	if(node->left != nullptr)
	{
		node->left->parent = node;
	}
	parent->parent = node->parent;
	if(node->parent == nullptr)
	{
		root = parent;
	}
	else if(node == node->parent->left)
	{
		node->parent->left = parent;
	}
	else
	{
		node->parent->right = parent;
	}
	parent->right = node;
	node->parent = parent;	
}

template<typename Key, typename Value>
void MyRBTree<Key, Value>::LeftRotate(Node<Key,Value>*& root, Node<Key,Value>*& node)
{
	Node<Key,Value> *parent = node->right;
	node->right = parent->left;
	if(node->right != nullptr)
	node->right->parent = node;

	parent->parent = node->parent;
	if(node->parent == nullptr)
	root = parent;
	else if (node == node->parent->left)
	node->parent->left = parent;
	else
	{
		node->parent->right = parent;
	}

	parent->left = node;
	node->parent = parent;
}


template<typename Key, typename Value>
std::vector<Key> MyRBTree<Key, Value>::reportAllInRange(const Key & k1, const Key & k2) const
{
	std::vector<Key> result;
	Node<Key,Value> *root = Root;
	while(root != nullptr && (root->key < k1 || root->key > k2))
	{
		if(root->key < k1)
		root = root->right;
		else if(root->key > k2)
		root = root->left;
	}
	getRange(root, result, k1, k2);
	return result; 
}
template<typename Key, typename Value>
void MyRBTree<Key, Value>::getRange(Node<Key,Value> *root, std::vector<Key>& result, const Key & k1, const Key & k2) const
{
	if(root == nullptr)
	return;

	if(root->key > k1)
	getRange(root->left, result,k1,k2);
	if(root->key >= k1 && root->key <= k2)
	result.push_back(root->key);
	if(root->key < k2)
	getRange(root->right,result,k1,k2);
}


template<typename Key, typename Value>
std::vector<Key> MyRBTree<Key, Value>::inOrder() const
{
	std::vector<Key> traversal;
	Node<Key,Value> *root = Root;
	InOrder(root,traversal);
	return traversal; 
}

template<typename Key, typename Value>
void MyRBTree<Key, Value>::InOrder(Node<Key,Value> *root, std::vector<Key>& traversal) const
{
	if(root == nullptr)
	return;
	InOrder(root->left, traversal);
	traversal.push_back(root->key);
	InOrder(root->right, traversal);
}


template<typename Key, typename Value>
std::vector<Key> MyRBTree<Key, Value>::preOrder() const
{
	std::vector<Key> traversal;
	Node<Key,Value> *root = Root;
	PreOrder(root,traversal);
	return traversal; 
}
template<typename Key, typename Value>
void MyRBTree<Key, Value>::PreOrder(Node<Key,Value> *root, std::vector<Key>& traversal) const
{
	if(root == nullptr)
	return;
	traversal.push_back(root->key);
	PreOrder(root->left, traversal);
	PreOrder(root->right, traversal);
}

template<typename Key, typename Value>
std::vector<Key> MyRBTree<Key, Value>::postOrder() const
{
	std::vector<Key> traversal;
	Node<Key,Value> *root = Root;
	PostOrder(root,traversal);
	return traversal;  
}
template<typename Key, typename Value>
void MyRBTree<Key, Value>::PostOrder(Node<Key,Value> *root, std::vector<Key>& traversal) const
{
	if(root == nullptr)
	return;
	PostOrder(root->left, traversal);
	PostOrder(root->right, traversal);
	traversal.push_back(root->key);
}

template<typename Key, typename Value>
void MyRBTree<Key, Value>::Clear(Node<Key,Value> *root )
{
	if(root == nullptr)
	return;
	Clear(root->left);
	Clear(root->right);
	root->parent = nullptr;
	delete(root);
	root = nullptr;
}

#endif 