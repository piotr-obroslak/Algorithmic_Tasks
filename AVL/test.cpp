#include <fstream>
#include <iostream>
#include "AVL_tree.h"

using TestTree = AVLTree<int, std::string>;

static std::ostream& operator<<(std::ostream & o, TestTree::Iterator & it)
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

int main(int argc, char * argv[])
{
	TestTree t;

	srand(time(0));

	if (argc < 3)
	{
		const auto max_size = 100000;
		const auto size = (argc == 1) ? rand() % max_size : atoi(argv[1]);

		std::cout << "creating a random AVL tree with " << size << " nodes..." << std::endl;

		for (auto i = 0; i<size; i++)
		{
			const auto key = rand() % (4 * size) - (2 * size);
			std::string val;
			{
				const auto val_len = rand() % 5 + 1;
				for (auto c = 0; c < val_len; c++)
				{
					val.push_back(rand() % ('z'-'a') + 'a');
				}
			}
			t.insert({key, val});
		}

		std::cout << "done!" << std::endl;
	}
	else
	{
		std::cout << "creating an AVL tree from the " << (argc - 1) / 2 << " specified nodes...\n";

		for (auto i = 0; i<argc/2; i++)
		{
			const auto key = atoi(argv[2*i+1]);
			const auto val = std::string(argv[2*i+2]);
			t.insert({key, val});
		}

		std::cout << "done!" << std::endl;
	}

	class DotWritter
	{
		public:
			DotWritter(TestTree & tr)
				: t(tr)
				, dot("test.dot")
			{
				dot << "digraph testAVL {\n";
			}

			~DotWritter()
			{
				dot << "}\n";
			}

			void operator()(TestTree::Iterator it)
			{
				//std::cout << "consuming" << it << std::endl;
				if (it != t.end())
				{
					dot << '\t' << it << '[' <<  "label=" << '\"' << it->first << "\\n" << it->second ;
					//auto parent = it->parent();
					//if (parent != nullptr)
					//{
						dot << "\\n" << it->height();
					//}
				   	dot << '\"' << ']' << '\n';
					add_edge(it, t.left(it));
					add_edge(it, t.right(it));
				}
			}

		private:
			void add_edge (TestTree::Iterator source, TestTree::Iterator target)
			{
				if (source != t.end() && target != t.end())
				{
					dot << '\t' << source << "->" << target << '\n';
				}
			};

			TestTree & t;
			std::ofstream dot;
	} writer(t);
	
	class NodeDumper
	{
		public:
			void operator()(TestTree::Iterator it)
			{
				std::cout << '{' <<it->first << ',' << it->second << '}' << ", ";
			}
	} dumper;

	t.traverse_in_order(dumper);
	t.traverse_in_order(writer);
	std::cout << std::endl;

	return 0;
}

