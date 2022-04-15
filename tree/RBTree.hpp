#ifndef RBTREE_HPP
# define RBTREE_HPP

# include <memory>
# include <iostream>
# include "../iter/iterator_reverse.hpp"
# include "../iter/RBTree_iterator.hpp"
# include "../utils/utils.hpp"

namespace ft
{
	template<class Value, 
				class Compare = std::less<Value>,
				class Allocator = std::allocator<Value> >
	class RBTree
	{
	public:
		typedef				Value											value_type;
		typedef				Compare											value_compare;
		typedef				Allocator										allocator_type;
		typedef typename	allocator_type::reference						reference;
		typedef typename	allocator_type::const_reference					const_reference;
		typedef typename	allocator_type::pointer							pointer;
		typedef typename	allocator_type::const_pointer					const_pointer;
		typedef typename 	allocator_type::size_type						size_type;

		typedef				RBTree_Node<Value>								Node;
		typedef				Node*											node_pointer;
		typedef typename	allocator_type::template rebind<Node>::other	allocator_node;

		typedef				ft::RBTree_iterator<Value>						iterator;
		typedef				ft::RBTree_iterator<const Value>				const_iterator;
		typedef				ft::reverse_iterator<iterator>					reverse_iterator;
		typedef				ft::reverse_iterator<const_iterator>			const_reverse_iterator;

	public:
		allocator_node	_alloc_node;
		allocator_type	_alloc_value;
		node_pointer	_nil;
		node_pointer	_root;
		value_compare	_comp;
		size_t			_size;

	public:
// construct/copy/destroy:	
		RBTree(const Compare &comp, const allocator_type& alloc = allocator_type()):
			_alloc_node(allocator_node()),
			_alloc_value(alloc),
			_nil(_alloc_node.allocate(1)),
			_root(_nil),
			_comp(comp),
			_size(0)
		{
			_alloc_node.construct(_nil, Node(_nil, _nil, _nil, nil));
		}

		RBTree():
			_alloc_node(allocator_node()),
			_alloc_value(allocator_type()),
			_nil(_alloc_node.allocate(1)),
			_root(_nil),
			_comp(value_compare()),
			_size(0)
		{
			_alloc_node.construct(_nil, Node(_nil, _nil, _nil, nil));
		}

		RBTree(const RBTree& src):
			_nil(_alloc_node.allocate(1)),
			_root(_nil),
			 _comp(src._comp)
		{
			_alloc_node.construct(_nil, Node(_nil, _nil, _nil, nil));
			*this = src;
		}

		RBTree& operator=(const RBTree & src){
			if (this == &src)
				return *this;

			clear_node(_root);
			_alloc_node.destroy(_nil);
			_alloc_node.deallocate(_nil, 1);
	
			_alloc_node = src._alloc_node;
			_alloc_value = src._alloc_value;
			_comp = src._comp;

			_nil = _alloc_node.allocate(1);
			_alloc_node.construct(_nil, *(src._nil));
			_root = _nil;

			//for (const_iterator it = src.begin(); it != src.end(); it++)
			//	insert(*it);

			if (src._size > 0)
			{
				_root = copy_node(src._root);
				_root->_parent = _nil;
				copy_all(_root, src._root);
			}
			_size = src._size;
	
			return *this;
		}

		node_pointer copy_node(node_pointer other)
		{
			node_pointer new_node = _alloc_node.allocate(1);
			_alloc_node.construct(new_node, Node(other->_parent, other->_left, other->_right, other->_type));
			if (other->_value)
			{
				new_node->_value = _alloc_value.allocate(1);
				_alloc_value.construct(new_node->_value, *other->_value);
			}
			return (new_node);
		}

		void	copy_all(node_pointer my_node, node_pointer other){				
			if (other->_left->_type == nil)
				my_node->_left = _nil;
			else 
			{
				my_node->_left = copy_node(other->_left);
				my_node->_left->_parent = my_node;
				copy_all(my_node->_left, other->_left);
			}
			if (other->_right->_type == nil)
				my_node->_right = _nil;
			else
			{
				my_node->_right = copy_node(other->_right);
				my_node->_right->_parent = my_node;
				copy_all(my_node->_right, other->_right);
			}
		}

