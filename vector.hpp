#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include "./utils/utils.hpp"
# include "./iter/iterator_random_access.hpp"
# include "./iter/iterator_reverse.hpp"

namespace ft
{
	template <class T, 
				class Allocator = std::allocator<T> >
	class vector 
	{
	public:
		typedef				T											value_type;
		typedef				Allocator									allocator_type;
		typedef typename	Allocator::pointer							pointer;
		typedef typename	Allocator::const_pointer					const_pointer;
		typedef				std::size_t									size_type;
		typedef typename	Allocator::reference						reference;
		typedef typename	Allocator::const_reference					const_reference;
		typedef				std::ptrdiff_t								difference_type;
		typedef				ft::random_access_iterator<pointer>			iterator;
		typedef				ft::random_access_iterator<const_pointer>	const_iterator;
		typedef				ft::reverse_iterator<iterator>				reverse_iterator;
		typedef				ft::reverse_iterator<const_iterator>		const_reverse_iterator;
	private:
		allocator_type	_alloc;
		pointer			_start;
		pointer			_finish;
		pointer 		_end_of_storage;
	public:
//(1) empty container constructor (default constructor):
		explicit vector(const Allocator& alloc = Allocator()):
			_alloc(alloc),
			_start(t_nullptr),
			_finish(t_nullptr),
			_end_of_storage(t_nullptr)	
		{}
//(2) fill constructor:
		explicit vector(size_type n,
						const value_type& value = value_type(),
						const allocator_type& alloc = allocator_type()):
			_alloc(alloc),
			_start(_alloc.allocate(n)),
			_finish(_start),
			_end_of_storage(_start + n)						
		{
			while (_finish != _end_of_storage)
				_alloc.construct(_finish++, value);
		}
//(3) range constructor:
		template <class InputIterator>
		vector (InputIterator first,
				InputIterator last,
				const allocator_type& alloc = allocator_type(),
				typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = t_nullptr):
			_alloc(alloc)
		{
			if (first > last)
				throw std::length_error("vector");
			_alloc = alloc;
			difference_type n = ft::distance(first, last);
			_start = _alloc.allocate(n);
			_end_of_storage = _start + n;
			_finish = _start;
			while (first != last)
				_alloc.construct(_finish++, *first++);
		}
//(4) copy constructor:
		vector (const vector& x)
		{
			if (x._start > x._finish)
				throw std::length_error("vector");
			_alloc = x._alloc;
			difference_type n = ft::distance(x._start, x._end_of_storage);
			_start = _alloc.allocate(n);
			_end_of_storage = _start + n;
			_finish = _start;
			pointer _x_start = x._start;
			while (_x_start != x._finish)
				_alloc.construct(_finish++, *_x_start++);				
		}
//Destructor:
		~vector()
		{
			clear();
			_alloc.deallocate(_start, capacity());
		}
//Assign content:
		vector &operator=(const vector& x)
		{ 
			if (x == *this)
				return (*this);
			clear();
			_alloc.deallocate(_start, capacity());
			if (x._start > x._finish)
				throw std::length_error("vector");
			_alloc = x._alloc;
			difference_type n = ft::distance(x._start, x._end_of_storage);
			_start = _alloc.allocate(n);
			_end_of_storage = _start + n;
			_finish = _start;
			pointer _x_start = x._start;
			while (_x_start != x._finish)
				_alloc.construct(_finish++, *_x_start++);
			return (*this);
		}

		template <class InputIterator>
		void assign(InputIterator first, 
					InputIterator last,
					typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = t_nullptr)
		{
			if (first > last)
				throw std::length_error("vector::vector");
			clear();
			difference_type n = ft::distance(first, last);
			if ((size_type)n > capacity())
			{
				_alloc.deallocate(_start, capacity());
				_start = _alloc.allocate(n);
				_end_of_storage = _start + n;;
			}
			_finish = _start;
			while (first != last)
				_alloc.construct(_finish++, *first++);
		}

