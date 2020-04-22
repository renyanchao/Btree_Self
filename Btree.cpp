/* github test */
/* gitgub test2 */
/* github test3 by branch */
#include<iostream>
#include <assert.h>
#include <stack>

#define RED_BLACK_TREE
//#define DEBUG

//Binary Sort Tree
template<typename N>
struct BTreeNode {
	BTreeNode*	left;

	BTreeNode*	right;
	
	BTreeNode*	parent;

	N	n;

	enum COLOR
	{
		RED,
		BLACK
	};
	COLOR color;

	BTreeNode(N _n) :n(_n), left(0), right(0), parent(0) {
#ifdef RED_BLACK_TREE
		color = BTreeNode<N>::COLOR::RED;
#endif
	}

};



template<typename N>
class BinaryTreeSort {
public:
	BinaryTreeSort():m_root(0){
		
	}
	BinaryTreeSort(BTreeNode<N>* node):m_root(node) {
		
	}
	~BinaryTreeSort() {}

	bool _insert(BTreeNode<N>* node) {
		if (!node)return false;
		
		if (!m_root) {
			m_root = node;
			return true;
		}

		BTreeNode<N>* insertPos = m_root;
		BTreeNode<N>* parent = NULL;

		while (insertPos) {
			parent = insertPos;

			if (node->n < insertPos->n) {
				insertPos = insertPos->left;
			}
			else if (node->n > insertPos->n) {
				insertPos = insertPos->right;
			}
			else {
				return false;
			}
		}

		if(parent) {
			if (node->n < parent->n) {
				parent->left = node;
			}
			else if (node->n > parent->n) {
				parent->right = node;
			}
			else {
				return false;
			}
		}
		
		node->parent = parent;
		node->left = node->right = NULL;
		
		return true;
	}

	bool _delete(BTreeNode<N> *node){
		if (!node)return false;
		
		if (node->left == NULL && node->right == NULL) {
			auto parent = node->parent;
			
			if (parent) {
				BTreeNode<N> ** ptr = (parent->left == node) ? &(parent->left) : &(parent->right);
				*ptr = NULL;
			}

			node->parent = NULL;

			if (node == getRoot()) {
				setRoot(NULL);
			}

			return true;
		}

		if (node->left != NULL && node->right != NULL) {
			
			auto next = findMinimum(node);
			node->n = next->n;

			return _delete(next);

			
		}

		if (node->left != NULL || node->right!= NULL) {

			
			auto child = node->left != NULL? node->left:node->right;

			auto parent = node->parent;
			if (parent) {
				BTreeNode<N> **ptr = (parent->left == node) ? &(parent->left) : &(parent->right);
				*ptr = child;	
			}
			child->parent = parent;
			node->parent = NULL;
			

			node->left = node->right = NULL;
			if (node == getRoot()) {
				setRoot(child);
			}
			return true;

		}
	}

	BTreeNode<N> * _find(N n) {
		if (!m_root) { return NULL; }
		BTreeNode<N>	*root = m_root;

		while (root){
			if (n < root->n) {
				root = root->left;
			}
			else if (n > root->n) {
				root = root->right;
			}
			else {
				return root;
			}

		}
		return NULL;
	}

	void PrintAll(BTreeNode<N> * root) {
		
		auto PrintPtr = [&](BTreeNode<N> *ptr) {
			if (ptr) {
				std::cout << ptr->n << "\t";
			}
			else
			{
				std::cout << "NULL\t";
			}
		};
		
		if (!root) return;
		//printNode(root);

		if (!m_root) {
			std::cout << "NULL" << std::endl;
			return;
		}

		std::cout << "node`s value:	" << root->n << "\t" << "node`s leftPtr:	";
		PrintPtr(root->left); 
		std::cout << "node`s rightPtr:	"; 
		PrintPtr(root->right);
#ifdef RED_BLACK_TREE
		auto str = root->color == BTreeNode<N>::RED ? "RED" : "BLACK";
		std::cout << "\tnode`s color:	" << str;
#endif
		
		assert(checkParent(root));
		std::cout<<std::endl;



		PrintAll(root->left);
		PrintAll(root->right);




	}