		~RBTree()
		{
			clear_node(_root);
			_alloc_node.deallocate(_nil, 1);
		}

		allocator_type get_allocator() const
		{
			return (_alloc_value);
		}

// iterators:
		iterator end() { return iterator(_nil); }
		const_iterator end() const { return const_iterator(_nil); }
		iterator begin() { return iterator(tree_min(_root)); }
		const_iterator	begin() const{ return const_iterator(tree_min(_root)); }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

// capacity:
		bool empty() const { return _size == 0; }
		size_type size() const { return _size; }
		size_type max_size() const { return (_alloc_value.max_size()); }

// modifiers:
		ft::pair<node_pointer, bool> insert(value_type const &value)
		{
//http://algolist.manual.ru/ds/rbtree.php
			node_pointer current, parent, x;
			current = _root;
			parent = _nil;

			while (current != _nil)
			{
				parent = current;
				if (_comp(value, *(parent->_value)))
					current = current->_left;
				else if (_comp(*(parent->_value), value))
					current = current->_right;
				else
					return ft::pair<node_pointer, bool>(current, false);
			}

			pointer new_val = _alloc_value.allocate(1);
		 	_alloc_value.construct(new_val, value);

			x = _alloc_node.allocate(1);
			_alloc_node.construct(x, Node(parent, _nil, _nil, red, new_val));
			
			if (parent != _nil)
			{		
				if (_comp(value, *(parent->_value)))
					parent->_left = x;
				else
					parent->_right = x;
			}
			else
			{		
				_root = x;
				_nil->_parent = x;
			}

			_nil->_parent = tree_max(_root);
			_insertFixup(x);
			++_size;
			return ft::pair<node_pointer, bool>(x, true);		
		}

		bool erase(const value_type& value)
		{
			node_pointer pos = _search(value, _root);
			if (pos == _nil) 
				return false;
//http://algolist.manual.ru/ds/rbtree.php
			/*****************************
			*  delete node z from tree  *
			*****************************/
			node_pointer x, y;

			if (pos->_left == _nil || pos->_right == _nil) 
			{		
				/* y has a NIL node as a child */
				y = pos;
			}
			else 
			{
				/* find tree successor with a NIL node as a child */
				y = pos->_right;
				while (y->_left != _nil) 
					y = y->_left;
			}

			/* x is y's only child */
			if (y->_left != _nil)
				x = y->_left;
			else
				x = y->_right;

			/* remove y from the parent chain */
			if (x != _nil)
				x->_parent = y->_parent;
			if (y->_parent != _nil)
				if (y == y->_parent->_left)
					y->_parent->_left = x;
				else
					y->_parent->_right = x;
			else
				_root = x;

			if (y != pos)
			{
				_alloc_value.destroy(pos->_value);
				_alloc_value.deallocate(pos->_value, 1);
				pos->_value = y->_value;
			}
			else
			{
				_alloc_value.destroy(y->_value);
				_alloc_value.deallocate(y->_value, 1);
			}

			_alloc_node.destroy(y);
			_alloc_node.deallocate(y, 1);

			_nil->_parent = tree_max(_root);
			_deleteFixup(x);
			_size--;
			return true;		
		}

		void swap(RBTree &other)
		{
			node_pointer tmpNil = _nil;
			node_pointer tmpHead = _root;
			Compare tmpCmp = _comp;
			size_t tmpSize = _size;

			_nil  = other._nil;
			_root = other._root;
			_comp = other._comp;
			_size = other._size;

			other._nil  = tmpNil;
			other._root = tmpHead;
			other._comp = tmpCmp;
			other._size = tmpSize;
		}

		void clear()
		{
			clear_node(_root);
			_root = _nil;
			_nil->_parent = _nil;
			_size = 0;
		}

// observers:
		value_compare value_comp() const { return _comp; }

// operations:
		iterator find(const value_type& value)
		{
			node_pointer find_res = _search(value, _root);
			return (find_res == NULL ? end() : iterator(find_res));
		}

