#include <boost/dynamic_bitset.hpp>
#include <ctime>
namespace lo2s
{
class Summary
{
public:
    static void start();
    static void finalize_and_print();
    static void record_wakeups(int num_wakeups);
    static void increase_thread_count();
    static void set_trace_dir(std::string trace_dir);
    static void set_pid(pid_t pid);
    static void set_exit_code(int exit_code);

private:
    Summary();
    static int exit_code_;
    static std::string trace_dir_;
    static double start_cpu_time_;
    static double start_wall_time_;
    static long num_wakeups_;
    static long thread_count_;
    static boost::dynamic_bitset<> processes;
};
}
