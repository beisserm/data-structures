#ifndef INCLUDE_ARRAYARRAYLIST_HPP_
#define INCLUDE_ARRAYARRAYLIST_HPP_

#include <limits>
#include <memory>

#define UNIT_TEST 1

#ifdef UNIT_TEST
#include <iostream>
#endif

/**
 * Access:  O(1) (array lookup)
 * Insert:  O(n) (might have to shift all elements to right)
 * Removal: O(n) (might have to shift all elements to left)
 * Add:     O(n) (list could be full and have to copy everything)
 */

template<typename T>
class ArrayList
{
	public:
		class iterator
		{
			// Do not inherit from std::iterator it is deprecated in c++17
		    using value_type = T;
		    using pointer    = T*;
		    using reference  = T&;
		    using difference_type = std::ptrdiff_t;

			// Iteration Type:
			//   - Random AccessIterator
			//     - Bidirectional Iterator
			//       - Forward Iterator
			//         - Output Iterator
			//         - Input Iterator
		    using iterator_category = std::random_access_iterator_tag;

		    // Pretty sure we could also do:
//		    typedef T                               value_type;
//		    typedef std::ptrdiff_t                  difference_type;
//		    typedef T*                              pointer;
//		    typedef T&                              reference;
//		    typedef std::random_access_iterator_tag iterator_category;

		    iterator() = default;

		    iterator(ArrayList<T>* data, size_t index)
		    {
		    	mData = data;
		    	mCurrentIndex = index;
		    }
		    // Default Copy/Move Are Fine.
		    // Default Destructor fine.

		    reference operator*()
		    {
		    	return (*mData)[mCurrentIndex];
		    }

		    const reference operator*() const
		    {
		    	return (*mData)[mCurrentIndex];
		    }

		    pointer operator->()
		    {
		    	return &((*mData)[mCurrentIndex]);
		    }

		    const pointer operator->() const
		    {
		    	return &((*mData)[mCurrentIndex]);
		    }

		    reference operator[](int m)
		    {
		    	return (*mData)[mCurrentIndex + m];
		    }

		    const reference operator[](int m) const
		    {
		    	return (*mData)[mCurrentIndex + m];
		    }

		    // Most of these are canonical
		    iterator& operator++()
			{
		    	++mCurrentIndex;
		    	return *this;
			}

		    iterator& operator--()
			{
		    	--mCurrentIndex;
		    	return *this;
			}

		    // Post increment
		    // Post increment returns a copy of itself, and THEN increments itself.
		    iterator operator++(int)
			{
		    	iterator other(*this);
		    	++mCurrentIndex;
		    	return other;
			}

		    // Post decrement
		    // Post decrement returns a copy of itself, and THEN decrements itself.
		    iterator operator--(int)
			{
		    	iterator other(*this);
		    	--mCurrentIndex;
		    	return other;
			}

		    iterator& operator+=(int n)
			{
		    	mCurrentIndex += n;
		    	return *this;
			}

		    iterator& operator-=(int n)
			{
		    	mCurrentIndex -= n;
		    	return *this;
			}

		    iterator operator+(int n) const
		    {
		    	iterator r(*this);
		    	return r += n;
		    }

		    iterator operator-(int n) const
		    {
		    	iterator r(*this);
		    	return r -= n;
		    }

		    difference_type operator-(iterator const& r) const
		    {
		    	return mCurrentIndex - r.mCurrentIndex;
		    }

		    // Comparing iterator from different containers is undefined behavior so don't check
		    // These canonical (and maybe not needed?
		    bool operator<(iterator const& other) const
		    {
		    	return mCurrentIndex <  other.mCurrentIndex;
		    }

		    bool operator<=(iterator const& other) const
			{
		    	return mCurrentIndex <= other.mCurrentIndex;
			}

		    bool operator>(iterator const& other) const
		    {
		    	return mCurrentIndex >  other.mCurrentIndex;
		    }

		    bool operator>=(iterator const& other) const
			{
		    	return mCurrentIndex >= other.mCurrentIndex;
			}

		    bool operator!=(const iterator& other) const
			{
		    	return mCurrentIndex != other.mCurrentIndex;
			}

		    bool operator==(const iterator& other) const
			{
		    	return mCurrentIndex == other.mCurrentIndex;
			}

		private:
		    ArrayList<T>* mData = nullptr;
		    size_t        mCurrentIndex = 0;
		};

