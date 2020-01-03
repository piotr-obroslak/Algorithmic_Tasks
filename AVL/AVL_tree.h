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
				
				long height() const { return ht; }
				const Node * parent() const { return up; }

			private:
				Node(Node * p, const value_type & keyval)
					: up(p), key(keyval.first), val(keyval.second), ht(0) {}

			private:
				const key_type key;
				mapped_type val;
				long ht;

				std::unique_ptr<Node> left;
				std::unique_ptr<Node> right;

				Node * up;
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

		std::pair<Iterator, bool> insert(const value_type & keyval)
		{
			try 
			{
				const auto & key = keyval.first;
				const auto & val = keyval.second;

				const auto comparator = key_compare();

				Node * up = nullptr;
				Iterator it = begin();
				while (it != end())
				{
					up = it.operator->();

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

				it.node->reset(new Node(up, keyval));
				auto the_new_one = (*it.node).get();
				long ht = the_new_one->ht;
				while (up != nullptr)
				{
					if (up->ht <= ht)
					{
						ht = ++up->ht;
					}
					up = up->up;
				}
#if 0
				{
					// TODO: height balancing
					auto parent = the_new_one->up;
					auto grand_parent = decltype(parent)(nullptr);

					if (parent != nullptr)
					{
						grand_parent = parent->up;
					}

					if (grand_parent != nullptr)
					{
						const auto get_height = [](const Node * n) {
							if (n != nullptr)
							{
								return n->ht;
							}

							return -1L;
						};

						const auto left_subtree_ht = get_height(grand_parent->left.get());
						const auto right_subtree_ht = get_height(grand_parent->right.get());
						std::cout << "left ht: " << left_subtree_ht << " right ht: " << right_subtree_ht << std::endl;

						if (abs(left_subtree_ht - right_subtree_ht) == 2)
						{
							if (grand_parent->left != nullptr && grand_parent->left->left.get() == the_new_one)
							{
								// TODO: LL
								auto grand_grand_parent = grand_parent->up;
								if (grand_grand_parent != nullptr)
								{
									
									//std::cout << "rotating" << std::endl;
									// i.e. grand_parent is not the root
									parent->up = grand_grand_parent;
									grand_grand_parent->left.release();
								   	grand_grand_parent->left.reset(parent);
									--grand_grand_parent->ht;

									grand_parent->up = parent;
									grand_parent->ht = 0;
									grand_parent->left.release();
									parent->right.release();
									parent->right.reset(grand_parent);
									//parent->left.release();
									//
									
								}
								else
								{
									//std::cout << "grand parent is the root, rotation" << std::endl;
									parent->up = nullptr;
									auto _r = root.release();
									root.reset(parent);

									grand_parent->left.release();
									grand_parent->up = parent;
									grand_parent->ht = 0;
									parent->right.reset(grand_parent);
								}
							}
						}
						{
							// TODO: LR
						}
						{
							// TODO: RL
						}
						{
							// TODO: RR
						}
					}

				}

#endif

				return std::make_pair(it, true);
			}
			catch (std::exception & e)
			{
				throw Exception(e.what());
			}
		}

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

