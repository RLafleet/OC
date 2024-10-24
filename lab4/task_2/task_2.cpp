#include <iostream>
#include <thread>
#include <chrono>
#include <stop_token>
#include <syncstream>
#include <sstream>
#include <iomanip>
#include <string>

std::string CurrentTimeToString() 
{
    using Clock = std::chrono::system_clock;
    auto t = Clock::to_time_t(Clock::now());
    struct tm buf;
    localtime_s(&buf, &t);
    std::ostringstream os;
    os << std::setw(2) << std::setfill('0') << buf.tm_hour << ":"
        << std::setw(2) << std::setfill('0') << buf.tm_min << ":"
        << std::setw(2) << std::setfill('0') << buf.tm_sec;
    return os.str();
}

void Tic(std::stop_token st)  
{
    while (!st.stop_requested()) 
    {
        {
            std::osyncstream sync_out(std::cout);
            sync_out << CurrentTimeToString() << " Tic" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    std::osyncstream(std::cout) << "Tic thread completed" << std::endl;
}

void Tac(std::stop_token st) 
{
    while (!st.stop_requested()) 
    {
        {
            std::osyncstream sync_out(std::cout);
            sync_out << CurrentTimeToString() << " Tac" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    std::osyncstream(std::cout) << "Tac thread completed" << std::endl;
}

int main() {
    std::jthread tic_thread(Tic);
    std::jthread tac_thread(Tac);

    std::string input;
    std::getline(std::cin, input);
    
    return 0;
}
