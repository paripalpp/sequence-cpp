#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <sequence-cpp/sequence_posix.hpp>
#include <sequence-cpp/builder.hpp>

// sequence_posix builder implementations
namespace sequence{
    class DelayBuilder: public SequenceBuilderBase {
    public:
        DelayBuilder(): SequenceBuilderBase("Delay", {{"duration", interfaces::ValueType::NUMBER}}) {}
        Sequence::ptr build(std::map<std::string, interfaces::Value> input, list builders) override {
            if(input["duration"].get_type() != interfaces::ValueType::NUMBER) {
                std::cerr << "duration not found" << std::endl;
                // return nullptr;
            }
            double duration = input["duration"].get<interfaces::ValueType::NUMBER>();
            return std::make_shared<Delay>(std::chrono::duration<double>(duration));
        }
    };

    class LogBuilder: public SequenceBuilderBase {
    public:
        LogBuilder(): SequenceBuilderBase("Log", {{"message", interfaces::ValueType::STRING}}) {}
        Sequence::ptr build(std::map<std::string, interfaces::Value> input, list builders) override {
            if(input["message"].get_type() != interfaces::ValueType::STRING) {
                std::cerr << "message not found" << std::endl;
                // return nullptr;
            }
            std::string message = input["message"].get<interfaces::ValueType::STRING>();
            return std::make_shared<Log>(message);
        }
    };

    class WhileTimeBuilder: public SequenceBuilderBase {
    public:
        WhileTimeBuilder(): SequenceBuilderBase("WhileTime", {{"duration", interfaces::ValueType::NUMBER}, {"sequence", interfaces::ValueType::SEQUENCE}}) {}
        Sequence::ptr build(std::map<std::string, interfaces::Value> input, list builders) override {
            if(input["duration"].get_type() != interfaces::ValueType::NUMBER) {
                std::cerr << "duration not found" << std::endl;
                // return nullptr;
            }
            double duration = input["duration"].get<interfaces::ValueType::NUMBER>();
            Sequence::ptr seq = input["sequence"].get<interfaces::ValueType::SEQUENCE>();
            return std::make_shared<WhileTime>(std::chrono::duration<double>(duration), seq);
        }
    };

    inline SequenceBuilderBase::list get_posix_builders() {
        return {
            std::make_shared<DelayBuilder>(),
            std::make_shared<LogBuilder>(),
            std::make_shared<WhileTimeBuilder>(),
        };
    }

}
