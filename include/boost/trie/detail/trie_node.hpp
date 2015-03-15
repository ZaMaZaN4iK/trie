#ifndef BOOST_TRIE_NODE_HPP
#define BOOST_TRIE_NODE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <map>
#include <vector>
#include <boost/utility.hpp>
#include <memory>

namespace boost { namespace tries {

namespace detail {

template <typename Key, typename Value, bool isMultiValue>
struct trie_node;

struct list_node_base : protected boost::noncopyable {
	typedef list_node_base *base_ptr;
	base_ptr pred;
	base_ptr next;

	list_node_base() : pred(0), next(0)
	{}
};

template <typename Key, typename Value>
struct value_list_node : public list_node_base {
	typedef Key key_type;
	typedef Value value_type;
	typedef trie_node<key_type, value_type, true> trie_node_type;
	typedef trie_node_type * trie_node_ptr;
	typedef value_list_node<key_type, value_type> node_type;
	typedef node_type * node_ptr;
	typedef list_node_base * base_ptr;
	value_type value;
	trie_node_ptr node_in_trie;

	explicit value_list_node() : value(), node_in_trie(0)
	{
	}

	explicit value_list_node(const value_type& x) : value(x), node_in_trie(0)
	{
	}

	~value_list_node() {
		node_in_trie->self_value_count--;
	}
};

template <typename Key, typename Value>
struct trie_node<Key, Value, true> : private boost::noncopyable
{
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type * value_ptr;
	typedef size_t size_type;
	typedef trie_node<key_type, value_type, true> node_type;
	typedef node_type* node_ptr;
	typedef value_list_node<key_type, value_type> value_list_type;
	typedef value_list_type * value_list_ptr;
	typedef std::map<key_type, node_ptr> children_type;
	typedef typename children_type::iterator children_iter;

	children_type children;
	children_iter child_iter_of_parent;
	node_ptr parent;
	// store the iterator to optimize operator++ and operator--
	// utilize that the iterator in map does not change after insertion
	size_type value_count;
	size_type self_value_count;
	value_list_ptr value_list_header;
	value_list_ptr value_list_tail;

	node_ptr pred_node;
	node_ptr next_node;

	explicit trie_node() : parent(0), value_count(0), self_value_count(0),
	value_list_header(0), value_list_tail(0),
	pred_node(0), next_node(0)
	{
	}

	const key_type& key_elem() const
	{
		return child_iter_of_parent->first;
	}

	size_type count() const
	{
		return self_value_count;
	}

	bool no_value() const
	{
		return self_value_count == 0;
	}

	template<typename Allocator>
	void remove_values(Allocator& alloc) {
		value_list_ptr vp = value_list_header;
		if (!no_value())
		{
			while (vp != NULL)
			{
				value_list_ptr tmp = static_cast<value_list_ptr>(vp->next);
				alloc.destroy(vp);
				alloc.deallocate(vp, 1);
				vp = tmp;
			}
		}
		self_value_count = 0;
		value_list_header = value_list_tail = NULL;
	}

	template<typename Allocator>
	void add_value(const value_type& value, Allocator& alloc) {
		value_list_ptr vn = alloc.allocate(1);
		vn = new(vn) value_list_type(value);
		vn->node_in_trie = this;
		vn->next = this->value_list_header;
		if (this->value_list_header != NULL)
		{
			this->value_list_header->pred = vn;
		}
		else {
			this->value_list_tail = vn;
		}
		this->value_list_header = vn;
		++this->self_value_count;
	}

	template<typename Allocator>
	void copy_values_from(const node_type& other, Allocator& alloc) {
		value_list_ptr vp = other.value_list_header;
		while (vp != NULL) {
			this->add_value(vp->value, alloc);
			vp = static_cast<value_list_ptr>(vp->next);
		}
		self_value_count = other.self_value_count;
		value_count = other.value_count;
	}

	struct comparator {
		bool operator () (const node_type& a, const node_type& b) const {
			return a.key < b.key;
		}

		bool operator () (const key_type& a, const node_type& b) const {
			return a < b.key;
		}

		bool operator () (const node_type& a, const key_type& b) const {
			return a.key < b;
		}
	};

