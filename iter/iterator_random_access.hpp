#ifndef ITERATOR_RANDOM_ACCESS_HPP
# define ITERATOR_RANDOM_ACCESS_HPP

# include "iterator.hpp"

namespace ft
{
	template <class Iterator>
	class random_access_iterator
	{
	public:
		typedef	typename 	iterator_traits<Iterator>::value_type 				value_type;
		typedef 			std::ptrdiff_t										difference_type;
		typedef typename	ft::iterator_traits<Iterator>::pointer				pointer;
		typedef typename	ft::iterator_traits<Iterator>::reference			reference;
		typedef typename 	ft::iterator_traits<Iterator*>::iterator_category	iterator_category;
		typedef 			pointer												iterator_type;
	
	private:
		Iterator	_current;
	public:
		random_access_iterator(): _current()
		{}

		explicit random_access_iterator(pointer x): _current(x)
		{}

		template <class U> 
		random_access_iterator(const random_access_iterator<U>& u): _current(u.base())
		{}

		template <class U>
		random_access_iterator& operator=(const random_access_iterator<U>& u)
		{
			_current = u.base();
			return *this;
		}

		virtual ~random_access_iterator() 
		{}

		Iterator base() const
		{ 
			return _current;
		}

		reference operator*() const
		{
			return *_current;
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		random_access_iterator& operator++()
		{
			++_current;
			return *this;
		}

		random_access_iterator operator++(int)
		{
			random_access_iterator tmp = *this;
			++_current;
			return tmp;
		}

		random_access_iterator& operator--()
		{
			--_current;
			return *this;			
		}

		random_access_iterator operator--(int)
		{
			random_access_iterator tmp = *this;
			--_current;
			return tmp;
		}

		random_access_iterator operator+ (difference_type n) const
		{
			return random_access_iterator(_current + n);
		}

		random_access_iterator& operator+=(difference_type n)
		{
			_current += n;
			return *this;
		}

		random_access_iterator operator- (difference_type n) const
		{
			return random_access_iterator(_current - n);
		}

		random_access_iterator& operator-=(difference_type n)
		{
			_current -= n;
			return *this;
		}

		reference operator[](difference_type n) const
		{
			return _current[n];
		}
	};

	template <typename Iterator1, typename Iterator2>
	bool operator==(const random_access_iterator<Iterator1>& x, const random_access_iterator<Iterator2>& y)
	{
		return x.base() == y.base();
	}

	template <typename Iterator1, typename Iterator2>
	bool operator<(const random_access_iterator<Iterator1>& x, const random_access_iterator<Iterator2>& y)
	{
		return x.base() < y.base();
	}

	template <typename Iterator1, typename Iterator2>
	bool operator!=(const random_access_iterator<Iterator1>& x, const random_access_iterator<Iterator2>& y)
	{
		return x.base() != y.base();
	}

	template <typename Iterator1, typename Iterator2>
	bool operator>(const random_access_iterator<Iterator1>& x, const random_access_iterator<Iterator2>& y)
	{
		return x.base() > y.base();
	}

	template <typename Iterator1, typename Iterator2>
		bool operator>=(const random_access_iterator<Iterator1>& x, const random_access_iterator<Iterator2>& y)
	{
		return x.base() >= y.base();
	}

	template <typename Iterator1, typename Iterator2>
	bool operator<=(const random_access_iterator<Iterator1>& x, const random_access_iterator<Iterator2>& y)
	{
		return x.base() <= y.base();
	}
}

#endif