	BTreeNode<N>* getRoot() {
		return m_root;
	}

	void setRoot(BTreeNode<N>* root) {
		m_root = root;
	}

	void outPutMessageInsert(N n) {
		_insert(new BTreeNode<N>(n));

#ifdef DEBUG

		std::cout << "\n\nafter _insert\t" << n << "\n";
		PrintAll(m_root);

#endif DEBUG
		
	}

	void outPutMessageDelete(N n) {
		_delete(_find(n));
	
#ifdef DEBUG

		std::cout << "\n\nafter _delete\t" << n << "\n";
		PrintAll(m_root);

#endif DEBUG
	}


protected:

	bool checkParent(BTreeNode<N> * root) {
		if (root->parent) {
			if (root != root->parent->left && root != root->parent->right) {
				return false;
			}
		}
		return true;
	}


	//左旋 以当前（失衡）节点为根，根的右子节点变成新根，原来的根退成新根的左子节点。现根的原左子节点变为原根的右子节点
	bool rotate_left(BTreeNode<N> * node) {
		auto oldRoot = node;
		auto newRoot = node->right;
		auto parent = node->parent;

		assert(oldRoot);
		assert(newRoot);
	
		oldRoot->right = newRoot->left;
		if(newRoot->left) newRoot->left->parent = oldRoot;
		newRoot->left = oldRoot;
		

		if (parent) {
			BTreeNode<N> ** ptr = (parent->left == node) ? &(parent->left) : &(parent->right);
			*ptr = newRoot;
		}
		newRoot->parent = parent;
		oldRoot->parent = newRoot;

		if (oldRoot == getRoot()) {
			setRoot(newRoot);
		}
		return true;

	}

	//右旋 以当前（失衡）节点为根，根的左子节点变成新根，原来的根退成新根的右子节点。现根的原右子节点变为原根的左子节点
	bool rotate_right(BTreeNode<N> * node) {

		auto oldRoot = node;
		auto newRoot = node->left;
		auto parent = node->parent;

		oldRoot->left = newRoot->right;
		if(newRoot->right)newRoot->right->parent = oldRoot;
		newRoot->right = oldRoot;
		if (parent) {
			BTreeNode<N> ** ptr = (parent->left == node) ? &(parent->left) : &(parent->right);
			*ptr = newRoot;
		}
		newRoot->parent = parent;
		oldRoot->parent = newRoot;

		if (oldRoot == getRoot()) {
			setRoot(newRoot);
		}

		return true;
	}

	BTreeNode<N> *findMinimum(BTreeNode<N> *preNode) {	//定义为找到该节点的后继节点，但是本例中，只有当左右子树均不为空的时候，才会调用。其他情况用不到此函数
		if (!preNode) return NULL;			
		if (!preNode->right) {						//右子树为空
			if (!preNode->parent)return NULL;		//没父亲

			auto parents = preNode->parent;
			while (parents) {
				if (parents->n > preNode->n) {
					return parents;					//一直往上遍历到根
				}
				parents = parents->parent;

			}
			return NULL;
		}
		else {
			BTreeNode<N> *minimum = preNode->right;
			while (minimum) {
				if (minimum->left) {
					minimum = minimum->left;
				}
				else {
					return minimum;
				}
			}
		}
	}


private:
	BTreeNode<N> *m_root;

	

};

template<typename N>
class AVLTree:public BinaryTreeSort<N>{
public:
	AVLTree() {}
	~AVLTree() {}

	bool _insert(BTreeNode<N> *node) {
		if (!node)return false;
		bool ret =  BinaryTreeSort<N>::_insert(node);
		if (!ret)return ret;

		adjustBalance(node);	//向上寻找失衡点，然后调整
		return true;
	}

	bool _delete(BTreeNode<N> * node) {
		if (!node)return false;
		auto parent = node->parent;
		bool ret = BinaryTreeSort<N>::_delete(node);
		if (!ret)return ret;
		if (parent) {
			adjustBalance(parent);
		}

		
		return  true;
		
	}

