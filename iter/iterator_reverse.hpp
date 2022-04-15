#ifndef ITERATOR_REVERSE_HPP
# define ITERATOR_REVERSE_HPP

# include "iterator.hpp"

namespace ft
{
	template <class Iterator>
	class reverse_iterator
	{
	public:
		typedef	Iterator	iterator_type;
		typedef	typename	ft::iterator_traits<Iterator>::value_type			value_type;
		typedef typename	ft::iterator_traits<Iterator>::difference_type		difference_type;
		typedef typename	ft::iterator_traits<Iterator>::pointer				pointer;
		typedef typename	ft::iterator_traits<Iterator>::reference			reference;
		typedef typename 	ft::iterator_traits<Iterator>::iterator_category 	iterator_category;
	private:
		Iterator	_current;
	public:
		reverse_iterator(): _current()
		{}

		explicit reverse_iterator(Iterator x): _current(x)
		{}

		template <class U> 
		reverse_iterator(const reverse_iterator<U>& u): _current(u.base())
		{}

		template <class U>
		reverse_iterator& operator=(const reverse_iterator<U>& u) 
		{
			_current = u.base();
			return *this;
		}

		virtual ~reverse_iterator()
		{}

		Iterator base() const { 
			return _current;
		}

		reference operator*() const
		{
			Iterator tmp = _current;
			return *--tmp;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		reverse_iterator& operator++()
		{
			--_current;
			return *this;
		}

		reverse_iterator operator++(int)
		{
			reverse_iterator tmp = *this;
			--_current;
			return tmp;
		}

		reverse_iterator& operator--()
		{
			++_current;
			return *this;			
		}

		reverse_iterator operator--(int)
		{
			reverse_iterator tmp = *this;
			++_current;
			return tmp;
		}

		reverse_iterator operator+ (difference_type n) const
		{
			return reverse_iterator(_current - n);
		}

		reverse_iterator& operator+=(difference_type n)
		{
			_current -= n;
			return *this;
		}

		reverse_iterator operator- (difference_type n) const
		{
			return reverse_iterator(_current + n);
		}

		reverse_iterator& operator-=(difference_type n)
		{
			_current += n;
			return *this;
		}

		reference operator[](difference_type n) const
		{
			return _current[-n - 1];
		}
	};

	template <typename Iterator1, typename Iterator2>
	bool operator==(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() == y.base();
	}

	template <typename Iterator1, typename Iterator2>
	bool operator<(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() < y.base();
	}

	template <typename Iterator1, typename Iterator2>
	bool operator!=(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() != y.base();
	}

	template <typename Iterator1, typename Iterator2>
	bool operator>(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() > y.base();
	}

	template <typename Iterator1, typename Iterator2>
	bool operator>=(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() >= y.base();
	}

	template <typename Iterator1, typename Iterator2>
	bool operator<=(const reverse_iterator<Iterator1>& x, const reverse_iterator<Iterator2>& y)
	{
		return x.base() <= y.base();
	}
}

#endif