		// Default constructor
		ArrayList()
		{
#ifdef UNIT_TEST
			std::cout << "Default constructor" << std::endl;
#endif
		}

		ArrayList(const std::initializer_list<T>& il)
		{
#ifdef UNIT_TEST
			std::cout << "Initializer list constructor" << std::endl;
#endif

			for(const T& val : il)
			{
				this->push_back(val);
			}
		}

		// User defined constructor
		ArrayList(T contents[], size_t listSize)
		{
#ifdef UNIT_TEST
			std::cout << "User defined constructor" << std::endl;
#endif
			mCurrentSize = listSize;
			mMaxSize = listSize * 2;
			mContents = copyArray(contents, listSize, mMaxSize);
		}

		// Copy constructor
		ArrayList(const ArrayList<T>& other)
		{
#ifdef UNIT_TEST
			std::cout << "Copy constructor" << std::endl;
#endif
			mCurrentSize = other.mCurrentSize;
			mMaxSize = other.mMaxSize;
			mContents = copyArray(other.mContents.get(), other.mMaxSize, other.mMaxSize);
		}

		// Move constructor should never throw
		ArrayList(ArrayList<T>&& other) noexcept
		{
#ifdef UNIT_TEST
			std::cout << "Move constructor" << std::endl;
#endif
			forwardMove(std::forward<ArrayList<T>>(other));
		}

		/**
		 * Copy assignment
		 * Having ArrayList<T>& operator=(ArrayList<T>& other) is problematic because the compiler can't tell which
		 * function operator=(ArrayList<T>&&) or operator=(ArrayList<T>&) to call. So we lose an optimization
		 * opportunity because of the temporary copy instead of letting the compiler figure things out in the
		 * parameter list.
		 */
		ArrayList<T>& operator=(const ArrayList<T>& other)
		{
#ifdef UNIT_TEST
			std::cout << "Assignment operator" << std::endl;
#endif
			// Get am
			ArrayList<T> temp = other;
			swap(*this, temp);
			return *this;
		}

		/**
		 * Move assignment should never throw
		 * General rules of engagement
		 *  1. Destroy visible resources
		 *  2. Move assign all members
		 *  3. If the move assignment members didn't make the rhs resource-less, then do it
		 */
		ArrayList<T>& operator=(ArrayList<T>&& other) noexcept
		{
#ifdef UNIT_TEST
			std::cout << "Move assignment operator" << std::endl;
#endif
			// Don't want to swap. Temporary variable is going away and assigning something to it would be strange
			// behavior.
			// Also we forward other because && doesn't always mean rvalue reference, it could be a
			// forwarding reference (universal reference).
			forwardMove(std::forward<ArrayList<T>>(other));
			return *this;
		}

		// Comparison operators - non member functions




//	    iterator begin()
//	    {
//
//	    }
//
//	    const iterator cbegin()
//		{
//
//		}
//
//	    rbegin()
//	    crbegin()
//	    end()
//	    cend()
//	    rend()
//	    crend()

		virtual ~ArrayList() noexcept = default;

		/**
		 * Swap function should never throw
		 */
		friend void swap(ArrayList<T>& left, ArrayList<T>& right) noexcept
		{
			// We always just want to call swap and be done with it. We don't want swap to be a member function. So we
			// enable ADL (argument dependent lookup) and when we call swap it will find our friend function because
			// it's a better match
			using std::swap;

			// Everything is a built in data type -> will never throw
			std::swap(left.mCurrentSize, right.mCurrentSize);
			std::swap(left.mMaxSize, right.mMaxSize);
			std::swap(left.mContents, right.mContents);
		}

		// Capacity:
		size_t size() const noexcept
		{
			return mCurrentSize;
		}

		size_t max_size() const noexcept
		{
			return std::numeric_limits<size_t>::max();
		}

		size_t capacity() const noexcept
		{
			return mMaxSize;
		}

		bool empty() const noexcept
		{
			return (mCurrentSize == 0);
		}

		// Element access:

		T& operator[] (size_t index) // throw out_of_range
		{
			return const_cast<T&>(static_cast<const ArrayList<T>*>(this)->operator[](index));
		}

		const T& operator[] (size_t index) const // throw out_of_range
		{
			if(index > mMaxSize)
			{
				throw std::out_of_range("Index out of bounds");
			}

			return mContents[index];
		}

		T& at(size_t index) // throw out_of_range
		{
			return const_cast<T&>(static_cast<const ArrayList<T>*>(this)->at(index));
		}