		const_iterator find(const value_type& value) const
		{
			node_pointer find_res = _search(value, _root);
			return (find_res == NULL ? end() : const_iterator(find_res));
		}

		size_type count(const value_type& value) const
		{
			return (find(value) != end());
		}

		iterator lower_bound(const value_type& value)
		{
			iterator last = end();
			for (iterator first = begin(); first != last; ++first)
				if(!_comp(*first, value))
					return (first);
			return (last);
		}

		const_iterator lower_bound(const value_type& value) const
		{
			const_iterator last = end();
			for (const_iterator first = begin(); first != last; ++first)
				if(!_comp(*first, value))
					return (first);
			return (last);
		}

		iterator upper_bound(const value_type& value)
		{
			iterator last = end();
			for (iterator first = begin(); first != last; ++first)
				if(_comp(value, *first))
					return (first);
			return (last);
		}

		const_iterator upper_bound(const value_type& value) const
		{
			const_iterator last = end();
			for (const_iterator first = begin(); first != last; ++first)
				if(_comp(value, *first))
					return (first);
			return (last);
		}

		ft::pair<iterator, iterator> equal_range(const value_type &value)
		{
			return (ft::make_pair(lower_bound(value), upper_bound(value)));
		}

		pair<const_iterator, const_iterator> equal_range(const value_type &value) const
		{
			return (ft::make_pair(lower_bound(value), upper_bound(value)));
		}

	private:
		node_pointer	tree_min(node_pointer node) const
		{
			while (node != _nil && node->_left != _nil) 
				node = node->_left;
			return node;
		}
		
		node_pointer	tree_max(node_pointer node) const
		{
			while (node != _nil && node->_right != _nil) 
				node = node->_right;
			return node;
		}

