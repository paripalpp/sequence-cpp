#pragma once
#include <chrono>
#include <string>
#include <iostream>
#include <sequence-cpp/sequence.hpp>

namespace sequence{
    class Delay : public Sequence {
        public:
            Delay(std::chrono::duration<double> delay) : delay(delay) {}
            void start() { start_time = std::chrono::steady_clock::now(); }
            void stop() {}
            void task() {}
            bool is_finished() { return std::chrono::steady_clock::now() - start_time > delay; }
        private:
            std::chrono::duration<double> delay;
            std::chrono::time_point<std::chrono::steady_clock> start_time;
    };

    class Log : public Sequence {
        public:
            Log(const std::string message) : message(message) {}
            void start() { std::cout << message << std::endl; }
            void stop() {}
            void task() {}
            bool is_finished() { return true; }
        private:
            const std::string message;
    };

    class WhileTime : public Sequence {
        public:
            WhileTime(std::chrono::duration<double> duration, Sequence::ptr seq) : duration(duration), seq(seq) {}
            void start() {
                seq->start();
                start_time = std::chrono::steady_clock::now();
                finished = false;
            }
            void stop() {
                seq->stop();
                finished = true;
            }
            void task() {
                if(!finished) {
                    seq->task();
                    if(seq->is_finished()) {
                        seq->start();
                    }else{
                        if(std::chrono::steady_clock::now() - start_time > duration) {
                            finished = true;
                        }else{
                            seq->start();
                        }
                    }
                }
            }
            bool is_finished() { return finished; }
        private:
            std::chrono::duration<double> duration;
            Sequence::ptr seq;
            std::chrono::time_point<std::chrono::steady_clock> start_time;
            bool finished = true;
    };
}
