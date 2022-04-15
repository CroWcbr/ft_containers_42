#ifndef MAP_HPP
# define MAP_HPP

# include <memory>
# include "./tree/RBTree.hpp"
# include "./utils/utils.hpp"

namespace ft {

	template <class Key,
				class T,
				class Compare = std::less<Key>,
				class Allocator = std::allocator<ft::pair<const Key, T> > >
	class map
	{
	public:
		typedef 			Key							key_type;
		typedef 			T							mapped_type;
		typedef 			ft::pair<const Key, T>		value_type;
		typedef 			Compare						key_compare;
		typedef 			Allocator					allocator_type;
		typedef typename 	Allocator::reference		reference;
		typedef typename 	Allocator::const_reference	const_reference;
		typedef typename 	Allocator::difference_type	difference_type;
		typedef typename 	Allocator::size_type		size_type;
		typedef typename 	Allocator::pointer			pointer;
		typedef typename 	Allocator::const_pointer	const_pointer;

		class value_compare
			: public std::binary_function<value_type,value_type,bool> {
		friend class map;
		protected:
			Compare comp;
			value_compare(Compare c) : comp(c) {}
		public:
			bool operator()(const value_type& x, const value_type& y) const {
				return comp(x.first, y.first);
			}
		};

		typedef RBTree<value_type, value_compare, allocator_type>	tree_type;
		typedef typename tree_type::iterator						iterator;
		typedef typename tree_type::const_iterator					const_iterator;
		typedef typename tree_type::reverse_iterator				reverse_iterator;
		typedef typename tree_type::const_reverse_iterator			const_reverse_iterator;
	
	public:
		allocator_type	_alloc;
		tree_type		_tree;
		key_compare		_comp;
	
	public:
// 23.3.1.1 construct/copy/destroy:
		explicit map(const Compare& comp = Compare(),
						const Allocator& alloc = Allocator()):
			_alloc(alloc), 
			_tree(tree_type(comp, alloc)), 
			_comp(comp)
		{}

		template< class InputIterator >
		map(InputIterator first,
				InputIterator last,
				const Compare& comp = Compare(),
				const Allocator& alloc = Allocator()):
			_alloc(alloc), 
			_tree(tree_type(comp, alloc)), 
			_comp(comp) 
		{
			insert(first, last);
		}

		map(const map& x): _tree(tree_type(x._comp, x._alloc))
		{
			*this = x;
		}

		map& operator=(const map& x)
		{
			if (&x == this)
				return (*this);
			_tree = x._tree;
			_alloc = x._alloc;
			_comp = x._comp;
			return *this;
		}

		~map()
		{}

// iterators:
		iterator begin() { return _tree.begin(); }
		const_iterator begin() const { return _tree.begin(); }
		iterator end() { return _tree.end(); }
		const_iterator end() const { return _tree.end(); }
		reverse_iterator rbegin() { return _tree.rbegin(); }
		const_reverse_iterator rbegin() const { return _tree.rbegin(); }
		reverse_iterator rend() { return _tree.rend(); }
		const_reverse_iterator rend() const { return _tree.rend(); }		

// capacity:
		bool empty() const { return _tree.empty(); }
		size_type size() const { return _tree.size(); }
		size_type max_size() const { return _tree.max_size(); }

// 23.3.1.2 element access
		T& operator[](const key_type& x)
		{
			return (*((this->insert(ft::make_pair(x,mapped_type()))).first)).second;
		}

// modifiers:
		pair<iterator, bool> insert(const value_type& x)
		{
			return _tree.insert(x);
		}
	
		iterator insert(iterator position, const value_type& x)
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

		size_type erase(const Key& x)
		{
			return _tree.erase(ft::make_pair(x, mapped_type()));
		}

		void erase(iterator first, iterator last)
		{
			while (first!=last)
				_tree.erase(*first++);
		}

		void swap(map & other)
		{
			_tree.swap(other._tree);
		}

		void clear()
		{
			_tree.clear();
		}

// observers:
		key_compare key_comp() const { return _comp; }
		value_compare value_comp() const { return _tree.value_comp(); }

// 23.3.1.3 map operations:
		iterator	find(const Key& x) { return _tree.find(ft::make_pair(x, mapped_type())); }
		const_iterator find(const key_type& x) const { return _tree.find(ft::make_pair(x, mapped_type())); }
		size_type count(const Key& x) const { return _tree.count(ft::make_pair(x, mapped_type())); }
		iterator lower_bound(const key_type& x) { return _tree.lower_bound(ft::make_pair(x, mapped_type())); }	
		const_iterator lower_bound(const key_type& x) const { return _tree.lower_bound(ft::make_pair(x, mapped_type())); }	
		iterator upper_bound(const key_type& x) { return _tree.upper_bound(ft::make_pair(x, mapped_type())); }	
		const_iterator upper_bound(const key_type& x) const { return _tree.upper_bound(ft::make_pair(x, mapped_type())); }
		pair<iterator, iterator> equal_range(const key_type & x) { return _tree.equal_range(ft::make_pair(x, mapped_type())); }	
		pair<const_iterator, const_iterator> equal_range(const key_type & x) const { return _tree.equal_range(ft::make_pair(x, mapped_type())); }	

//other C++11 standart
		T& at(const Key &key)
		{
			iterator res = _tree.find(ft::make_pair(key, mapped_type()));
			if (res == _tree.end())
				throw std::out_of_range("key not found");
			return res->second;
		}

//synopsis
		template<class _Key, class _T, class _Compare, class _Alloc>
		friend bool operator==(const map<_Key, _T, _Compare, _Alloc> & lhs,
				const map<_Key, _T, _Compare, _Alloc>& rhs);

		template<class _Key, class _T, class _Compare, class _Alloc>
		friend bool operator<(const map<_Key, _T, _Compare, _Alloc> & lhs,
				const map<_Key, _T, _Compare, _Alloc>& rhs);
	};

	template<class Key, class T, class Compare, class Alloc>
	bool operator==(const map<Key, T, Compare, Alloc> & lhs, const map<Key, T, Compare, Alloc> & rhs)
	{
			return (lhs._tree == rhs._tree); 
	}

	template<class Key, class T, class Compare, class Alloc>
	bool operator!=(const map<Key, T, Compare, Alloc> & lhs, const map<Key, T, Compare, Alloc> & rhs)
	{
			return !(lhs == rhs); 
	}

	template<class Key, class T, class Compare, class Alloc>
	bool operator<(const map<Key, T, Compare, Alloc> & lhs, const map<Key, T, Compare, Alloc> & rhs)
	{
			return (lhs._tree < rhs._tree);
	}

	template<class Key, class T, class Compare, class Alloc>
	bool operator>(const map<Key, T, Compare, Alloc> & lhs, const map<Key, T, Compare, Alloc> & rhs)
	{
			return (rhs < lhs);
	}

	template<class Key, class T, class Compare, class Alloc>
	bool operator<=(const map<Key, T, Compare, Alloc> & lhs, const map<Key, T, Compare, Alloc> & rhs)
	{
			return !(lhs > rhs);
	}

	template<class Key, class T, class Compare, class Alloc>
	bool operator>=(const map<Key, T, Compare, Alloc> & lhs, const map<Key, T, Compare, Alloc> & rhs)
	{
			return !(lhs < rhs);
	}

	template<class Key, class T, class Compare, class Alloc>
	void swap(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}

};

#endif