		void clear_node(node_pointer node)
		{
			if (node != _nil)
			{
				clear_node(node->_right);
				clear_node(node->_left);
				_alloc_value.destroy(node->_value);
				_alloc_value.deallocate(node->_value, 1);
				_alloc_node.deallocate(node, 1);
			}
		}

//http://algolist.manual.ru/ds/rbtree.php		
		node_pointer _search(const value_type &value, node_pointer node) const
		{
			if(!node || node == _nil)
				return node_pointer(_nil);
			if (_comp(value, *node->_value))
				return _search(value, node->_left);
			if (_comp(*node->_value, value))
				return _search(value, node->_right);
			return node;
		}

//http://algolist.manual.ru/ds/rbtree.php
		void _rotateLeft(node_pointer x) 
		{
			/**************************
			*  rotate node x to left *
			**************************/

			node_pointer y = x->_right;

			/* establish x->right link */
			x->_right = y->_left;
			if (y->_left != _nil) 
				y->_left->_parent = x;

			/* establish y->parent link */
			if (y != _nil) 
				y->_parent = x->_parent;
			if (x->_parent != _nil) 
			{
				if (x == x->_parent->_left)
					x->_parent->_left = y;
				else
					x->_parent->_right = y;
			} 
			else 
			{
				_root = y;
			}

			/* link x and y */
			y->_left = x;
			if (x != _nil) 
				x->_parent = y;
		}

//http://algolist.manual.ru/ds/rbtree.php
		void _rotateRight(node_pointer x) 
		{
			/****************************
			*  rotate node x to right  *
			****************************/

			node_pointer y = x->_left;

			/* establish x->left link */
			x->_left = y->_right;
			if (y->_right != _nil) 
				y->_right->_parent = x;

			/* establish y->parent link */
			if (y != _nil) 
				y->_parent = x->_parent;
			if (x->_parent != _nil)
			{
				if (x == x->_parent->_right)
					x->_parent->_right = y;
				else
					x->_parent->_left = y;
			} 
			else 
			{
				_root = y;
			}

			/* link x and y */
			y->_right = x;
			if (x != _nil) 
				x->_parent = y;
		}

//http://algolist.manual.ru/ds/rbtree.php
		void _insertFixup(node_pointer x) 
		{
			/*************************************
			*  maintain Red-Black tree balance  *
			*  after inserting node x           *
			*************************************/

			/* check Red-Black properties */
			while (x != _root && x->_parent->_type == red) 
			{
				/* we have a violation */
				if (x->_parent == x->_parent->_parent->_left) 
				{
					node_pointer y = x->_parent->_parent->_right;
					if (y->_type == red) 
					{
						/* uncle is RED */
						x->_parent->_type = black;
						y->_type = black;
						x->_parent->_parent->_type = red;
						x = x->_parent->_parent;
					} 
					else 
					{
						/* uncle is BLACK */
						if (x == x->_parent->_right) 
						{
							/* make x a left child */
							x = x->_parent;
							_rotateLeft(x);
						}
						/* recolor and rotate */
						x->_parent->_type = black;
						x->_parent->_parent->_type = red;
						_rotateRight(x->_parent->_parent);
					}
				} 
				else 
				{
					/* mirror image of above code */
					node_pointer y = x->_parent->_parent->_left;
					if (y->_type == red) 
					{
						/* uncle is RED */
						x->_parent->_type = black;
						y->_type = black;
						x->_parent->_parent->_type = red;
						x = x->_parent->_parent;
					} 
					else 
					{
						/* uncle is BLACK */
						if (x == x->_parent->_left) 
						{
							x = x->_parent;
							_rotateRight(x);
						}
						x->_parent->_type = black;
						x->_parent->_parent->_type = red;
						_rotateLeft(x->_parent->_parent);
					}
				}
			}
			_root->_type = black;
		}

//http://algolist.manual.ru/ds/rbtree.php
		void _deleteFixup(node_pointer x)
		{
			/*************************************
			 *  maintain Red-Black tree balance  *
			 *  after deleting node x            *
			 *************************************/

			while (x != _root && x->_type == black) 
			{
				if (x == x->_parent->_left)
				{
					node_pointer w = x->_parent->_right;
					if (w->_type == red) 
					{
						w->_type = black;
						x->_parent->_type = red;
						_rotateLeft(x->_parent);
						w = x->_parent->_right;
					}
					if (w->_left->_type==black && w->_right->_type==black)
					{
						w->_type = red;
						x = x->_parent;
					}
					else
					{
						if (w->_right->_type==black) 
						{
							w->_left->_type = black;
							w->_type = red;
							_rotateRight(w);
							w = x->_parent->_right;
						}
						w->_type = x->_parent->_type;
						x->_parent->_type = black;
						w->_right->_type = black;
						_rotateLeft(x->_parent);
						x = _root;
					}
				}
				else 
				{
					node_pointer w = x->_parent->_left;
					if (w->_type==red)
					{
						w->_type = black;
						x->_parent->_type = red;
						_rotateRight(x->_parent);
						w = x->_parent->_left;
					}
					if (w->_right->_type == black && w->_left->_type==black)
					{
						w->_type = red;
						x = x->_parent;
					}
					else
					{
						if (w->_left->_type==black)
						{
							w->_right->_type = black;
							w->_type = red;
							_rotateLeft(w);
							w = x->_parent->_left;
						}
						w->_type = x->_parent->_type;
						x->_parent->_type = black;
						w->_left->_type = black;
						_rotateRight(x->_parent);
						x = _root;
					}
				}
			}
			if (x->_type != nil)
				x->_type = black;
		}

	};

	template<class Content, class Compare, class Alloc>
	bool operator<(const RBTree<Content, Compare, Alloc>& lhs,  const RBTree<Content, Compare, Alloc>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template<class Content, class Compare, class Alloc>
	bool operator>(const RBTree<Content, Compare, Alloc>& lhs,  const RBTree<Content, Compare, Alloc>& rhs)
	{
		return (lhs < rhs);
	}

	template<class Content, class Compare, class Alloc>
	bool operator==(const RBTree<Content, Compare, Alloc>& lhs, const RBTree<Content, Compare, Alloc>& rhs)
	{
		return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}
}
	
#endif
