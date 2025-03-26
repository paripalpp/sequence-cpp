#pragma once
#include <vector>
#include <string>
#include <initializer_list>
#include <cstdint>
#include <functional>
#include <memory>

namespace sequence{
    class Sequence {
        public:
            Sequence(const std::string& name = "") : name(name) {}
            virtual void start() = 0;
            virtual void stop() = 0;
            virtual void task() = 0;
            virtual bool is_finished() = 0;

            void set_name(const std::string& name) { this->name = name; }
            std::string get_name() const { return name; }

            using ptr = std::shared_ptr<Sequence>;
            using list = std::vector<Sequence::ptr>;
        protected:
            std::string name;
    };

    class SequenceSeries : public Sequence {
        public:
            SequenceSeries(std::vector<Sequence::ptr> seqs);
            SequenceSeries(std::initializer_list<Sequence::ptr> seqs);
            void start();
            void stop();
            void task();
            bool is_finished();
        private:
            void start_next();
            std::vector<Sequence::ptr> seqs;
            std::vector<Sequence::ptr>::iterator current;
    };

    class SequenceParallel : public Sequence {
        public:
            SequenceParallel(std::vector<Sequence::ptr> seqs);
            SequenceParallel(std::initializer_list<Sequence::ptr> seqs);
            void start();
            void stop();
            void task();
            bool is_finished();
        private:
            std::vector<Sequence::ptr> seqs;
            std::size_t finished_count = 0;
    };

    class Loop : public Sequence {
        public:
            Loop(Sequence::ptr seq);
            void start();
            void stop();
            void task();
            bool is_finished();
        private:
            Sequence::ptr seq;
            bool finished = true;
    };

    class SingleFunction : public Sequence {
        public:
            SingleFunction(std::function<void()> func) : func(func) {}
            void start() { func(); }
            void stop() {}
            void task() {}
            bool is_finished() { return true; }
        private:
            std::function<void()> func;
    };

    class Condition : public Sequence {
        public:
            Condition(std::function<bool()> condition) : condition(condition) {}
            void start() {}
            void stop() {}
            void task() {}
            bool is_finished() { return condition(); }
        private:
            std::function<bool()> condition;
    };

    class If : public Sequence {
        public:
            If(std::function<bool()> condition, Sequence::ptr seq) : condition(condition), seq(seq) {}
            void start() {
                if(condition()) {
                    seq->start();
                    skipped = false;
                } else skipped = true;
            }
            void stop() { seq->stop(); }
            void task() { seq->task(); }
            bool is_finished() { if(skipped) return true; else return seq->is_finished(); }
        private:
            std::function<bool()> condition;
            Sequence::ptr seq;
            bool skipped = true;
    };

    class While : public Sequence {
        public:
            While(std::function<bool()> condition, Sequence::ptr seq);
            void start();
            void stop();
            void task();
            bool is_finished();
        private:
            std::function<bool()> condition;
            Sequence::ptr seq;
            bool finished = true;
    };
}
