#include <fstream>
#include <iostream>
#include <numeric>

#include <boost/filesystem.hpp>

#include <lo2s/config.hpp>
#include <lo2s/summary.hpp>
#include <lo2s/trace/trace.hpp>
#include <lo2s/util.hpp>
namespace lo2s
{
std::string Summary::trace_dir_ = "";
double Summary::start_cpu_time_ = 0;
double Summary::start_wall_time_ = 0;
long Summary::thread_count_ = 0;
long Summary::num_wakeups_ = 0;
Summary::Summary()
{
}
void Summary::record_wakeups(int num_wakeups)
{
    num_wakeups_ += num_wakeups;
}
void Summary::start()
{
    std::pair<double, double> times = get_process_times();
    start_wall_time_ = times.first;
    start_cpu_time_ = times.second;
}
void Summary::set_trace_dir(std::string trace_dir)
{
    trace_dir_ = trace_dir;
}
void Summary::finalize_and_print()
{
    int trace_size;

    std::pair<double, double> times = get_process_times();
    double wall_time = times.first - start_wall_time_;
    double cpu_time = times.second - start_cpu_time_;

    boost::filesystem::recursive_directory_iterator it(trace_dir_), end;
    trace_size = std::accumulate(
        it, end, 0, [](long unsigned int sum, boost::filesystem::directory_entry& entry) {
            if (!boost::filesystem::is_directory(entry))
            {
                return sum + boost::filesystem::file_size(entry);
            }
            return sum;
        });
    std::cout << "EXECUTION SUMMARY\n";
    std::cout << " * executed command: ";
    for (auto i = config().command.begin(); i != config().command.end(); ++i)
    {
        std::cout << *i << ' ';
    }
    std::cout << "\n";
    std::cout << " * " << wall_time << "s Wall Time\n";
    std::cout << " * " << cpu_time << "s CPU time\n";
    std::cout << " * spawned threads: " << thread_count_ << '\n';
    std::cout << " * wakeups: " << num_wakeups_ << '\n';
    if (trace_dir_ != "" && trace_size != -1)
    {
        std::cout << " * traces have been written to: " << trace_dir_;
        std::cout << "(" << trace_size << "B)\n";
    }
}
void Summary::increase_thread_count()
{
    thread_count_++;
}
}
