#ifndef RBTREE_ITERATOR_HPP
# define RBTREE_ITERATOR_HPP

# include "../tree/RBTree_Node.hpp"

namespace ft
{
	template<class T> struct remove_const { typedef T type; };
	template<class T> struct remove_const <const T> { typedef T type; };

	template<class Value>
	class RBTree_iterator
	{
	public:
		typedef std::bidirectional_iterator_tag							iterator_category;
		typedef typename ft::iterator_traits<Value*>::value_type		value_type;
		typedef typename ft::iterator_traits<Value*>::reference			reference;
		typedef typename ft::iterator_traits<Value*>::pointer			pointer;
		typedef typename ft::iterator_traits<Value*>::difference_type	difference_type;

		typedef typename ft::remove_const<value_type>::type				clear_value_type;
		typedef RBTree_Node<clear_value_type> 							Node;
		typedef	Node*													node_pointer;

	private:
		node_pointer _node;

		node_pointer maximum(node_pointer node) const
		{
			while (node->_right->_type != nil)
				node = node->_right;
			return node;
		}

		node_pointer minimum(node_pointer node) const
		{
			while (node->_left->_type != nil)
				node = node->_left;
			return node;
		}

		void next()
		{
			if (_node->_type == nil)
				return ;

			if (_node->_right->_type != nil)
			{
				_node = minimum(_node->_right);
				return ;
			}	
			node_pointer y = _node->_parent;
			while (y->_type != nil && _node == y->_right)
			{
				_node = y;
				y = y->_parent;
			}
			_node = y;
		}

		void previous()
		{
			if (_node->_type == nil)
			{
				_node = _node->_parent;
				return ;
			}

			if (_node->_type != nil && _node->_left->_type != nil)
			{
				_node = maximum(_node->_left);
				return ;
			}

			node_pointer y = _node->_parent;
			while (y->_type != nil && _node == y->_left)
			{
				_node = y;
				y = y->_parent;
			}

			if (y->_type != nil)
				_node = y;
		}

	public:
		RBTree_iterator() {}

		RBTree_iterator(void *node): _node(static_cast<node_pointer>(node)) {}

		RBTree_iterator(const RBTree_iterator<clear_value_type> & copy)
		{
			*this = copy;
		}

		RBTree_iterator& operator=(const RBTree_iterator<clear_value_type>& copy) {
			_node = copy.node();
			return *this;
		}

		node_pointer node() const
		{
			return _node;
		}

		reference operator*() const
		{
			return *(_node->_value);
		}

		pointer operator->() const
		{
			return _node->_value;
		}


		RBTree_iterator operator++(int)
		{
			RBTree_iterator tmp(*this);
			next();
			return (tmp);
		}

		RBTree_iterator &operator++()
		{
			next();
			return (*this);
		}

		RBTree_iterator operator--(int)
		{
			RBTree_iterator tmp(*this);
			previous();
			return (tmp);
		}

		RBTree_iterator &operator--()
		{
			previous();
			return (*this);
		}
	};	

	template<typename A, typename B>
	bool operator==(const RBTree_iterator<A> & lhs, const RBTree_iterator<B> & rhs)
	{
		return (lhs.node() == rhs.node());
	}

	template<typename A, typename B>
	bool operator!=(const RBTree_iterator<A> & lhs, const RBTree_iterator<B> & rhs)
	{
		return (lhs.node() != rhs.node());
	}
}

#endif