	void PrintAll(BTreeNode<N>* root) {
		return BinaryTreeSort<N>::PrintAll(root);
	}
	BTreeNode<N> * _find(N n) {
		return BinaryTreeSort<N>::_find(n);
	}

	BTreeNode<N>* getRoot() {
		return BinaryTreeSort<N>::getRoot();
	}

	void setRoot(BTreeNode<N> * root) {
		BinaryTreeSort<N>::setRoot(root);
	}

	void outPutMessageInsert(N n) {
		_insert(new BTreeNode<N>(n));
#ifdef DEBUG
		auto root = BinaryTreeSort::getRoot();
		std::cout << "\n\nafter _delete\t" << n << "\n";
		PrintAll(root);

#endif DEBUG


	}
	void outPutMessageDelete(N n) {

		_delete(_find(n));
#ifdef DEBUG
		auto root = BinaryTreeSort::getRoot();
		std::cout << "\n\nafter _delete\t" << n << "\n";
		PrintAll(root);

#endif DEBUG
	}


private:
	//BTreeNode<N> *m_root;

	//检查失衡,从参数指定的节点 向上查找

	BTreeNode<N>* getloseBalanceNodeUP(BTreeNode<N> * node) {
		
		BTreeNode<N> * root = node;
		
		while (root) {
			if (std::abs(static_cast<int>(getNodeHight(root->left)) - static_cast<int>(getNodeHight(root->right))) > 1) {
				return root;
			}
			root = root->parent;
		}

		return NULL;
	}

	size_t getNodeHight(BTreeNode<N> * node) {
		if (!node) return 0;
	
		size_t res = 0;
		res += 1;
		res += getNodeHight(node->left) > getNodeHight(node->right) ? getNodeHight(node->left) : getNodeHight(node->right);
		
		return res;
	}



	enum LoseBalanceType
	{
		Left_Left = 0,
		Left_Right = 1,
		Right_Left = 2,
		Right_Right = 3
	};
	uint32_t getloseBalanceType(BTreeNode<N> * node) {
		
		uint8_t type = 0;
		if (!node)return type;
		auto leftHight =  getNodeHight(node->left);
		auto rightHight = getNodeHight(node->right);
		
		assert(leftHight != rightHight);

		if (leftHight > rightHight) {
			type |= 0;			//left_
			auto leftnode = node->left;
			assert(leftnode);

			if (getNodeHight(leftnode->left) > getNodeHight(leftnode->right)) {
				type |= 0;		//left_left
			}
			else {
				type |= 1;		//left_right
			}
		}
		else {
			type |= 2;			//right_
			auto rightnode = node->right;
			assert(rightnode);

			if (getNodeHight(rightnode->left) >= getNodeHight(rightnode->right)) {				//!!!!!!!!!!这里判断类型时，左右高度相等 怎么判断类型 会影响最后的结果。结果虽然表现方式不同，但遵守平衡
				type |= 2;		//right_left
			}
			else {
				type |= 3;		//right_right
			}
		
		}
		return type;
	}

	void adjustBalance(BTreeNode<N>* node) {
	
		BTreeNode<N>* loseBalanceNode = getloseBalanceNodeUP(node);
		if (loseBalanceNode) {
			auto type = getloseBalanceType(loseBalanceNode);
			switch (type)
			{
			case Left_Left:
				BinaryTreeSort::rotate_right(loseBalanceNode);
				break;
			case Left_Right:
				BinaryTreeSort::rotate_left(loseBalanceNode->left);
				BinaryTreeSort::rotate_right(loseBalanceNode);
				break;
			case Right_Left:
				BinaryTreeSort::rotate_right(loseBalanceNode->right);
				BinaryTreeSort::rotate_left(loseBalanceNode);
				break;
			case Right_Right:
				BinaryTreeSort::rotate_left(loseBalanceNode);
				break;

			default:
				break;
			}
		}
	
	}
};


template<typename T>
class Red_Black_Tree:public BinaryTreeSort<T>{
public:

