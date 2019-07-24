#include <mutex>
#include <thread>

class TimeCount {
public:
	TimeCount():m_time(0)/*,m_multipe(0) */{}
	void thread_work_fun() {
		while (1) {
			std::lock_guard<std::mutex>lg(m_Mutex);
			m_time += 1;
		}
		
	}
	uint64_t getTime() {
		std::lock_guard<std::mutex> lg(m_Mutex);
		uint64_t time = m_time;
		m_time = 0;
		return time;
	}

	void start() {
		std::thread td(&TimeCount::thread_work_fun,this);
		td.detach();
	}


private:
	std::mutex m_Mutex;
	uint64_t m_time;
	//uint32_t m_multipe;
};