	friend bool operator < (const node_type& a, const node_type& b) {
		return a.key < b.key;
	}
	friend bool operator > (const node_type& a, const node_type& b) {
		return a.key > b.key;
	}
	friend bool operator == (const node_type& a, const node_type& b) {
		return a.key == b.key;
	}
};

template <typename Key, typename Value>
struct trie_node<Key, Value, false> : private boost::noncopyable
{
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type * value_ptr;
	typedef size_t size_type;
	typedef trie_node<key_type, value_type, false> node_type;
	typedef node_type* node_ptr;
	typedef std::map<key_type, node_ptr> children_type;
	typedef typename children_type::iterator children_iter;

	node_ptr parent;
	node_ptr pred_node;
	node_ptr next_node;
	children_type children;
	children_iter child_iter_of_parent;
	size_type value_count;
	bool has_value;
	value_type value;

	explicit trie_node() : parent(0), value_count(0), pred_node(0), next_node(0), has_value(false)
	{
	}

	const key_type& key_elem() const
	{
		return child_iter_of_parent->first;
	}

	size_type count() const
	{
		return has_value;
	}

	bool no_value() const
	{
		return !has_value;
	}

	void remove_values() {
		has_value = false;
	}

	void add_value(const value_type& value) {
		this->value = value;
		has_value = true;
	}

	void copy_values_from(const node_type& other) {
		value = other.value;
		value_count = other.value_count;
		has_value = other.has_value;
	}

	struct comparator {
		bool operator () (const node_type& a, const node_type& b) const {
			return a.key < b.key;
		}

		bool operator () (const key_type& a, const node_type& b) const {
			return a < b.key;
		}

		bool operator () (const node_type& a, const key_type& b) const {
			return a.key < b;
		}
	};

	friend bool operator < (const node_type& a, const node_type& b) {
		return a.key < b.key;
	}
	friend bool operator > (const node_type& a, const node_type& b) {
		return a.key > b.key;
	}
	friend bool operator == (const node_type& a, const node_type& b) {
		return a.key == b.key;
	}
};

template <typename Key>
struct trie_node<Key, void, false> : private boost::noncopyable
{
	typedef Key key_type;
	typedef void value_type;
	typedef value_type * value_ptr;
	typedef size_t size_type;
	typedef trie_node<key_type, value_type, false> node_type;
	typedef node_type* node_ptr;
	typedef std::map<key_type, node_ptr> children_type;
	typedef typename children_type::iterator children_iter;

	children_type children;
	children_iter child_iter_of_parent;
	node_ptr parent;
	size_type value_count;
	bool key_ends_here;
	node_ptr pred_node;
	node_ptr next_node;

	explicit trie_node() : parent(0), value_count(0),  key_ends_here(false), pred_node(0), next_node(0)
	{
	}

	const key_type& key_elem() const
	{
		return child_iter_of_parent->first;
	}

	size_type count() const
	{
		return key_ends_here;
	}

	bool no_value() const
	{
		return !key_ends_here;
	}

	void remove_values() {
		key_ends_here = false;
	}

	void copy_values_from(const node_type& other) {
		key_ends_here = other.key_ends_here;
		value_count = other.value_count;
	}

	struct comparator {
		bool operator () (const node_type& a, const node_type& b) const {
			return a.key < b.key;
		}

		bool operator () (const key_type& a, const node_type& b) const {
			return a < b.key;
		}

		bool operator () (const node_type& a, const key_type& b) const {
			return a.key < b;
		}
	};

	friend bool operator < (const node_type& a, const node_type& b) {
		return a.key < b.key;
	}
	friend bool operator > (const node_type& a, const node_type& b) {
		return a.key > b.key;
	}
	friend bool operator == (const node_type& a, const node_type& b) {
		return a.key == b.key;
	}
};

template<class Node, class Allocator, bool multi_value_node>
struct value_remove_helper {
	void operator() (Node* node, Allocator& alloc) {
		node->remove_values(alloc);
	}
	void operator() (Node* node) { }
};

template<class Node, class Allocator>
struct value_remove_helper<Node, Allocator, false> {
	void operator() (Node *node) {
		node->remove_values();
	}
	void operator() (Node *node, Allocator& alloc) { }
};

template<class Node, class Allocator, bool multi_value_node>
struct value_copy_helper {
	void operator() (Node *dest, Node* source, Allocator& alloc) {
		dest->copy_values_from(*source, alloc);
	}
	void operator() (Node *dest, Node* source) { }
};

template<class Node, class Allocator>
struct value_copy_helper<Node, Allocator, false> {
	void operator() (Node *dest, Node* source, Allocator& alloc) { }
	void operator() (Node *dest, Node* source) { 
		dest->copy_values_from(*source);
	}
};
} /* detail */
} /* tries */
} /* boost */

#endif