	typedef		BTreeNode<T>*		NodePtr;
	/*typedef		BTreeNode<T>::COLOR::RED	_RED;
	typedef		BTreeNode<T>::COLOR::BLACK	_BLACK;*/

	Red_Black_Tree<T>() {
		
	}

	Red_Black_Tree<T>(NodePtr node):BinaryTreeSort<T>(node) {
		
	}
	~Red_Black_Tree<T>() {}


	bool _insert(NodePtr node) {
		if(!node)return false;
		assert(node->color == BTreeNode<T>::RED);
		{
			node->left = node->right = node->parent = NULL;
		}

		NodePtr root = BinaryTreeSort::getRoot();

		if (!root) {
			BinaryTreeSort::setRoot(node);
			root = BinaryTreeSort::getRoot();
			root->color = BTreeNode<T>::BLACK;
			return  true;
		}
		
		if (!BinaryTreeSort::_insert(node)) {
			return false;
		}
		NodePtr curNode = node;

		while (curNode) {
			
			if (curNode == root) {
				root->color = BTreeNode<T>::BLACK;
				return true;
			}

			NodePtr parentNode = curNode->parent;
			assert(parentNode);

			if (parentNode->color == BTreeNode<T>::BLACK) {
				return true;
			}

			//parent`s color == RED. must have black-grandfather
			NodePtr grandFatherNode = parentNode->parent;
			assert(grandFatherNode);			

			NodePtr uncleNode = grandFatherNode->left == parentNode ? grandFatherNode->right : grandFatherNode->left;

			if (!uncleNode || uncleNode->color == BTreeNode<T>::BLACK) {

				if (grandFatherNode->left == parentNode && parentNode->left == curNode) {		//loop break;

					parentNode->color = BTreeNode<T>::BLACK;
					grandFatherNode->color = BTreeNode<T>::RED;

					BinaryTreeSort::rotate_right(grandFatherNode);


					break;
				}

				if (grandFatherNode->left == parentNode && parentNode->right == curNode) {
					curNode = curNode->parent;
					BinaryTreeSort::rotate_left(curNode);

					continue;
				}

				if (grandFatherNode->right == parentNode && parentNode->right == curNode) {
					parentNode->color = BTreeNode<T>::BLACK;
					grandFatherNode->color = BTreeNode<T>::RED;
					
					BinaryTreeSort::rotate_left(grandFatherNode);
					break;
				}

				if (grandFatherNode->right == parentNode && parentNode->left == curNode) {
					curNode = curNode->parent;
					BinaryTreeSort::rotate_right(curNode);
					continue;
				}
				assert(false);

			}
			if (parentNode->color == BTreeNode<T>::RED && uncleNode->color == BTreeNode<T>::RED) {
				parentNode->color = BTreeNode<T>::BLACK;
				uncleNode->color = BTreeNode<T>::BLACK;
				grandFatherNode->color = BTreeNode<T>::RED;

				curNode = grandFatherNode;
				continue;
			}
		}
		return true;
	}

