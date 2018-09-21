#include<iostream>
#define minus_infinite -1000000
using namespace std;

template <typename T> 
struct node {
public:
	node<T>* left;
	node<T>* right;
	node<T>* child;
	node<T>* parent;
	T value;
	int degree;
	bool marked;

	node(T new_value): value(new_value), left(this), right(this), degree(0), marked(false), child(NULL), parent(NULL){}
};

template <typename T>
class FibonacciHeap {
public:
	node<T>* heap;
	int n;

	FibonacciHeap(): heap(NULL), n(0) {}

	void insert( T value ) {
		node<T>* new_node = new node<T>(value);
		heap = concatenate_root(heap, new_node);
		n += 1;
	}

	void Union( FibonacciHeap<T>& second_heap) {
		union_(second_heap);
		second_heap.heap = NULL;
	}

	T get_minimum_value() {
		return heap->value;
	}

	T extract_min() {
		node<T>* old = heap;
		heap = extract_min_(heap);
		T result = old->value;
		delete old;
		return result;
	}

	void decrease_key(node<T>* n, T value) {
		heap=decrease_key_(n, value);
	}

	T delete_(node<T>* n) {
		decrease_key_(n, minus_infinite);
		return extract_min();
	}

	void display_root() {
		node<T>* aux = heap;
		do {
			cout << aux->value << ", ";
			aux = aux->right;
		} while (aux != heap);
		cout << endl;
	}

	virtual ~FibonacciHeap() {
		if (heap) {
			deleteAll(heap);
		}
	}
	
private:

	node<T>* concatenate_root(node<T>* a, node<T>* b) {

		if ( a == NULL )return b;
		if ( b == NULL )return a;

		if ( a->value > b->value ) {
			node<T>* temp = a;
			a = b;
			b = temp;
		}//a>b

		node<T>* a_right = a->right;	
		node<T>* b_left = b->left;
		a->right = b;
		b->left = a;
		a_right->left = b_left;
		b_left->right = a_right;

		return a;
	}
	
	FibonacciHeap<T> union_(FibonacciHeap<T>& H2) {

		if (this->heap == NULL)return H2;
		if (H2.heap == NULL)return *this;
		
		//concatenamos roots de H1 y H2
		node<T>* a = this->heap;
		node<T>* b = H2.heap;
		node<T>* a_right = a->right;
		node<T>* b_left = b->left;

		a->right = b;
		b->left = a;
		a_right->left = b_left;
		b_left->right = a_right;

		if ((this->heap)->value > (H2.heap)->value) {
			this->heap = H2.heap;
		}

		this->n += H2.n;
		
		return *this;
	}

	void linking_tree(node<T>* parent, node<T>* child) {
		child->left = child->right = child;
		child->parent = parent;
		parent->degree++;
		parent->child = concatenate_root(parent->child, child);
	}

	node<T>* consolidate(node<T>* n) {
		if (n == NULL)return n;
		node<T>* trees[64] = { NULL };

		while (true) {
			if (trees[n->degree] != NULL) {
				node<T>* t = trees[n->degree];
				if (t == n)break;
				trees[n->degree] = NULL;

				if ( t->value < n->value ) {
					node<T>* aux = n;
					n = t;
					t = aux;
				}
				
				t->left->right = t->right;
				t->right->left = t->left;
				linking_tree(n, t);

				continue;
			}
			else {
				trees[n->degree] = n;
			}
			n = n->right;
		}
		return n;
	}

	node<T>* extract_min_(node<T>* n) {
		node<T>* heap_ = heap;
		if (heap_ == NULL)
			return heap_;

		if (heap_->child != NULL) {
			node<T>* children = heap_->child;
			do {
				children->marked = false;
				children->parent = NULL;
				children = children->right;
			} while (children != heap_->child);
		}

		//REMOVER HEAP DE ROOT LIST Y SUBIR LOS HIJOS A ROOT LIST
		if (heap_->right == heap_) {
			n = heap_->child;
		}
		else {
			heap_->right->left = heap_->left;
			heap_->left->right = heap_->right;
			n = concatenate_root(heap_->right, heap_->child);
		}

		n = consolidate(n);

		this->n -= 1;

		//CALCULAR NEW HEAP
		node<T>* min = n;
		node<T>* aux = n;
		do {
			if ( aux->value < min->value )
				min = aux;
			aux = aux->right;
		} while (aux != n);

		return min;
	}
	
	node<T>* cut_(node<T>* n) {
		if (n->right == n) {
			n->parent->child = NULL;
		}
		else {
			n->right->left = n->left;
			n->left->right = n->right;
			n->parent->child = n->right;
		}
		n->right = n->left = n;
		n->marked = false;
		return concatenate_root(heap,n);
	}

	node<T>* decrease_key_(node<T>* n, T value) {
		if ( n->value < value ) return heap;
		n->value = value;

		if ( !n->parent ) 
			if (n->value < heap->value) 
				heap = n;
		

		else if ( n->parent && n->value < n->parent->value ) {
			heap = cut_(n);

			node<T>* parent = n->parent;	//CASCADING-CUT
			n->parent = NULL;

			while (parent != NULL && parent->marked) {
				heap = cut_(parent);
				n = parent;
				parent = n->parent;
				n->parent = NULL;
			}

			if (parent != NULL && parent->parent != NULL)
				parent->marked = true;
		}
		return heap;
	}


	void deleteAll(node<T>* n) {
		if (n != NULL) {
			node<T>* c = n;
			node<T>* aux;

			do {
				deleteAll(c->child);
				aux = c;
				c = c->right;
				delete aux;
			} while (c != n);
		}
	}

};




int main() {
	FibonacciHeap<int> H;
	H.insert(1);
	H.insert(8);
	H.insert(4);
	H.insert(2);
	H.insert(3);
	H.insert(7);

	/*
	//test case union
	FibonacciHeap<int> H2;
	H2.insert(0);
	H2.insert(5);

	H.Union(H2);
	H.display_root();
	*/

	/*
	//test case extract min
	H.display_root();
	cout << H.get_minimum_value() << endl;
	H.extract_min();
	*/

	node<int>* node1 = H.heap->right;
	/*
	H.display_root();//decrease_key
	H.decrease_key(node1, 0);
	H.display_root();
	*/
	/*
	H.display_root();//delete_key
	H.delete_(node1);
	H.display_root();
	*/

	system("PAUSE");
	return 0;
}