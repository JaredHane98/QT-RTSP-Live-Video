#ifndef TIMER_HPP
#define TIMER_HPP
#include <chrono> 
#include <ctime>



template<typename Time_Base>
class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_last_time;
public:
    Timer()
        : m_last_time(std::chrono::high_resolution_clock::now())
    {}
    ~Timer()
    {}
    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    
    // returns the time in milliseconds between previous calls
    float getTimeElapsed()
    {
        auto now = std::chrono::high_resolution_clock::now();
        const float elapsed_time = std::chrono::duration<float, Time_Base>(now - m_last_time).count();
        m_last_time = now;
        return elapsed_time;
    }

    static int64_t getTime()
    {
        //std::chrono::duration<int64_t, Time_Base>
        return std::chrono::duration_cast<std::chrono::duration<int64_t, Time_Base>>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    }
};




#endif // TIMER_HPP