	bool _delete(NodePtr root) {
		if (!root)return false;

		bool really_delete = true;

		

		auto isBlackFunc = [&](NodePtr ptr) {//不是红，就算黑
			return (ptr == NULL || ptr->color == BTreeNode<T>::BLACK);
		};
		auto eraseBlackFunc = [&](NodePtr ptr) {//要删除的黑色节点，只能有一支子,而且子必定为红色.或者两子都为空


			auto child = ptr->left != NULL ? ptr->left : ptr->right;

			if (child && ptr->parent) {
				auto ptrParentToSelf = ptr->parent->left == ptr ? &(ptr->parent->left) : &(ptr->parent->right);
				*ptrParentToSelf = child;
				child->parent = ptr->parent;
				
				ptr->parent = NULL;
				*(ptr->left != NULL ? &(ptr->left) : &(ptr->right)) = NULL;
			}
			else if (child && !ptr->parent) {
				BinaryTreeSort::setRoot(child);
				*(ptr->left != NULL ? &(ptr->left) : &(ptr->right)) = NULL;
				child->parent = NULL;

			}
			else if (!child && ptr->parent) {
				auto ptrParentToSelf = ptr->parent->left == ptr ? &(ptr->parent->left) : &(ptr->parent->right);
				ptr->parent = NULL;
				*ptrParentToSelf = NULL;
			}
			
		
		};


		if (root->left != NULL && root->right != NULL) {					//左右都不空   找到后继节点（实际要删除的节点），以后root就指向它。
			auto nextNode = BinaryTreeSort::findMinimum(root);
			assert(nextNode);
			assert(!(nextNode->left != NULL && nextNode->right != NULL));
			root->n = nextNode->n;
			root = nextNode;

		}


		assert( ! (root->left != NULL && root->right != NULL));				//要删除的节点肯定不会两边都非空,至少一边为空
		auto child = root->left != NULL ? root->left : root->right;			//先获取一下子节点，最多有一支

		//如果是红色的 直接删除
		if (root->color == BTreeNode<T>::RED) {								//如果要删除的节点是红色的：那它肯定是个叶子节点。（不会左右一边空，一边非空）
			assert(child == NULL);											//因为一边空，黑色num为0，另一边非空，必有黑色节点。违背性质5
																			//所以如果要删除的节点是红色的，直接删除即可。不需要调整颜色			
			if (root->parent) {
				auto ptr = root->parent->left == root ? &(root->parent->left) : &(root->parent->right);
				*ptr = NULL;
			}
			root->parent = NULL;
			return true;
		}

		//下面  删除的黑色节点

		if (child) {//如果有孩子，直接拿孩子替换	
			assert(child->color == BTreeNode<T>::RED);
			eraseBlackFunc(root);

			root->left = root->right = root->parent = NULL;
			child->color = BTreeNode<T>::BLACK;
			
			return true;
		}

		
		if (root == BinaryTreeSort::getRoot()) {		
				eraseBlackFunc(root);					//如果要删除的节点是根，且整个红黑树只有这一个节点，直接删除。
				BinaryTreeSort::setRoot(NULL);
				return true;
		}

		while (root != BinaryTreeSort::getRoot()) {
			assert(root->parent);
			auto brotherNode = root->parent->left == root ? root->parent->right : root->parent->left;
			assert(brotherNode);	//这里兄弟节点不会为空的。可能是红色且有两个分支，两个分支各有一个黑色子节点结束	//也可能，兄弟节点就是黑色节点，结束
								
			//兄红
			if (brotherNode->color == BTreeNode<T>::RED) {	//兄弟是红的，兄弟的两个儿子肯定是黑的。通过旋转，让兄弟当新根，将新根与旧根的颜色互换，这样新的兄弟是黑色的。且新的父亲是红色的
				//此叶左
				if (root == root->parent->left) {
					root->parent->color = BTreeNode<T>::RED;
					brotherNode->color = BTreeNode<T>::BLACK;
					BinaryTreeSort::rotate_left(root->parent);	
					continue;
				}else if (root == root->parent->right) {
					root->parent->color = BTreeNode<T>::RED;
					brotherNode->color = BTreeNode<T>::BLACK;
					BinaryTreeSort::rotate_right(root->parent);
					continue;
				}
			}
		
			//兄黑 		
			//远侄红
			if (root == root->parent->left && !isBlackFunc(brotherNode->right)) {
					
					auto parentColor = root->parent->color;
					root->parent->color = brotherNode->color;
					brotherNode->color = parentColor;

					BinaryTreeSort::rotate_left(root->parent);
					brotherNode->right->color = BTreeNode<T>::BLACK;

					if (really_delete) {
						eraseBlackFunc(root);
						break;
					}
					break;

						
			}
			else if (root == root->parent->right && !isBlackFunc(brotherNode->left)) {
							
					auto parentColor = root->parent->color;
					root->parent->color = brotherNode->color;
					brotherNode->color = parentColor;
								
					BinaryTreeSort::rotate_right(root->parent);
					brotherNode->left->color = BTreeNode<T>::BLACK;

					if (really_delete) {
						eraseBlackFunc(root);
						break;
					}
					break;

							
			}
						
			//近侄红
			if (root == root->parent->left && !isBlackFunc(brotherNode->left)) {//转化为远侄红
		
					brotherNode->color = BTreeNode<T>::RED;
					brotherNode->left->color = BTreeNode<T>::BLACK;
					BinaryTreeSort::rotate_right(brotherNode);
								
					continue;
			}
			else if (root == root->parent->right && !isBlackFunc(brotherNode->right)) {
									
					brotherNode->color = BTreeNode<T>::RED;
					brotherNode->right->color = BTreeNode<T>::BLACK;
					BinaryTreeSort::rotate_left(brotherNode);
								
					continue;
			}
						

				
			//兄弟节点也是黑色，且没有儿子
			//1 父节点是红色
			if (!isBlackFunc(root->parent) && isBlackFunc(root) && isBlackFunc(brotherNode)) {

				root->parent->color = BTreeNode<T>::BLACK;
				brotherNode->color = BTreeNode<T>::RED;
				if (really_delete) {
					eraseBlackFunc(root);
					break;
				}
				else {
					break;
				}

			}
			//2 父节点是黑色
			if (isBlackFunc(root->parent) && isBlackFunc(root) && isBlackFunc(brotherNode)) {	
						
					brotherNode->color = BTreeNode<T>::RED;
					auto parent = root->parent;

					if(really_delete)eraseBlackFunc(root);
					//eraseBlackFunc(brotherNode);
						
					root = parent;
					really_delete = false;


					continue;

						

			}
			

		}

		return true;
	}


