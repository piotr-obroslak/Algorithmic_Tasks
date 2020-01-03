#include <exception>
#include <functional>
#include <memory>


template<typename _Kty, typename _Ty, typename _Pr = std::less<_Kty>>
class AVLTree
{
	public:
		using key_type = _Kty;
		using mapped_type = _Ty;
		using value_type = std::pair<key_type, mapped_type>;
		using key_compare = _Pr;

	protected:
		class Node
		{
			friend class AVLTree;
			friend class AVLTree::Iterator;

			public:
				const key_type & first = key;
				mapped_type & second = val;

			private:
				Node(const value_type & keyval)
					: key(keyval.first), val(keyval.second)	{}

			private:
				const key_type key;
				mapped_type val;

				std::unique_ptr<Node> left;
				std::unique_ptr<Node> right;
		};

		std::unique_ptr<Node> root;
		/*const*/ std::unique_ptr<Node> toor;	// toor => root backwards, the invalid node for end() iterator

	public:
		class Exception
			: public std::exception
		{
			public:
				Exception(const char * what) : wtf(what) {}
				const char * what() const noexcept override { return wtf.c_str(); }

			private:
				std::string wtf;
		};

		class Iterator
		{
			friend class AVLTree;

			public:
				Node * operator->()
				{
					if (node != nullptr)
					{
						return node->get();
					}

					return tree->toor.get();
				}

				bool operator==(const Iterator & that) const { return !(this != that); }
				bool operator!=(const Iterator & that) const
				{
					if (this->tree == nullptr || that.tree == nullptr)
					{
						throw Exception("iterators invalid!");
					}
					else if (this->tree != that.tree)
					{
						throw Exception("iterators not comparable!");
					}

					return (this->node->get() != that.node->get());
				}

			private:
				Iterator(const AVLTree * t, std::unique_ptr<Node> & n)
					: tree(t), node(&n) {}

			private:
				std::unique_ptr<Node> * node;
				const AVLTree * tree;
		};

	public:
		AVLTree()
			: root(nullptr), toor(nullptr) {}

		std::pair<Iterator, bool> insert(const value_type & keyval);
		//Iterator find(const key_type & key);
		Iterator begin() { return Iterator(this, root); }
		Iterator end() { return Iterator(this, toor); }

		Iterator left(Iterator it) const { return Iterator(this, it->left); }
		Iterator right(Iterator it) const { return Iterator(this, it->right); }

		
		template<typename _Visitor>
		void traverse_in_order(_Visitor & consumer)
		{
			return traverse_in_order_recursively(begin(), consumer);
		}

	protected:
		template<typename _Visitor>
		void traverse_in_order_recursively(Iterator it, _Visitor & consumer)
		{
			if (it.node == nullptr)
			{
				return;
			}

			auto & n = *(it.node);

			if (n == nullptr)
			{
				return;
			}

			traverse_in_order_recursively(Iterator(this, n->left), consumer);
			consumer(Iterator(this, n));
			traverse_in_order_recursively(Iterator(this, n->right), consumer);
		}
};

template<typename _Kty, typename _Ty, typename _Pr>
std::pair<typename AVLTree<_Kty, _Ty, _Pr>::Iterator, bool> AVLTree<_Kty, _Ty, _Pr>::insert(const value_type & keyval)
{
	try 
	{
		const auto & key = keyval.first;
		const auto & val = keyval.second;

		const auto comparator = key_compare();

		Iterator it = begin();
		while (it != end())
		{
				const auto less = comparator(it->first, key);
				const auto greeter = comparator(key, it->first);

				if (less && greeter)
				{
					throw Exception("invalid comparator!");
				}

				if (less)
				{
					it = right(it);
				}
				else if (greeter)
				{
					it = left(it);
				}
				else
				{
					return std::make_pair(it, false);
				}
		}

		it.node->reset(new Node(keyval));
		return std::make_pair(it, true);
	}
	catch (std::exception & e)
	{
		throw Exception(e.what());
	}
}

template<typename _Kty, typename _Ty, typename _Pr>
std::ostream& operator<<(std::ostream & o, typename AVLTree<_Kty, _Ty, _Pr>::Iterator & it)
{
	// TOOD: the itretor must be valid
	const auto key = it->first;

	o << 'N';
	if (key < 0)
	{
		o << '_';
	}

	o << abs(key);
	
	return o;
}

