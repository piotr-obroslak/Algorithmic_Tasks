#ifndef __BST_H__
#define __BST_H__

#include <exception>
#include <functional>
#include <memory>

template<typename KeyTy, typename MappedTy, typename KeyCmp = std::less<KeyTy>>
class BST
{
	class Node;

	public:
		class Exception
			: public std::exception
		{
			public:
				Exception(const char * what) : _wtf(what) {}
				const char * what() const noexcept override { return _wtf.c_str(); }

			private:
				std::string _wtf;
		};


		class Iterator
		{
			friend class BST;

			private:
				Iterator(const Node * p, std::unique_ptr<Node> & n)
					: _parent(p)
					, _node(&n)
				{
				}

				Iterator left() { return Iterator(_node->get(), (*_node)->_left); }
				Iterator right() { return Iterator(_node->get(), (*_node)->_right); }

				const Node * /*const*/ _parent;
				std::unique_ptr<Node> * _node;
		};


		BST()
			: _root(nullptr)
		{
		}

		
		Iterator find(const KeyTy & key)
		{
			try
			{
				//const auto & key = keyval.first;
				//const auto & val = keyval.second;

				const auto cmp = KeyCmp();

				Iterator it(nullptr, _root);
				while (auto p_node = it._node->get())
				{
					const auto & k = p_node->first;
					const auto less = cmp(k, key);
					const auto greeter = cmp(key, k);

					if (less & greeter)
					{
						throw Exception("invalid comparator!");
					}

					if (less)
					{
						it = it.right();
					}
					else if (greeter)
					{
						it = it.left();
					}
					else
					{
						return it;
					}
				}
			}
			catch(const std::exception & e)
			{
				throw Exception(e.what());
			}
		}


		void insert(const std::pair<KeyTy, MappedTy> & keyval)
		{
			try
			{
			}
			catch(const std::exception & e)
			{
				throw Exception(e.what());
			}
		}


	private:
		class Node
			: public std::pair<const KeyTy, MappedTy>
		{
			friend class BST::Iterator;

			private:
				Node(Node * const p, const std::pair<KeyTy, MappedTy> & keyval)
					: std::pair<KeyTy, MappedTy>(keyval)/*_parent(p)
					,*/
					, _height(0)
				{
				}

			private:
				size_t _height;

				std::unique_ptr<Node> _left;
				std::unique_ptr<Node> _right;
		};

		std::unique_ptr<Node> _root;
		//const std::unique_ptr<Node> _toor;
};

#endif

