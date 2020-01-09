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

		class Iterator;

	protected:
		class Node
		{
			friend class AVLTree;
			friend class AVLTree::Iterator;

			public:
				const key_type & first = key;
				mapped_type & second = val;
				
				long height() const { return ht; }

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

				bool operator==(const Iterator & that) const { return !(*this != that); }
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

				balance(the_new_one);

				return std::make_pair(it, true);
			}
			catch (std::exception & e)
			{
				throw Exception(e.what());
			}
		}

		Iterator find(const key_type & key)
		{
			// TODO: identical code snippet appears in insert

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
					return it;
				}
			}

			return end();
		}

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
		long get_height (const Node * top)
		{
			return (top != nullptr) ? top->ht : -1L;
		};

		void rotate_left(Node * top)
		{
			//std::cout << "performing left rotation" << std::endl;

			auto topup = top->up;
			auto r = top->right.get();

			if (topup  == nullptr)
			{
				root.release();
				root.reset(r);
			}
			else
			{
				if (topup->right.get() == top)
				{
					topup->right.release();
					topup->right.reset(r);
				}
				else if (topup->left.get() == top)
				{
					topup->left.release();
					topup->left.reset(r);
				}
			}
			r->up = topup;

			auto orphaned = r->left.release();
			r->left.reset(top);
			top->up = r;

			/*auto adoptor =*/ top->right.release();
			top->right.reset(orphaned);
			if (orphaned != nullptr)
			{
				orphaned->up = top;
			}

			top->ht = std::max(get_height(top->left.get()), get_height(top->right.get())) + 1;
			r->ht = std::max(get_height(r->left.get()), get_height(r->right.get())) + 1;
			while (topup != nullptr)
			{
				topup->ht = std::max(get_height(topup->left.get()), get_height(topup->right.get())) + 1;
				topup = topup->up;
			}
		}

		void rotate_right(Node * top)
		{
			//std::cout << "performing right rotation" << std::endl;

			auto topup = top->up;
			auto l = top->left.get();

			if (topup == nullptr)
			{
				root.release();
				root.reset(l);
			}
			else
			{
				if (topup->left.get() == top)
				{
					topup->left.release();
					topup->left.reset(l);
				}
				else if (topup->right.get() == top)
				{
					topup->right.release();
					topup->right.reset(l);
				}
			}
			l->up = topup;

			auto orphaned = l->right.release();
			l->right.reset(top);
			top->up = l;

			/*auto adoptor =*/ top->left.release();
			top->left.reset(orphaned);
			if (orphaned != nullptr)
			{
				orphaned->up = top;
			}

			top->ht = std::max(get_height(top->left.get()), get_height(top->right.get())) + 1;
			l->ht = std::max(get_height(l->left.get()), get_height(l->right.get())) + 1;
			while (topup != nullptr)
			{
				topup->ht = std::max(get_height(topup->left.get()), get_height(topup->right.get())) + 1;
				topup = topup->up;
			}
		}

		void balance(Node * leaf)
		{
			// NOTE: it is assumed this method is only called for newly added leaf nodes

			auto up = leaf->up;
			while (up != nullptr)
			{
				auto l = up->left.get();
				auto r = up->right.get();
				const auto l_ht = get_height(l);
				const auto r_ht = get_height(r);
				const auto d_ht = abs(l_ht -  r_ht);

				if (d_ht < 2)
				{
				}
				else if (d_ht == 2)
				{
					auto myself = up;

					if (l_ht > r_ht)
					{
						auto ll = l->left.get();
						auto lr = l->right.get();
						const auto ll_ht = get_height(ll);
						const auto lr_ht = get_height(lr);

						if (ll_ht > lr_ht)
						{
							rotate_right(myself);
						}
						else if (lr_ht > ll_ht)
						{
							rotate_left(l);
							rotate_right(myself);
						}
					}
					else if (r_ht > l_ht)
					{
						auto rr = r->right.get();
						auto rl = r->left.get();
						const auto rl_ht = get_height(rl);
						const auto rr_ht = get_height(rr);

						if (rl_ht > rr_ht)
						{
							rotate_right(r);
							rotate_left(myself);
						}
						else if (rr_ht > rl_ht)
						{
							rotate_left(myself);
						}
					}
					
					break;
				}
				else
				{
					throw Exception("something overseen!");
				}

				up = up->up;
			}

		}

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

