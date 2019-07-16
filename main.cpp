#include <iostream>
#include <thread>

// parallel_linesはあるスレッドから別スレッドで実行される処理を記述するためのライブラリ
#include <parallel_lines.h>

// thunderclapは通知センターとなるライブラリ
// いわゆるObserverパターン（iOSで言うところのNSNotificationCenterのほうが近い）
#include <thunderclap.h>

int main()
{
	// schedulerを回しておく
	// schedulerに処理が追加されると逐次実行される
	std::thread parallel_lines_thread([=] {
		auto s = parallel_lines::scheduler::shared_scheduler();
		while (1) {
			s->update();
		}
	});
	// 完全に別スレッドでは知らせる
	parallel_lines_thread.detach();

	auto t = m2d::thunderclap<std::string>::shared_instance();
	// 通知に対する処理を記述する
	auto idx = t->watch("thread", [](std::string x) {
		// 通知を受け取ったら引数となるstringを表示する
		std::cout << std::this_thread::get_id() << ": Hello! World! " << x << std::endl;

		// schedulerに処理を追加してみる
		auto s = parallel_lines::scheduler::shared_scheduler();
		s->push([] {
			// schedulerが回っている別スレッドで実行される
			// これをメインスレッドにすれば、別スレッドからメインスレッドで実行する処理を記述できる
			// iOSで言うところのGrand Central Dispatchみたいな処理
			std::cout << "dispatch1 in thread " << std::this_thread::get_id() << std::endl;
		});
	});

	// メインスレッドのIDを表示
	std::cout << std::this_thread::get_id() << ": main thread " << std::endl;

	while (1) {
		std::string str;
		// 何らかの入力を受け取ってみる
		std::cout << "input your name -> ";
		std::cin >> str;

		// 別スレッドで通知を発行
		std::thread th([=] {
			auto t = m2d::thunderclap<std::string>::shared_instance();
			t->clap("thread", str);
		});
		th.detach();
	}
	return 0;
}