		void assign(size_type n, const T& u)
		{
			clear();
			if ((size_type)n > capacity())
			{
				_alloc.deallocate(_start, capacity());
				_start = _alloc.allocate(n);
				_end_of_storage = _start + n;
			}
			_finish = _start;
			for(size_type i = 0; i < n; i++)
				_alloc.construct(_finish++, u);
		}

//iterators:
		iterator begin() { return iterator(_start);}
		const_iterator begin() const { return const_iterator(_start); }
		iterator end() { return (iterator(_finish)); }
		const_iterator end() const { return const_iterator(_finish); }
		reverse_iterator rbegin(){ return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const{ return const_reverse_iterator(end()); }
		reverse_iterator rend(){ return reverse_iterator(begin()); }
		const_reverse_iterator rend() const{ return const_reverse_iterator(begin()); }

//capacity:
		size_type size() const { return _finish - _start; }
		size_type capacity() const { return _end_of_storage - _start; }
		size_type max_size() const { return allocator_type().max_size(); }

		bool empty() const
		{
			if (size() == 0)
				return true;
			return false;
		}

		void resize(size_type sz, T c = T())
		{
			if (sz > max_size())
				throw (std::length_error("vector"));
			else if (sz < size())
			{
				while (size() > sz)
					_alloc.destroy(--_finish);
			}
			else
			{
				if (capacity() > sz)
					;
				else if (capacity() * 2 < sz)
					reserve(sz);
				else
					reserve(capacity() * 2);
				while (size() < sz)
					_alloc.construct(_finish++, c);
			}
		}

		void reserve(size_type n)
		{
			if (n > max_size())
				throw(std::length_error("vector"));
			else if (n > capacity())
			{
				pointer prev_start = _start;
				pointer prev_start_save = _start;				
				pointer prev_end = _finish;
				size_type prev_capacity = capacity();
				_start = _alloc.allocate(n);
				_end_of_storage = _start + n;
				_finish = _start;
				while (prev_start != prev_end)
				{
					_alloc.construct(_finish++, *prev_start);
					_alloc.destroy(prev_start++);
				}
				_alloc.deallocate(prev_start_save, prev_capacity);
			}
		}

//element access:
		reference operator[](size_type n) { return (*(_start + n)); }
		const_reference operator[](size_type n) const { return (*(_start + n));	}
		reference front() { return (*_start); }
		const_reference front() const { return (*_start); }
		reference back() { return (*(_finish - 1)); }
		const_reference back() const { return (*(_finish - 1)); }

		reference at(size_type n)
		{
			if (n >= size())
				throw (std::length_error("vector"));
			return ((*this)[n]);		
		}

		const_reference at(size_type n) const
		{
			if (n >= size())
				throw (std::length_error("vector"));
			return ((*this)[n]);
		}

//Modifiers:	
		void push_back(const value_type &value)
		{
			if (_finish == _end_of_storage)
			{
				if (size() == 0)
				{
					_alloc.deallocate(_start, capacity());
					_start = _alloc.allocate(1);
					_end_of_storage = _start + 1;
					_finish = _start;
				}
				else
					reserve(size() * 2);
			}
			_alloc.construct(_finish++, value);
		}	

		void pop_back()
		{
			_alloc.destroy(--_finish);
		}

		iterator insert(iterator position, const T& x)
		{
			pointer pos = &(*position);
			if (capacity() == 0)
			{
				_start = _alloc.allocate(1);
				_end_of_storage = _start + 1;
				_finish = _start;
				_alloc.construct(_finish++, x);
				return (iterator(_start));
			}
			else if (capacity() >= size() + 1)
			{
				pointer tmp = _finish;
				while (pos != tmp--)
					_alloc.construct(tmp + 1, *tmp);
				_alloc.construct(tmp, x);
				_finish++;
				return (position);
			}
			else
			{
				difference_type n = pos - _start;
				pointer prev_start = _start;
				pointer prev_start_save = _start;				
				pointer prev_end = _finish;
				size_type prev_capacity = capacity();
				_start = _alloc.allocate(2 * prev_capacity);
				_end_of_storage = _start + 2 * prev_capacity;
				_finish = _start;
				while (prev_start != pos)
				{
					_alloc.construct(_finish++, *prev_start);
					_alloc.destroy(prev_start++);
				}
				_alloc.construct(_finish++, x);
				while (prev_start != prev_end)
				{
					_alloc.construct(_finish++, *prev_start);
					_alloc.destroy(prev_start++);
				}
				_alloc.deallocate(prev_start_save, prev_capacity);
				return (iterator(_start + n));
			}
 		}

		void insert(iterator position, size_type n, const T& x)
		{
			pointer pos = &(*position);
			if (n == 0)
				return ;
			if (size() + n > max_size())
				throw (std::length_error("vector"));	
			if (capacity() >= size() + n)
			{
				pointer tmp = _finish;
				while (pos != tmp--)
					_alloc.construct(tmp + n, *tmp);
				for (size_type i = 0; i < n; i++)
					_alloc.construct(tmp + i, x);
				_finish += n;
			}
			else
			{
				pointer prev_start = _start;
				pointer prev_start_save = _start;
				pointer prev_end = _finish;
				size_type size_save = size();				
				size_type prev_capacity = capacity();
				if (2 * prev_capacity > n + size_save)
				{
					_start = _alloc.allocate(2 * prev_capacity);
					_end_of_storage = _start + 2 * prev_capacity;
				}
				else
				{
					_start = _alloc.allocate(n + size_save);
					_end_of_storage = _start + n + size_save;					
				}
				_finish = _start;
				while (prev_start != pos)
				{
					_alloc.construct(_finish++, *prev_start);
					_alloc.destroy(prev_start++);
				}
				for (size_type i = 0; i < n; i++)
					_alloc.construct(_finish++, x);
				while (prev_start != prev_end)
				{
					_alloc.construct(_finish++, *prev_start);
					_alloc.destroy(prev_start++);
				}
				_alloc.deallocate(prev_start_save, prev_capacity);
			}			
		}

	private:
		template<class InputIt>
		bool check_insert(InputIt first, InputIt last, difference_type n)
		{
			pointer test = _alloc.allocate(n);
			bool result = true;
			int i = 0;
			try 
			{
				for (; first != last; ++i)
					_alloc.construct(test + i, *first++);
			}
			catch (...) 
			{
				result = false;
			}
			for (; i > 0; i--)
				_alloc.destroy(test + i - 1);
			_alloc.deallocate(test, n);
			return result;
		}

	public:
		template <class InputIterator>
		void insert(iterator position,
					InputIterator first,
					InputIterator last,
					typename enable_if<!is_integral<InputIterator>::value>::type* = 0)
		{
			if (position < begin() || position > end() || first > last)
				throw std::logic_error("vector");
			pointer pos = &(*position);	
			difference_type n = ft::distance(first, last);
			if (!check_insert(first, last, n))
				throw (std::length_error("vector"));
			if (n == 0)
				return ;
			if (size() + n > max_size())
				throw (std::length_error("vector"));
			if (capacity() >= size() + n)
			{				
				pointer tmp = _finish;
				while (pos != tmp--)
					_alloc.construct(tmp + n, *tmp);
				for (size_type i = 0; i < (size_type)n; i++)
					_alloc.construct(tmp + i, *first++);
				_finish += n;
			}
			else
			{
				pointer prev_start = _start;
				pointer prev_start_save = _start;
				pointer prev_end = _finish;
				size_type size_save = size();
				size_type prev_capacity = capacity();
				if (2 * prev_capacity > (size_type)n + size_save)
				{
					_start = _alloc.allocate(2 * prev_capacity);
					_end_of_storage = _start + 2 * prev_capacity;
				}
				else
				{
					_start = _alloc.allocate(n + size_save);
					_end_of_storage = _start + n + size_save;					
				}
				_finish = _start;				
				while (prev_start != pos)
				{
					_alloc.construct(_finish++, *prev_start);
					_alloc.destroy(prev_start++);
				}
				while (first != last)
					_alloc.construct(_finish++, *first++);				
				while (prev_start != prev_end)
				{
					_alloc.construct(_finish++, *prev_start);
					_alloc.destroy(prev_start++);
				}
				_alloc.deallocate(prev_start_save, prev_capacity);
			}						
		}

		iterator erase(iterator position)
		{
			pointer pos = &(*position);
			_alloc.destroy(pos);
			while (pos != _finish - 1)
			{
				_alloc.construct(pos, *(pos + 1));
				pos++;
			}
			_alloc.destroy(--_finish);
			return (position);
		}

		iterator erase(iterator first, iterator last)
		{
			if (first > last)
				throw std::length_error("vector");
			pointer f = &(*first);
			pointer l = &(*last);
			pointer tmp = f;
			while (tmp != l)
				_alloc.destroy(tmp++);
			tmp = f;
			while (l != _finish)
				_alloc.construct(tmp++, *l++);
			_finish = tmp;
			return (first);	
		}

		void swap (vector& x)
		{
			pointer save_start = x._start;
			pointer save_finish = x._finish;
			pointer save_end_of_storage = x._end_of_storage;
			allocator_type save_alloc = x._alloc;
			x._start = _start;
			x._finish = _finish;
			x._end_of_storage = _end_of_storage;
			x._alloc = _alloc;
			_start = save_start;
			_finish = save_finish;
			_end_of_storage = save_end_of_storage;
			_alloc = save_alloc;
		}

		void clear() 
		{
			while (_finish != _start)
				_alloc.destroy(--_finish);
		}

//C++11
		pointer data() { return _start; }

//Non-member function overloads
		friend bool operator==(const vector<T,Allocator>& x, const vector<T,Allocator>& y)
		{
			if (x.size() != y.size())
				return false;
			return ft::equal(x.begin(), x.end(), y.begin());
		};

		friend bool operator!=(const vector<T,Allocator>& x, const vector<T,Allocator>& y)
		{
			return (!(x == y));
		};

		friend bool operator< (const vector<T,Allocator>& x, const vector<T,Allocator>& y)
		{
			return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
		}

		friend bool operator> (const vector<T,Allocator>& x,const vector<T,Allocator>& y)
		{
			return (y < x);
		}

		friend bool operator<=(const vector<T,Allocator>& x, const vector<T,Allocator>& y)
		{
			return !(y < x);
		}

		friend bool operator>=(const vector<T,Allocator>& x, const vector<T,Allocator>& y)
		{
			return !(x < y);
		}
	};

	template <class T, class Allocator>
	void swap(vector<T,Allocator>& x, vector<T,Allocator>& y) 
	{
		x.swap(y);
	};
}

#endif
