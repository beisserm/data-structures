#ifndef INCLUDE_LINKEDLIST_HPP_
#define INCLUDE_LINKEDLIST_HPP_

#include <memory>

template<typename T>
class LinkedList
{
	public:
//		// Default constructor
//		LinkedList() = default;
//
//		// Copy constructor, could just use default
//		LinkedList(const LinkedList<T>& other)
//		{
//			mNode = other.mNode;
//			mSize = other.mSize;
//		}
//
//		// Move constructor should never throw
//		LinkedList(LinkedList<T>&& other) noexcept
//		{
//			forwardMove(std::forward(other));
//		}
//
//		/**
//		 * Copy assignment
//		 */
//		LinkedList<T>& operator=(LinkedList<T> other)
//		{
//			swap(*this, other);
//			return *this;
//		}
//
//		/**
//		 * Move assignment should never throw
//		 */
//		LinkedList<T>& operator=(LinkedList<T>&& other) noexcept
//		{
//			// Don't want to swap. Temporary variable is going away and assigning something to it would be strange
//			// behavior.
//			// Also we forward other because && doesn't always mean rvalue reference, it could be a
//			// forwarding reference (universal reference).
//			forwardMove(std::forward(other));
//			return *this;
//		}

		virtual ~LinkedList() = default;

		// Capacity:
		size_t size() const noexcept
		{
			return mCurrentSize;
		}

		size_t max_size() const noexcept
		{
			return std::numeric_limits<size_t>::max();
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
			if(index > mCurrentSize)
			{
				throw std::out_of_range("Index out of bounds");
			}

			Node<T>* it = mNode.get();
			for(std::size_t i = 0; i < index; ++i)
			{
				it = it->next.get();
			}

			return it->data;
		}

		T& at(size_t index) // throw out_of_range
		{
			return const_cast<T&>(static_cast<const ArrayList<T>*>(this)->at(index));
		}

		const T& at(size_t index) const // throw out_of_range
		{
			if(index > mCurrentSize)
			{
				throw std::out_of_range("Index out of bounds");
			}

			Node<T>* it = mNode.get();
			for(std::size_t i = 0; i < index; ++i)
			{
				it = it->next.get();
			}

			return it->data;
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

			return mNode->data;
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

			Node<T>* it = mNode.get();
			for(std::size_t i = 0; i < mCurrentSize; ++i)
			{
				it = it->next();
			}

			return it->data();
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

		void insert(const T& val, std::size_t insertIndex)
		{
			if(insertIndex > mCurrentSize + 1)
			{
				throw std::out_of_range("Index out of bounds");
			}

			if(insertIndex == 0)
			{
				std::unique_ptr<Node> head = std::make_unique<Node>(val);
				head.next = std::move(mNode);
				mNode = head;
			}
			else
			{
				// This seems dirty
				std::unique_ptr<Node>* it = &mNode;
				std::unique_ptr<Node>* prev = nullptr;

				for(std::size_t i = 0; i != insertIndex; ++i)
				{
					if(i != 0)
					{
						prev = it;
					}

					it = &(it->next);
				}

				std::unique_ptr<Node> newNode = std::make_unique<Node>(val);
				newNode.next = std::move((*it));
				prev->next = std::move(newNode);


//				Node<T>* it = mNode.get();
//				Node<T>* prev = nullptr;
//				for(std::size_t i = 0; i != insertIndex; ++i)
//				{
//					if(i != 0)
//					{
//						prev = it;
//					}
//
//					it = it->next.get();
//				}
//
//				std::unique_ptr<Node> newNode = std::make_unique<Node>(val);
//				it
//				newNode
//
//				it->data() = val;
//				it->next() = nullptr;
			}

			mCurrentSize++;
		}

		void insert(T&& val, std::size_t insertIndex)
		{
			if(insertIndex > mCurrentSize + 1)
			{
				throw std::out_of_range("Index out of bounds");
			}

			if(insertIndex == 0)
			{
				std::unique_ptr<Node> head = std::make_unique<Node>(std::move(val));
				head.next = std::move(mNode);
				mNode = head;
			}
			else
			{
				// This seems dirty
				std::unique_ptr<Node>* it = &mNode;
				std::unique_ptr<Node>* prev = nullptr;

				for(std::size_t i = 0; i != insertIndex; ++i)
				{
					if(i != 0)
					{
						prev = it;
					}

					it = &(it->next);
				}

				std::unique_ptr<Node> newNode = std::make_unique<Node>(std::move(val));
				newNode.next = std::move((*it));
				prev->next = std::move(newNode);
			}

			mCurrentSize++;
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

			// This seems dirty
			std::unique_ptr<Node>* it = &mNode;

			for(std::size_t i = 0; i != insertIndex; ++i)
			{
				it = &(it->next);
			}


//			if(insertIndex > mCurrentSize)
//			{
//				throw std::out_of_range("Index out of bounds");
//			}
//
//			Node<T>* it = mNode.get();
//			for(std::size_t i = 0; i != insertIndex; ++i)
//			{
//				it = it->next();
//			}
//
//
//
//			mContents[insertIndex] = std::move(val);
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

			// This seems dirty
			std::unique_ptr<Node>* it = &mNode;

			for(std::size_t i = 0; i < index; ++i)
			{
				it = &(it->next);
			}

			T removed = (*it)->next->data;

			// it is prev. it->next is one we want to delete.
			(*it)->next = (*it)->next->next;

			return removed;
		}

		void remove(const T& val)
		{
			if( find(val) != mCurrentSize)
			{
				erase(val);
			}
		}

		// Return index of element or total size if not found
		size_t find(const T& val) const
		{
			int index = mCurrentSize;

			Node<T>* it = mNode.get();
			for(std::size_t i = 0; i < mCurrentSize; ++i)
			{
				if(it->data == val)
				{
					index = i;
					break;
				}

				it = it->next.get();
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

		/**
		 * Swap function should never throw
		 */
		friend void swap(LinkedList<T>& left, LinkedList<T>& right) noexcept
		{
			// We always just want to call swap and be done with it. We don't want swap to be a member function. So we
			// enable ADL (argument dependent lookup) and when we call swap it will find our friend function because
			// it's a better match
			using std::swap;

			std::swap(left.mNode, right.mNode);
			std::swap(left.mCurrentSize, right.mCurrentSize);
		}

	private:
		template<typename T>
		class Node
		{
			public:
				// Copy constructor
				explicit Node(const Node<T>& data)
				{
					this->data = data;
					this->next = nullptr;
				}

//				Node(const T& data, std::shared_ptr<T> next)
//				{
//					mData = data;
//					mNext = next;
//				}

//				T& data()
//				{
//					return mData;
//				}
//
//				Node* next() const
//				{
//					return mNext->get();
//				}
//
//				const Node* next() const
//				{
//					return mNext->get();
//				}

				virtual ~Node() = default;

//			private:
				T data;
				std::unique_ptr<Node> next = nullptr;
		};

//		void forwardMove(T&& other)
//		{
//			mNode = std::move(other.mNode);
//			mCurrentSize = std::move(other.mCurrentSize);
//		}

		std::unique_ptr<Node<T>> mNode;
		size_t mCurrentSize = 0;

};


#endif /* INCLUDE_LINKEDLIST_HPP_ */
