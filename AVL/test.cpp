#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "AVL_tree.h"

using Map = AVLTree<int, std::string>;

static std::ostream& operator<<(std::ostream & o, Map::Iterator & it)
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
	class NodeDumper
	{
		public:
			void operator()(Map::Iterator it)
			{
				std::cout << '\t' << '{' <<it->first << ',' << it->second << '}' << '\n';
			}
	};

	class DotWriter
	{
		public:
			DotWriter(Map & tr, const char * fname = "demo.dot")
				: t(tr)
				, dot(fname)
			{
				dot << "digraph testAVL {\n";
			}

			~DotWriter()
			{
				dot << "}\n";
			}

			void operator()(Map::Iterator it)
			{
				//std::cout << "consuming" << it << std::endl;
				if (it != t.end())
				{
					dot << '\t' << it << '[' <<  "label=" << '\"' << it->first << "\\n" << it->second ;
					dot << "\\n" << it->height();
				   	dot << '\"' << ']' << '\n';
					add_edge(it, t.left(it));
					add_edge(it, t.right(it));
				}
			}

		private:
			void add_edge (Map::Iterator source, Map::Iterator target)
			{
				if (source != t.end() && target != t.end())
				{
					dot << '\t' << source << "->" << target << '\n';
				}
			};

			Map & t;
			std::ofstream dot;
	};
	
	try
	{
		srand(time(0));

		std::vector<Map::value_type> values;



		if (argc < 3)
		{
			const auto max_size = 100000;
			const auto size = (argc == 1) ? rand() % max_size : atoi(argv[1]);

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
				values.push_back({key, val});
			}
		}
		else
		{
			for (auto i = 0; i<argc/2; i++)
			{
				const auto key = atoi(argv[2*i+1]);
				const auto val = std::string(argv[2*i+2]);
				values.push_back({key, val});
			}

		}
		
		std::cout << "creating a random AVL tree with " << values.size() << " nodes..." << std::endl;
		Map t;
		for (const auto & value : values)
		{
			t.insert(value);
		}
		std::cout << "done!" << std::endl;
		
		std::cout << "the tree contents:\n";
		NodeDumper d;
		t.traverse_in_order(d);
		std::cout << std::endl;


		std::cout << "generating *.dot file for graphviz" << std::endl;
		DotWriter w(t, "demo.dot");
		t.traverse_in_order(w);
		std::cout << "done!" << std::endl;

		{
			std::cout << "\nrunning some test..." << std::endl;

			auto data_presence_test = [](const decltype(values) & data)
			{
				Map m;
				for (const auto & val : data)
				{
					std::cout << "\t\t inserting @ " << val.first << "...";
					m.insert(val);
					std::cout << "   done." << std::endl;
				}
				std::cout << "\tdone!" << std::endl;

				std::cout << "\t2. making sure all values are there..." << std::endl;
				for (const auto & val : data)
				{
					const auto & key = val.first;
					if (m.find(key) == m.end())
					{
						std::cout << "\t\tERROR: key " << key << " not found!" << std::endl;
					}
				}
				std::cout << "\tdone!" << std::endl;
			};

			{
				std::cout << "\t1. inserting the values in ascending order... " << std::endl;
				{
					decltype(values) vals(values);
					std::sort(vals.begin(), vals.end(),
						[](const Map::value_type & a, const Map::value_type & b){ return a.first < b.first;});

					data_presence_test(vals);
				}

				std::cout << "\t1. inserting the values in descending order... " << std::endl;
				{
					decltype(values) vals(values);
					std::sort(vals.begin(), vals.end(),
						[](const Map::value_type & a, const Map::value_type & b){ return b.first < a.first; });

					data_presence_test(vals);
				}

				std::cout << "\t1. inserting the values in random order... " << std::endl;
				{
					decltype(values) vals(values);
					data_presence_test(vals);
				}
			}

			std::cout << "done!" << std::endl;
		}

		//t.traverse_in_order(dumper);
		std::cout << std::endl;
	}
	catch (const Map::Exception & e)
	{
		std::cout << "FAILURE: " << e.what() << std::endl;
	}

	return 0;
}

