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
		heap = concatenate_root(heap, second_heap.heap);
		n += second_heap.n;
	}
	
	T get_minimum_value() {
		return heap->value;
	}
	
	T extract_min() {
		node<T>* old = heap;
		heap = extract_min_();
		T result = old->value;
		delete old;
		return result;
	}
	
	void decrease_key(node<T>* n, T value) {
		heap = decrease_key_(n, value);
	}
	
	T delete_(node<T>* n) {
		decrease_key_(n, minus_infinite);
		return extract_min();
	}
	
	void display_root() {
		node<T>* aux = heap;
		
		do {
			imprimir(aux, "\t");
			cout << endl << "\t|" << endl;
			aux = aux->right;
		} while (aux != heap);
		
		cout <<"------------------------------------"<< endl;
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
		
		a->right->left = b->left;
		b->left->right = a->right;
		
		a->right = b;
		b->left = a;
		
		return a;
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
	
	node<T>* extract_min_() {
		if (heap == NULL)
			return heap;
		
		if (heap->child != NULL) {//DESMARCAR NODOS
			node<T>* children = heap->child;
			do {
				children->marked = false;
				children->parent = NULL;
				children = children->right;
			} while (children != heap->child);
		}
		
		//REMOVER HEAP DE ROOT LIST Y SUBIR LOS HIJOS A ROOT LIST
		node<T>* n;
		if (heap->right == heap) {
			n = heap->child;
		}
		else {
			heap->right->left = heap->left;
			heap->left->right = heap->right;
			n = concatenate_root(heap->right, heap->child);
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
		
		if ( !n->parent ){
			if (n->value < heap->value) 
				heap = n;
		}
		else if ( n->value < n->parent->value ) {
			heap = cut_(n);
			
			node<T>* parent = n->parent;	//CASCADING-CUT
			n->parent = NULL;
			
			while (parent != NULL && parent->marked) {
				heap = cut_(parent);
				n = parent;
				parent = parent->parent;
				n->parent = NULL;
			}
			
			if (parent != NULL && parent->parent != NULL)
				parent->marked = true;
		}
		return heap;
	}
	
	void imprimir(node<T> *pointer, string buffer){
		if(pointer){
			imprimir(pointer->child,buffer+"\t");
			cout<<endl;
			cout<<buffer<<pointer->value;
			cout<<endl; 
			if( pointer->child && pointer->child != pointer->child->right )
				imprimir(pointer->child->right,buffer+"\t");
		}
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
	H.display_root();
	
	//test case union
	/*
	FibonacciHeap<int> H2;
	H2.insert(0);
	H2.insert(5);
	
	H.Union(H2);
	H.display_root();
	*/
	
	
	
	H.display_root();//test case extract min
	cout << "value to extract is: " << H.get_minimum_value() << endl;
	H.extract_min();
	H.display_root();
	
	node<int>* node1 = H.heap; 
	/*
	cout<< " decrease key" << endl;//decrease_key
	H.decrease_key(node1, 1);
	H.display_root();
	*/
	/*
	cout<< "delete key" << endl;//delete_key
	H.delete_(node1);
	H.display_root();
	*/
	delete node1;
	system("PAUSE");
	return 0;
}
