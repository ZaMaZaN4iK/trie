#ifndef BOOST_TRIE_MULTIMAP
#define BOOST_TRIE_MULTIMAP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <boost/trie/trie.hpp>


namespace boost { namespace tries {

template<typename Key, typename Value>
class trie_multimap
{
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef trie<key_type, value_type, true> trie_type;
	typedef trie_multimap<Key, Value> trie_multimap_type;
	typedef typename trie_type::iterator iterator;
	typedef typename trie_type::const_iterator const_iterator;
	typedef typename trie_type::reverse_iterator reverse_iterator;
	typedef typename trie_type::const_reverse_iterator const_reverse_iterator;
	typedef typename trie_type::pair_iterator_bool pair_iterator_bool;
	typedef typename trie_type::iterator_range iterator_range;
	typedef size_t size_type;

protected:
	trie_type t;

public:
	explicit trie_multimap() : t()
	{
	}

	explicit trie_multimap(const trie_multimap_type& other) : t(other.t)
	{
	}

	trie_multimap_type& operator=(const trie_multimap_type& other)
	{
		t = other.t;
		return *this;
	}


	iterator begin() 
	{
		return t.begin();
	}

	const_iterator begin() const
	{
		return t.begin();
	}

	const_iterator cbegin() const
	{
		return t.cbegin();
	}

	iterator end() 
	{
		return t.end();
	}

	const_iterator end() const
	{
		return t.end();
	}

	const_iterator cend() const
	{
		return t.cend();
	}

	reverse_iterator rbegin() 
	{
		return t.rbegin();
	}

	const_reverse_iterator rbegin() const
	{
		return t.rbegin();
	}

	const_reverse_iterator crbegin() const
	{
		return t.crbegin();
	}

	reverse_iterator rend() 
	{
		return t.rend();
	}

	const_reverse_iterator rend() const
	{
		return t.rend();
	}

	const_reverse_iterator crend() const
	{
		return t.crend();
	}

	template<typename Iter>
	iterator insert(Iter first, Iter last, const value_type& value)
	{
		return t.insert_equal(first, last, value);
	}

	template<typename Container>
	iterator insert(const Container& container, const value_type& value)
	{
		return t.insert_equal(container, value);
	}

	template<typename Iter>
	iterator find(Iter first, Iter last)
	{
		return t.find(first, last);
	}

	template<typename Container>
	iterator find(const Container& container)
	{
		return t.find(container);
	}

	template<typename Iter>
	size_type count(Iter first, Iter last)
	{
		return t.count(first, last);
	}

	template<typename Container>
	size_type count(const Container& container)
	{
		return t.count(container);
	}

	template<typename Iter>
	size_type count_prefix(Iter first, Iter last)
	{
		return t.count_prefix(first, last);
	}

	template<typename Container>
	size_type count_prefix(const Container& container)
	{
		return t.count_prefix(container);
	}

	template<typename Iter>
	iterator_range find_prefix(Iter first, Iter last)
	{
		return t.find_prefix(first, last);
	}

	template<typename Container>
	iterator_range find_prefix(const Container& container)
	{
		return t.find_prefix(container);
	}

	template<typename Iter>
	iterator_range equal_range(Iter first, Iter last)
	{
		return t.equal_range(first, last);
	}

	template<typename Container>
	iterator_range equal_range(const Container& container)
	{
		return t.equal_range(container);
	}

	// upper and lower bound
	template<typename Iter>
	iterator upper_bound(Iter first, Iter last)
	{
		return (iterator)t.upper_bound(first, last);
	}

	template<typename Container>
	iterator upper_bound(const Container& container)
	{
		return (iterator)t.upper_bound(container);
	}

	template<typename Iter>
	iterator lower_bound(Iter first, Iter last)
	{
		return (iterator)t.lower_bound(first, last);
	}

	template<typename Container>
	iterator lower_bound(const Container& container)
	{
		return (iterator)t.lower_bound(container);
	}

	//erasions
	iterator erase(iterator it)
	{
		return t.erase(it);
	}

	iterator erase(const_iterator it)
	{
		return t.erase(it);
	}

	reverse_iterator erase(reverse_iterator it)
	{
		return t.erase(it);
	}

	reverse_iterator erase(const_reverse_iterator it)
	{
		return t.erase(it);
	}

	template<typename Container>
	iterator erase(const Container &container)
	{
		return t.erase(container);
	}

	template<typename Iter>
	iterator erase(Iter first, Iter last)
	{
		return t.erase(first, last);
	}

	template<typename Container>
	size_type erase_prefix(const Container &container)
	{
		return t.erase_prefix(container);
	}

	template<typename Iter>
	size_type erase_prefix(Iter first, Iter last)
	{
		return t.erase_prefix(first, last);
	}

	size_type count_node() const
	{
		return t.count_node();
	}

	size_type size() const
	{
		return t.size();
	}

	bool empty()
	{
		return t.empty();
	}

	void swap(const trie_multimap_type& other)
	{
		t.swap(other.t);
	}

	void clear()
	{
		t.clear();
	}

	~trie_multimap()
	{
	}

};
}	// namespace tries
}	// namespace boost
#endif
