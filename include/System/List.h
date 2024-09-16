#pragma once
template <class T>
class List {
public:
	class Node {
	public:
		T Value;
		Node* Next;
	public:
		Node(const T& value) : Value(value), Next(0) { }
	};

	class Iterator {
		Node* node;
	public:
		Iterator(Node* p = 0) : node(p) {}
		T& operator*() { return node->Value; }
		Iterator& operator++() { node = node->Next; return *this; }
		bool operator!=(const Iterator& other) { return node != other.node; }
	};

	friend Iterator begin(const List& list) { return Iterator(list.head); }
	friend Iterator end(const List& list) { return Iterator(); }

private:
	int count;
	Node* head;
	Node* tail;

private:
	void clear();
public:
	List() : head(0), tail(0), count(0) { }
	~List() { clear(); }
public:
	// Lấy số nút
	int Count() const { return count; }
	
	// Kiểm tra danh sách rỗng
	bool IsEmpty() const { return count == 0; }

	// Lấy nút đầu, trả về NULL nếu danh sách rỗng
	Node* First() const { return count == 0 ? 0 : head; }
	
	// Lấy nút cuối, trả về NULL nếu danh sách rỗng
	Node* Last() const { return count == 0 ? 0 : tail; }

public:
	// Xóa toàn bộ danh sách
	List& RemoveAll() { clear(); return *this; }

	// Thêm giá trị vào cuối danh sách
	List& AddLast(const T& value) {
		Node* p = new Node(value);
		if (count++ == 0) {
			head = p;
		}
		else {
			tail->Next = p;
		}
		tail = p;
		return *this;
	}

	// Thêm giá trị vào đầu danh sách
	List& AddFirst(const T& value) {
		Node* p = new Node(value);
		if (count++ == 0) {
			tail = p;
		}
		else {
			p->Next = head;
		}
		head = p;
		return *this;
	}
};

template<class T>
void List<T>::clear() {
	while (count) {
		Node* p = head;
		head = p->Next;
		delete p;

		--count;
	}
}