	NodePtr _find(T t) {
		return BinaryTreeSort<T>::_find(t);
	}

	void outPutMessageInsert(T t) {
		_insert((new BTreeNode<T>(t)));
		
#ifdef DEBUG
		auto root = BinaryTreeSort::getRoot();
		checkRedBlackTree(root);
		std::cout << "\n\nafter _delete\t" << t << "\n";
		PrintAll(root);

#endif DEBUG
		
		
	}

	void outPutMessageDelete(T t) {

		_delete(_find(t));
		
#ifdef DEBUG
		auto root = BinaryTreeSort::getRoot();
		checkRedBlackTree(root);
		std::cout << "\n\nafter _delete\t" << t << "\n";
		PrintAll(root);

#endif DEBUG
	}

private:
	
	
	size_t checkRedBlackTree(NodePtr root) {

		size_t num = 0;

		if (!root) {
			return 0;
		}

		if (root == BinaryTreeSort::getRoot() && root->color != BTreeNode<T>::BLACK) {
			assert(false);
		}

		if (root->color == BTreeNode<T>::BLACK) {
			num += 1;
		}
		else if(root->color == BTreeNode<T>::RED){
			if (root->parent && root->parent->color == BTreeNode<T>::RED) {
				assert(false);
			}
		}
		else {
			assert(false);
		}

		size_t leftnum = checkRedBlackTree(root->left);;
		size_t rightnum = checkRedBlackTree(root->right);
		
		if (leftnum != rightnum) {
			assert(false);
		}

		num += leftnum;
		return num;
		
	}




};



#include <vector>
#include <random>
#include <stdlib.h>
#include<stdio.h>
#include <ctime>
#include <windows.h>
#include <mutex>
#include "TimeCount.h"
#define RANDOM_MAX 1000000
#define VECTORSIZE 100000



