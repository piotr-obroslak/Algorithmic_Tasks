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
				//const Node * parent() const { return up; }

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

				balance(the_new_one);

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
		void rotate_left(Node * top)
		{
			auto upup = top->up;
			auto l = top->left.get();

			l->up = upup;

			if (upup == nullptr)
			{
				std::cout << "we're @ root" << std::endl;
				l->right.release();
				l->right.reset(top);
				top->up = l;

				root.release();
				root.reset(l);

				l->up = upup;

				top->left.release();

				std::cout << "it is not about rotation" << std::endl;
			}
			else
			{
				upup->left.release();	// TODO: wr're not sure if that's left or right
				upup->left.reset(l);
			}

			/*up->up = child;
			auto r = child->right.release();
			child->right.reset(up);

			r->up = up;
			up->left.release();
			up->left.reset(r);
			up->ht = std::max(get_height(up->left.get()), get_height(up->right.get())) + 1;
			child->ht = std::max(get_height(child->left.get()), get_height(child->right.get())) + 1;
			upup->ht = std::max(get_height(upup->left.get()), get_height(upup->right.get())) + 1;
*/
		}

		void rotate_right(Node * top)
		{
		}

		void balance(Node * leaf)
		{
			return;

			// NOTE: it is assumed this method is only called for newly added leaf nodes

			// TODO: height balancing - traverse the tree up (to the very root in extreme
			// case to see if any node on the traverse path got disbalanced after the 
			// insertion)

			const auto get_height =
				[](const Node * n) { return (n != nullptr) ? n->ht : -1L; };

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
					// TODO: balance the node

					auto myself = up;

					std::cout << "myself: " << "{" << myself->first << "," << myself->second << "}" << std::endl;
					std::cout << "left ht: " << l_ht << " right ht: " << r_ht << std::endl;

					if (l_ht > r_ht)
					{

						std::cout << "left child:" << "{" << l->first << "," << l->second << "}" << std::endl;
						
						auto ll = l->left.get();
						auto lr = l->right.get();
						const auto ll_ht = get_height(ll);
						const auto lr_ht = get_height(lr);

						std::cout << "left left ht: " << ll_ht << " left right ht: " << lr_ht << std::endl;

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
						const auto rl_ht = get_height(up->right->left.get());
						const auto rr_ht = get_height(up->right->right.get());

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
					// TODO: I'll uncomment once I'm sure the tree is balanced
					//throw Exception("something overseen!");
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