		const T& at(size_t index) const // throw out_of_range
		{
			if(index > mMaxSize)
			{
				throw std::out_of_range("Index out of bounds");
			}

			return mContents[index];
		}

		T& front()
		{
			return const_cast<T&>(static_cast<const ArrayList<T>*>(this)->front());
		}

		const T& front() const
		{
			if(empty())
			{
				throw std::out_of_range("Empty list");
			}

			return mContents[0];
		}

		T& back() // throw out_of_range
		{
			return const_cast<T&>(static_cast<const ArrayList<T>*>(this)->back());
		}

		const T& back() const // throw out_of_range
		{
			if(empty())
			{
				throw std::out_of_range("Empty list");
			}

			return mContents[mCurrentSize - 1];
		}

		T* data() noexcept
		{
			return mContents.get();
		}

		const T* data() const noexcept
		{
			return static_cast<const T*>(mContents->get());
		}

		// Modifiers

		void push_front (const T& val)
		{
			insert(val, 0);
		}

		void push_front (T&& val)
		{
			insert(val, 0);
		}

		void push_back (const T& val)
		{
			insert(val, mCurrentSize);
		}

		void push_back (T&& val)
		{
			insert(val, mCurrentSize);
		}

		T pop_front()
		{
			return erase(0);
		}

		T pop_back()
		{
			return erase(mCurrentSize);
		}

		void insert(const T& val, std::size_t insertIndex)
		{
			if(insertIndex > mCurrentSize)
			{
				throw std::out_of_range("Index out of bounds");
			}

			// Start at the right and move everything over by 1 until we get to our position we want to insert
			for(size_t i = mCurrentSize; i > insertIndex; i--)
			{
				std::swap(mContents[i], mContents[i-1]);
			}

			mContents[insertIndex] = val;
			mCurrentSize++;

			if(mCurrentSize == mMaxSize)
			{
				mMaxSize = mMaxSize * 2;
				mContents = copyArray(mContents.get(), mCurrentSize, mMaxSize);
			}
		}

		void insert(T&& val, std::size_t insertIndex)
		{
			if(insertIndex > mCurrentSize)
			{
				throw std::out_of_range("Index out of bounds");
			}

			// Start at the right and move everything over by 1 until we get to our position we want to insert
			for(size_t i = mCurrentSize; i > insertIndex; i--)
			{
				std::swap(mContents[i], mContents[i-1]);
			}

			mContents[insertIndex] = std::move(val);
			mCurrentSize++;

			if(mCurrentSize == mMaxSize)
			{
				mMaxSize = mMaxSize * 2;
				mContents = copyArray(mContents.get(), mCurrentSize, mMaxSize);
			}
		}

		void replace(const T& val, std::size_t insertIndex)
		{
			replace(std::forward(val), insertIndex);
		}

		void replace(T&& val, std::size_t insertIndex)
		{
			if(insertIndex > mCurrentSize)
			{
				throw std::out_of_range("Index out of bounds");
			}

			mContents[insertIndex] = std::move(val);
		}

		T erase(std::size_t index)
		{
			if(empty())
			{
				throw std::out_of_range("Empty list");
			}

			if(index > mCurrentSize)
			{
				throw std::out_of_range("Index out of bounds");
			}

			T removed = std::move(mContents[index]);

			// Start at the removal point, and copy everything left by 1
			for(std::size_t i = index; i < mCurrentSize - 1; ++i)
			{
				std::swap(mContents[i], mContents[i+1]);
			}

			mMaxSize--;
			mCurrentSize--;

			// Shrink max amount
			if(mMaxSize / 4 > mCurrentSize)
			{
				mContents = copyArray(mContents.get(), mCurrentSize, mMaxSize / 2);
			}

			return removed;
		}

		void remove(const T& val)
		{
			size_t index = find(val);
			if( index != mMaxSize)
			{
				erase(index);
			}
		}

		size_t find(const T& val) const
		{
			int index = mCurrentSize;

			for(size_t i = 0; i < mCurrentSize; ++i)
			{
				if(val == mContents[i])
				{
					index = i;
					break;
				}
			}

			return index;
		}

		bool contains(const T& data) const
		{
			bool ret = false;

			if(find(data) != mCurrentSize)
			{
				ret = true;
			}

			return ret;

		}