int main() {
	
	BinaryTreeSort<uint32_t> btSortTree;
	AVLTree<uint32_t> avlTree;
	Red_Black_Tree<uint32_t> rbTree;
	TimeCount timemanage;


	std::vector<uint32_t>vecCache(RANDOM_MAX);
	for (int i = 0; i < RANDOM_MAX; i++) {
		vecCache.push_back(i);
	}

	std::vector<uint32_t> insertVec(VECTORSIZE);	//生成插入序列
	for (int i = 0; i < VECTORSIZE; i++) {
		auto index = rand() % vecCache.size();
		insertVec.push_back(vecCache[index]);
		vecCache.erase(vecCache.begin() + index);
	}
	std::vector<uint32_t> _toMakeDeleteVec(insertVec);	//复制插入序列
	std::vector<uint32_t> _toMakeFindVec(insertVec);

	std::vector<uint32_t> deleteVec(VECTORSIZE);				//生成删除序列
	for (int i = 0; i < VECTORSIZE; i++) {
		auto index = rand() % _toMakeDeleteVec.size();
		deleteVec.push_back(_toMakeDeleteVec[index]);
		_toMakeDeleteVec.erase(_toMakeDeleteVec.begin() + index);
	}

	std::vector<uint32_t> findVec(VECTORSIZE);
	for (int i = 0; i < VECTORSIZE; i++) {
		auto index = rand() % _toMakeFindVec.size();
		findVec.push_back(_toMakeFindVec[index]);
		_toMakeFindVec.erase(_toMakeFindVec.begin() + index);
	}

	
	uint64_t btsortTreeInsertTime, btsortTreeDeleteTime, btsortTreeFindTime;
	uint64_t avlTreeInsertTime, avlTreeDeleteTime, avlTreeFindTime;
	uint64_t rbTreeInsertTime, rbTreeDeleteTime, rbTreeFindTime;

	timemanage.start();

	//insert
	timemanage.getTime();
	for (auto i : insertVec) {
		btSortTree.outPutMessageInsert(i);
	}
	btsortTreeInsertTime = timemanage.getTime();
	for (auto i : insertVec) {
		avlTree.outPutMessageInsert(i);
	}
	avlTreeInsertTime = timemanage.getTime();
	for (auto i : insertVec) {
		rbTree.outPutMessageInsert(i);
	}
	rbTreeInsertTime = timemanage.getTime();
	

	//find
	for (auto i : findVec) {
		btSortTree._find(i);
	}
	btsortTreeFindTime = timemanage.getTime();
	for (auto i : findVec) {
		avlTree._find(i);
	}
	avlTreeFindTime = timemanage.getTime();
	for (auto i : findVec) {
		rbTree._find(i);
	}
	rbTreeFindTime = timemanage.getTime();

	//delete
	for (auto i : deleteVec) {
		btSortTree.outPutMessageDelete(i);
	}
	btsortTreeDeleteTime = timemanage.getTime();

	
	for (auto i : deleteVec) {
		avlTree.outPutMessageDelete(i);
	}
	avlTreeDeleteTime = timemanage.getTime();

	
	for (auto i : deleteVec) {
		rbTree.outPutMessageDelete(i);
	}
	rbTreeDeleteTime = timemanage.getTime();



	std::cout << "\n\n\n" << std::endl;
	std::cout << "BTSortTree insert cost time:\t" << btsortTreeInsertTime << std::endl;
	std::cout << "AVLTree insert cost time:\t" << avlTreeInsertTime << std::endl;
	std::cout << "RBTree insert cost time:\t" << rbTreeInsertTime << std::endl;


	std::cout << "\n\n\n" << std::endl;
	std::cout << "BTSortTree find cost time:\t" << btsortTreeFindTime << std::endl;
	std::cout << "AVLTree find cost time:\t" << avlTreeFindTime << std::endl;
	std::cout << "RBTree find cost time:\t" << rbTreeFindTime << std::endl;


	std::cout << "\n\n\n" << std::endl;
	std::cout << "BTSortTree delete cost time:\t" << btsortTreeDeleteTime << std::endl;
	std::cout << "AVLTree delete cost time:\t" << avlTreeDeleteTime << std::endl;
	std::cout << "RBTree delete cost time:\t" << rbTreeDeleteTime << std::endl;

	std::cout << "\n\n\n" << std::endl;
	std::cout << "ALL TIME:\n";
	std::cout << "BTSortTree:\t" << btsortTreeInsertTime + btsortTreeFindTime + btsortTreeDeleteTime << std::endl;
	std::cout << "AVLTree:\t" << avlTreeInsertTime + avlTreeFindTime + avlTreeDeleteTime << std::endl;
	std::cout << "RBTree:\t" << rbTreeInsertTime + rbTreeFindTime + rbTreeDeleteTime << std::endl;

	




	getchar();
}

