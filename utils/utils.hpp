#ifndef UTILS_HPP
# define UTILS_HPP

# include "../iter/iterator.hpp"

namespace ft
{
// nullptr
	const class s_nullptr
	{
	private:
		void operator&() const;
	public:
		template<class T>
		operator T*() const { 
			return 0; 
		}

		template<class C, class T>
		operator T C::*() const { 
			return 0; 
		}
	} t_nullptr = {};

// enable_if (reimplemented)
	template <bool B, class T = void>
	struct enable_if {};

	template <class T>
	struct enable_if<true, T> { typedef T type;	};

//is_integral (reimplemented)
	template< class T, T v >
	struct integral_constant 
	{
		static const bool value = v;
		typedef T value_type;
		typedef integral_constant<T, v> type;
		operator T() { return v; }
	};

	typedef integral_constant<bool, false> false_type;
	typedef integral_constant<bool, true> true_type;

	template< class T >	struct is_integral : public false_type {};
	template< class T >	struct is_integral<const T> : public is_integral<T> {};
	template< class T >	struct is_integral<volatile const T> : public is_integral<T> {};
	template< class T >	struct is_integral<volatile T> : public is_integral<T> {};
	template< >	struct is_integral<unsigned char> : public true_type {};
	template< >	struct is_integral<unsigned short> : public true_type {};
	template< >	struct is_integral<unsigned int> : public true_type {};
	template< > struct is_integral<unsigned long> : public true_type {};
	template< > struct is_integral<signed char> : public true_type {};
	template< >	struct is_integral<short> : public true_type {};
	template< > struct is_integral<int> : public true_type {};
	template< >	struct is_integral<long> : public true_type {};
	template< >	struct is_integral<char> : public true_type {};
	template< >	struct is_integral<bool> : public true_type {};
	template< >	struct is_integral<char32_t> : public true_type {};
	template< >	struct is_integral<char16_t> : public true_type {};
	template< >	struct is_integral<wchar_t> : public true_type {};
	template< >	struct is_integral<long long> : public true_type {};
	template< >	struct is_integral<unsigned long long> : public true_type {};

//lexicographical compare (reimplemented)
	template<class InputIt1, class InputIt2>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
								InputIt2 first2, InputIt2 last2)
	{
		for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
			if (*first1 < *first2) return true;
			if (*first2 < *first1) return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

	template<class InputIt1, class InputIt2, class Compare>
	bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
								InputIt2 first2, InputIt2 last2,
								Compare comp)
	{
		for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
			if (comp(*first1, *first2)) return true;
			if (comp(*first2, *first1)) return false;
		}
		return (first1 == last1) && (first2 != last2);
	}

//equal (reimplemented)
	template< class InputIterator1, class InputIterator2 >
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while (first1!=last1) {
			if (!(*first1==*first2))
				return false;
			++first1;
			++first2;
		}
		return true;
	}

	template< class InputIterator1, class InputIterator2, class BinaryPredicate >
	bool equal(InputIterator1 first1, InputIterator1 last1,
			InputIterator2 first2, BinaryPredicate pred)
	{
		while (first1!=last1) {
			if (!pred(*first1, *first2))
				return false;
			++first1;
			++first2;
		}
		return true;
	}

//pair (reimplemented)
	template<typename T1, typename T2>
	struct pair 
	{
	public :
		typedef T1 first_type;
		typedef T2 second_type;
		
		T1 first;
		T2 second;

		pair():
			first(),
			second()
		{}

		template<class U, class V>
		pair (const pair<U, V>& p):
			first(p.first),
			second(p.second)
		{}

		pair(const T1& x, const T2& y):
			first(x),
			second(y)
		{}

		pair& operator= (const pair& p)
		{
			if (*this == p)
				return *this;
			first = p.first;
			second = p.second;
			return *this;		
		}

		friend bool operator==(const pair<T1, T2>& x, const pair<T1, T2>& y)
		{
			return x.first == y.first && x.second == y.second;
		}

		friend bool operator!=(const pair<T1, T2>& x, const pair<T1, T2>& y)
		{
			return !(x == y);
		}

		friend bool operator<(const pair<T1,T2>& x, const pair<T1,T2>& y)
		{
			return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
		};

		friend bool operator<=(const pair<T1,T2>& x, const pair<T1,T2>& y)
		{
			return !(y < x);
		};

		friend bool operator>(const pair<T1,T2>& x, const pair<T1,T2>& y)
		{
			return (y < x);
		};

		friend bool operator>=(const pair<T1,T2>& x, const pair<T1,T2>& y)
		{
			return !(x < y);
		};
	};

	template <class T1, class T2>
	pair<T1, T2> make_pair(T1 x, T2 y) {
		return (pair<T1, T2>(x, y));
	}
}

#endif
