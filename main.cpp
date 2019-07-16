#include <iostream>
#include <thread>

#include <parallel_lines.h>
#include <thunderclap.h>

int main()
{
	std::thread parallel_lines_thread([=] {
		auto s = parallel_lines::scheduler::shared_scheduler();
		while (1) {
			s->update();
		}
	});
	parallel_lines_thread.detach();

	auto t = m2d::thunderclap<std::string>::shared_instance();
	auto idx = t->watch("thread", [](std::string x) {
		std::cout << std::this_thread::get_id() << ": Hello! World! " << x << std::endl;
		auto s = parallel_lines::scheduler::shared_scheduler();
		s->push([] {
			std::cout << "dispatch1 in thread " << std::this_thread::get_id() << std::endl;
		});
	});
	std::cout << std::this_thread::get_id() << ": main thread " << std::endl;
	while (1) {
		std::string str;
		std::cout << "input your name -> ";
		std::cin >> str;
		std::thread th([=] {
			auto t = m2d::thunderclap<std::string>::shared_instance();
			t->clap("thread", str);
		});
		th.detach();
	}
	return 0;
}