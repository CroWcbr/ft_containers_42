#ifndef RBTREE_NODE_HPP
# define RBTREE_NODE_HPP

namespace ft 
{
	enum NodeType 
	{
		black,
		red,
		nil
	};

	template<class Value>
	class RBTree_Node
	{
	public:
		typedef	RBTree_Node*	node_pointer;

		node_pointer	_parent;
		node_pointer	_left;
		node_pointer	_right;
		NodeType		_type;
		Value*			_value;

		 RBTree_Node(node_pointer parent,
					node_pointer left,
					node_pointer right,
					NodeType type = black,
					Value	*value = NULL):
			_parent(parent),
			_left(left),
			_right(right),
			_type(type),
			_value(value)
		{}

		RBTree_Node(RBTree_Node const &copy)
		{
			*this = copy;
		}

		RBTree_Node& operator=(const RBTree_Node& copy)
		{
			if (this == &copy)
				return *this;		
			_parent = copy._parent;
			_left = copy._left;
			_right = copy._right;
			_value = copy._value;
			_type = copy._type;
			return *this;
		}	

		~RBTree_Node()
		{}
	};

}

#endif
