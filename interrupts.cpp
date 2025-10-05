/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 * @author Armeena Sajjad
 * @author Salma Khai 
 */

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
     int current_time = 0; 
     int context_time_ms = 10; 
     bool use_device_delay_in_isr = true;
     int fixed_isr_body_ms = 40;
     int isr_stage_ms = 40;
    /******************************************************************/

     // parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") {
            int cpu_ms = duration_intr;
            execution += std::to_string(current_time) + ", "
                       + std::to_string(cpu_ms) + ", CPU Burst\n";
            current_time += cpu_ms;
        }
        else if (activity == "SYSCALL") {
            int dev = duration_intr;

            auto [boil_text, t_after_entry] = intr_boilerplate(current_time, dev, context_time_ms, vectors);
            execution += boil_text;
            current_time = t_after_entry;

            int total_isr = use_device_delay_in_isr
                            ? ((dev >= 0 && dev < (int)delays.size()) ? delays.at(dev) : fixed_isr_body_ms)
                            : fixed_isr_body_ms;
            int part1 = std::min(isr_stage_ms, total_isr);
            int part2 = std::min(isr_stage_ms, std::max(0, total_isr - part1));
            int part3 = std::max(0, total_isr - part1 - part2);

            execution += std::to_string(current_time) + ", "
                       + std::to_string(part1) + ", SYSCALL: run the ISR (device driver)\n";
            current_time += part1;

            execution += std::to_string(current_time) + ", "
                       + std::to_string(part2) + ", transfer data from device to memory\n";
            current_time += part2;

            execution += std::to_string(current_time) + ", "
                       + std::to_string(part3) + ", check for errors\n";
            current_time += part3;

            execution += std::to_string(current_time) + ", "
                       + std::to_string(context_time_ms) + ", context restored\n";
            current_time += context_time_ms;

            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time += 1;

            execution += std::to_string(current_time) + ", 1, switch to user mode\n";
            current_time += 1;
        }
        else if (activity == "END_IO") {
            int dev = duration_intr;

            auto [boil_text, t_after_entry] = intr_boilerplate(current_time, dev, context_time_ms, vectors);
            execution += boil_text;
            current_time = t_after_entry;

            int total_isr = use_device_delay_in_isr
                            ? ((dev >= 0 && dev < (int)delays.size()) ? delays.at(dev) : fixed_isr_body_ms)
                            : fixed_isr_body_ms;
            int part1 = std::min(isr_stage_ms, total_isr);
            int part2 = std::max(0, total_isr - part1);

            execution += std::to_string(current_time) + ", "
                       + std::to_string(part1) + ", ENDIO: run the ISR (device driver)\n";
            current_time += part1;

            execution += std::to_string(current_time) + ", "
                       + std::to_string(part2) + ", check device status\n";
            current_time += part2;

            execution += std::to_string(current_time) + ", "
                       + std::to_string(context_time_ms) + ", context restored\n";
            current_time += context_time_ms;

            execution += std::to_string(current_time) + ", 1, IRET\n";
            current_time += 1;

            execution += std::to_string(current_time) + ", 1, switch to user mode\n";
            current_time += 1;
        }
        else {
            execution += std::to_string(current_time) + ", 0, UNKNOWN activity: " + activity + "\n";
        }
        /************************************************************************/
    }

    input_file.close();

    write_output(execution);

    return 0;
}
