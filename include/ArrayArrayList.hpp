#ifndef INCLUDE_ARRAYARRAYLIST_HPP_
#define INCLUDE_ARRAYARRAYLIST_HPP_

#include <memory>

template<typename T>
class ArrayList
{
	public:
		// Default constructor
		ArrayList() = default;

		// User defined constructor
		ArrayList(T contents[], size_t listSize)
		{
			mCurrentSize = listSize;
			mMaxSize = listSize * 2;
			mContents = copyArray(contents, listSize, mMaxSize);
		}

		// Copy constructor
		ArrayList(const ArrayList<T>& other)
		{
			mCurrentSize = other.mCurrentSize;
			mMaxSize = other.mMaxSize;
			mContents = copyArray(other.mContents.get(), other.mMaxSize, other.mMaxSize);
		}

		// Move constructor should never throw
		ArrayList(ArrayList<T>&& other) noexcept
		{
			forwardMove(std::forward(other));
		}

		/**
		 * Copy assignment
		 * We do not want: ArrayList<T>& operator=(const ArrayList<T>& other) because
		 * we miss an optimization opportunity. Since we have to make a copy anyways let
		 * the compiler do it in the parameter list
		 */
		ArrayList<T>& operator=(ArrayList<T> other)
		{
			swap(*this, other);
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
			// Don't want to swap. Temporary variable is going away and assigning something to it would be strange
			// behavior.
			// Also we forward other because && doesn't always mean rvalue reference, it could be a
			// forwarding reference (universal reference).
			forwardMove(std::forward(other));
			return *this;
		}

		virtual ~ArrayList() = default;

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
			insert(val, mCurrentSize + 1);
		}

		void push_back (T&& val)
		{
			insert(val, mCurrentSize + 1);
		}

		T pop_front()
		{
			return erase(0);
		}

		T pop_back()
		{
			return erase(mCurrentSize - 1);
		}

//		 If we make insert not const T& val but instead T val
		void insert(const T& val, std::size_t insertIndex)
		{
			// Does this work???
//			insert(std::move(val));
			// Any way to remove duplicate code between insert functions?

//			// Ok if equal because that would be adding to the end, but not ok if more
//			if(empty())
//			{
//				throw std::out_of_range("Empty list");
//			}

			if(insertIndex > mCurrentSize + 1)
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
			// Ok if equal because that would be adding to the end, but not ok if more
			if(empty())
			{
				throw std::out_of_range("Empty list");
			}

			if(insertIndex > mCurrentSize + 1)
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
			if( find(val) != mMaxSize)
			{
				erase(val);
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

	//     ~rule_of_five()
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

		void forwardMove(T&& other)
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

#endif /* INCLUDE_ARRAYARRAYLIST_HPP_ */