		// Canonical implementation
//		inline bool operator==(const X& lhs, const X& rhs){ /* do actual comparison */ }
//		inline bool operator!=(const X& lhs, const X& rhs){return !operator==(lhs,rhs);}
//		inline bool operator< (const X& lhs, const X& rhs){ /* do actual comparison */ }
//		inline bool operator> (const X& lhs, const X& rhs){return  operator< (rhs,lhs);}
//		inline bool operator<=(const X& lhs, const X& rhs){return !operator> (lhs,rhs);}
//		inline bool operator>=(const X& lhs, const X& rhs){return !operator< (lhs,rhs);}

		//swap functions and move operations should be never-throw if at all possible

	//     rule_of_five(const char* s = "")
	//     : cstring(nullptr)
	//     {
	//         if (s) {
	//             std::size_t n = std::strlen(s) + 1;
	//             cstring = new char[n];      // allocate
	//             std::memcpy(cstring, s, n); // populate
	//         }
	//     }

	//     virtual ~rule_of_five()
	//     {
	//         delete[] cstring;  // deallocate
	//     }

	//     rule_of_five(const rule_of_five& other) // copy constructor
	//     : rule_of_five(other.cstring)
	//     {}

	//     rule_of_five(rule_of_five&& other) noexcept // move constructor
	//     : cstring(std::exchange(other.cstring, nullptr))
	//     {}

	//     rule_of_five& operator=(const rule_of_five& other) // copy assignment
	//     {
	//          return *this = rule_of_five(other);
	//     }

	//     rule_of_five& operator=(rule_of_five&& other) noexcept // move assignment
	//     {
	//         std::swap(cstring, other.cstring);
	//         return *this;
	//     }

	// // alternatively, replace both assignment operators with
	// //  rule_of_five& operator=(rule_of_five other) noexcept
	// //  {
	// //      std::swap(cstring, other.cstring);
	// //      return *this;
	// //  }

	private:
		std::unique_ptr<T[]> copyArray(T contents[], size_t listSize, size_t maxSize)
		{
			std::unique_ptr<T[]> copy = std::make_unique<T[]>(maxSize);

			for(std::size_t i = 0; i < listSize; ++i)
			{
				copy[i] = contents[i];
			}

			return copy;
		}

		void forwardMove(ArrayList<T> && other)
		{
			mCurrentSize = std::move(other.mCurrentSize);
			mMaxSize = std::move(other.mMaxSize);
			mContents = std::move(other.mContents);
		}

		static constexpr size_t DEFAULT_CAPACITY = 8;
		size_t mCurrentSize = 0;
		size_t mMaxSize = DEFAULT_CAPACITY;
		std::unique_ptr<T[]> mContents = std::make_unique<T[]>(DEFAULT_CAPACITY);
};

// Comparison operators
//   - For all operators where you have to choose to either implement them as a member function or a
//     non-member function, use the following rules of thumb to decide:
//
//   - If it is a unary operator, implement it as a member function.
//   - If a binary operator treats both operands equally (it leaves them unchanged), implement this
//     operator as a non-member function.
//   - If a binary operator does not treat both of its operands equally (usually it will change its
//     left operand), it might be useful to make it a member function of its left operand’s type,
//     if it has to access the operand's private parts.

template<typename T>
inline bool operator==(const ArrayList<T>& left, const ArrayList<T>& right)
{
	bool same = true;

	if(left.size() == right.size())
	{
		for(size_t i = 0; i < left.size(); ++i)
		{
			if(left.at(i) != right.at(i))
			{
				same = false;
				break;
			}
		}
	}
	else
	{
		same = false;
	}

	return same;
}

template<typename T>
inline bool operator!=(const ArrayList<T>& left, const ArrayList<T>& right)
{
	return !operator==(left,right);
}

template<typename T>
inline bool operator< (const ArrayList<T>& left, const ArrayList<T>& right)
{
	bool lessThan = true;

	for(size_t i = 0; i < left.size(); ++i)
	{
		if(i < right.size())
		{
			if(left.at(i) > right.at(i))
			{
				lessThan = false;
				break;
			}
		}
	}

	return lessThan;
}

template<typename T>
inline bool operator> (const ArrayList<T>& left, const ArrayList<T>& right)
{
	return  operator< (right,left);
}

template<typename T>
inline bool operator<=(const ArrayList<T>& left, const ArrayList<T>& right)
{
	return !operator> (left,right);
}

template<typename T>
inline bool operator>=(const ArrayList<T>& left, const ArrayList<T>& right)
{
	return !operator< (left,right);
}

#endif /* INCLUDE_ARRAYARRAYLIST_HPP_ */
