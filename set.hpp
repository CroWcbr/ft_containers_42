#ifndef SET_HPP
# define SET_HPP

# include "./tree/RBTree.hpp"
# include "./iter/iterator_reverse.hpp"

namespace ft
{
	template<class Key,
				class Compare = std::less<Key>,
				class Allocator = std::allocator<Key> >
	class set
	{
	public:
		typedef				Key							key_type;
		typedef				Key							value_type;
		typedef				Compare						key_compare;
		typedef				Compare						value_compare;
		typedef				Allocator					allocator_type;
		typedef typename 	Allocator::reference		reference;
		typedef typename 	Allocator::const_reference	const_reference;
		typedef typename 	Allocator::difference_type	difference_type;
		typedef typename 	Allocator::size_type		size_type;		
		typedef typename 	Allocator::pointer			pointer;
		typedef typename 	Allocator::const_pointer	const_pointer;

		typedef RBTree<value_type, key_compare, allocator_type>	tree_type;
		typedef typename tree_type::iterator						iterator;
		typedef typename tree_type::const_iterator					const_iterator;
		typedef typename tree_type::reverse_iterator				reverse_iterator;
		typedef typename tree_type::const_reverse_iterator			const_reverse_iterator;

	private:
		tree_type _tree;

	public:
// 23.3.3.1 construct/copy/destroy:
		explicit set(const key_compare &comp = key_compare(), 
				const allocator_type &alloc = allocator_type()): 
			_tree(tree_type(comp, alloc)) 
		{}
		
		template< class InputIterator >
		set(InputIterator first,
				InputIterator last,
				const Compare & comp = Compare(),
				const Allocator &alloc = Allocator()):
			_tree(tree_type(comp, alloc)) 
		{
			insert(first, last);
		}

		set(const set& x): 
			_tree(tree_type(x._tree)) {}

		set operator=(const set & x)
		{
			_tree = x._tree;
			return *this;
		}

		allocator_type get_allocator() const{
			return _tree.get_allocator();
		}

// iterators:
		iterator begin() { return _tree.begin(); }
		const_iterator begin() const { return _tree.begin(); }
		iterator end() { return _tree.end(); }
		const_iterator end() const { return _tree.end(); }
		reverse_iterator rbegin() {	return _tree.rbegin(); }
		const_reverse_iterator rbegin() const { return _tree.rbegin(); }
		reverse_iterator rend() { return (_tree.rend()); }
		const_reverse_iterator rend() const { return _tree.rend(); }

// capacity:
		bool empty() const { return _tree.empty(); }
		size_type size() const { return _tree.size(); }
		size_type max_size() const { return _tree.max_size(); }

// modifiers:
		ft::pair<iterator, bool> insert( const value_type& x)
		{
			return _tree.insert(x);
		}

		iterator insert( iterator position, const value_type& x)
		{
			(void)position;
			return _tree.insert(x).first;
		}

		template<class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			while (first!=last)
				_tree.insert(*first++);
		}

		void erase(iterator position)
		{
			_tree.erase(*position);
		}

		size_type erase(const Key & x)
		{
			return _tree.erase(x);
		}

		void erase(iterator first, iterator last)
		{
			while (first!=last)
				_tree.erase(*first++);
		}

		void swap(set & other) 
		{
			_tree.swap(other._tree);
		}

		void clear(){
			_tree.clear();
		}	

// observers:
		value_compare value_comp() const { return (_tree.value_comp()); }
		key_compare key_comp() const { return (value_comp()); }

// set operations:
		iterator find(const key_type& x) { return _tree.find(x); }
		size_type count(const key_type &x) const { return _tree.count(x); }
		iterator lower_bound(const key_type& x) { return _tree.lower_bound(x); }
		iterator upper_bound(const key_type& x) { return _tree.lower_bound(x); }
		pair<iterator, iterator> equal_range(const key_type & x) { return _tree.equal_range(x); }

		template<class K, class Com, class Al>
		friend bool operator==(const set<K, Com, Al>& lhs, const set<K, Com, Al>& rhs);

		template<class K, class Com, class Al>
		friend bool operator<(const set<K, Com, Al>& lhs, const set<K, Com, Al>& rhs);
	};

	template<class Key, class Compare, class Alloc>
	bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return lhs._tree == rhs._tree;
	}

	template<class Key, class Compare, class Alloc>
	bool operator!=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template<class Key, class Compare, class Alloc>
	bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return lhs._tree < rhs._tree;
	}

	template<class Key, class Compare, class Alloc>
	bool operator>(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template<class Key, class Compare, class Alloc>
	bool operator<=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return !(lhs > rhs);
	}

	template<class Key, class Compare, class Alloc>
	bool operator>=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template<class Key,class Compare, class Alloc>
	void swap(const ft::set<Key, Compare, Alloc>& lhs, const ft::set<Key, Compare, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
};

#endif
