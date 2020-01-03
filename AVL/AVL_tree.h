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

			Node(const value_type & val);

			const key_type key;
			mapped_type val;

			std::unique_ptr<Node> left;
			std::unique_ptr<Node> right;

			public:
				const key_type & first = key;
				mapped_type & second = val;
		};

		std::unique_ptr<Node> root;

	public:
		class Iterator
		{
			friend class AVLTree;

			Iterator(Node * n);

			Node * node;

			public:
			//	value_type & keyval();
				Node * operator->() { return node; }
				bool operator==(const Iterator & that) const { return node == that.node; }
				bool operator!=(const Iterator & that) const { return !(*this == that); }
		};

	public:
		AVLTree();

		std::pair<Iterator, bool> insert(const value_type & keyval);
		//Iterator find(const key_type & key);
		Iterator begin() const;
		Iterator end() const;

		Iterator left(Iterator it) const;
		Iterator right(Iterator it) const;
		
		template<typename _Visitor>
		void traverse_in_order(_Visitor & consumer);

	protected:
		template<typename _Visitor>
		void traverse_in_order_recursively(Iterator it, _Visitor & consumer);
};

template<typename _Kty, typename _Ty, typename _Pr>
AVLTree<_Kty, _Ty, _Pr>::Node::Node(const value_type & keyval)
	: key(keyval.first)
	, val(keyval.second)
{
}


template<typename _Kty, typename _Ty, typename _Pr>
AVLTree<_Kty, _Ty, _Pr>::Iterator::Iterator(Node * n)
	: node(n)
{
}


template<typename _Kty, typename _Ty, typename _Pr>
AVLTree<_Kty, _Ty, _Pr>::AVLTree()
{
}

template<typename _Kty, typename _Ty, typename _Pr>
std::pair<typename AVLTree<_Kty, _Ty, _Pr>::Iterator, bool> AVLTree<_Kty, _Ty, _Pr>::insert(const value_type & keyval)
{
	const auto dump_info = false;

	const auto & key = keyval.first;
	const auto & val = keyval.second;

	if (dump_info)
	{
		std::cout << "attempt to insert " << '{' << key << ',' << val << '}' << std::endl;
	}

	if (root == nullptr)
	{
		root.reset(new Node(keyval));
		return std::make_pair(begin(), true);
	}
	else
	{
		const auto comparator = key_compare();

		Node * p = nullptr;
		Node * n = root.get();
		while (n != nullptr)
		{
			if (comparator(n->key, key))
			{
				p = n;
				n = n->right.get();
			}
			else if (comparator(key, n->key))
			{
				p = n;
				n = n->left.get();
			}
			else
			{
				if (dump_info)
				{
					std::cout << "already there " << '{' << key << ',' << val << '}' << std::endl;
				}

				return std::make_pair(Iterator(n), false);
			}
		}


		if (p != nullptr)
		{
			if (dump_info)
			{
				std::cout << '{' << key << ',' << val << '}' << "'s parent would be " <<
					'{' << p->key << ',' << p->val << '}' << std::endl;
			}

			if (comparator(p->key, key))
			{
				p->right.reset(new Node(keyval));
			}
			else if (comparator(key, p->key))
			{
				p->left.reset(new Node(keyval));
			}

			if (dump_info)
			{

				std::cout << "inserted " << '{' << key << ',' << val << '}' << std::endl;
			}

			return std::make_pair(Iterator(p), true);
		}
	}

	return std::make_pair(end(), false);	// TODO: that's a failed-to-create-a-node exception
}

template<typename _Kty, typename _Ty, typename _Pr>
typename AVLTree<_Kty, _Ty, _Pr>::Iterator AVLTree<_Kty, _Ty, _Pr>::end() const
{
	return Iterator(nullptr);
}

template<typename _Kty, typename _Ty, typename _Pr>
typename AVLTree<_Kty, _Ty, _Pr>::Iterator AVLTree<_Kty, _Ty, _Pr>::begin() const
{
	return Iterator(root.get());
}

template<typename _Kty, typename _Ty, typename _Pr>
template<typename _Visitor>
void AVLTree<_Kty, _Ty, _Pr>::traverse_in_order(_Visitor & consumer)
{
	return traverse_in_order_recursively(begin(), consumer);
}

template<typename _Kty, typename _Ty, typename _Pr>
typename AVLTree<_Kty, _Ty, _Pr>::Iterator AVLTree<_Kty, _Ty, _Pr>::left(Iterator it) const
{
	return Iterator(it->left.get());
}

template<typename _Kty, typename _Ty, typename _Pr>
typename AVLTree<_Kty, _Ty, _Pr>::Iterator AVLTree<_Kty, _Ty, _Pr>::right(Iterator it) const
{
	return Iterator(it->right.get());
}

template<typename _Kty, typename _Ty, typename _Pr>
template<typename _Visitor>
void AVLTree<_Kty, _Ty, _Pr>::traverse_in_order_recursively(Iterator it, _Visitor & consumer)
{
	if (it.node == nullptr)
	{
		return;
	}

	auto & n = it.node;

	traverse_in_order_recursively(Iterator(n->left.get()), consumer);
	consumer(Iterator(n));
	traverse_in_order_recursively(Iterator(n->right.get()), consumer);
}
