#ifndef __AVL_H__
#define __AVL_H__

#include "../BST.h"

template<typename _Kty, typename _Vty, typename _Pr = std::less<_Kty>>
class AVL
	: public BST<_Kty, _Vty, _Pr>
{
	using base_type = BST<_Kty, _Vty, _Pr>;

	public:
		//using value_type = typename BST<_Kty, _Vty, _Pr>::value_type;
		using Exception = typename base_type::Exception;
		//using Iterator = typename BST<_Kty, _Vty, _Pr>::Iterator;*/

	protected:
		using Node = typename base_type::Node;
		void balance(Node * leaf) override
		{
			// NOTE: it is assumed this method is only called for newly added leaf nodes

			auto up = leaf->parent();
			while (up != nullptr)
			{
				auto l = up->l();
				auto r = up->r();
				const auto l_ht = base_type::get_height(l);
				const auto r_ht = base_type::get_height(r);
				const auto d_ht = abs(l_ht -  r_ht);

				if (d_ht < 2)
				{
				}
				else if (d_ht == 2)
				{
					auto myself = up;

					if (l_ht > r_ht)
					{
						auto ll = l->l();
						auto lr = l->r();
						const auto ll_ht = base_type::get_height(ll);
						const auto lr_ht = base_type::get_height(lr);

						if (ll_ht > lr_ht)
						{
							base_type::rotate_right(const_cast<Node*>(myself));
						}
						else if (lr_ht > ll_ht)
						{
							base_type::rotate_left(const_cast<Node*>(l));
							base_type::rotate_right(const_cast<Node*>(myself));
						}
					}
					else if (r_ht > l_ht)
					{
						auto rr = r->r();
						auto rl = r->l();
						const auto rl_ht = base_type::get_height(const_cast<Node*>(rl));
						const auto rr_ht = base_type::get_height(const_cast<Node*>(rr));

						if (rl_ht > rr_ht)
						{
							base_type::rotate_right(const_cast<Node*>(r));
							base_type::rotate_left(const_cast<Node*>(myself));
						}
						else if (rr_ht > rl_ht)
						{
							base_type::rotate_left(const_cast<Node*>(myself));
						}
					}
					
					break;
				}
				else
				{
					throw Exception("something overseen!");
				}

				up = up->parent();
			}
		}
};

#endif

