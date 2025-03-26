#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include <yaml-cpp/yaml.h>
#include <sequence-cpp/sequence.hpp>
#include <sequence-cpp/interfaces.hpp>

namespace sequence{

    class SequenceBuilderBase {
    public:
        using ptr = std::shared_ptr<SequenceBuilderBase>;
        using list = std::vector<ptr>;

        virtual Sequence::ptr build(std::map<std::string, interfaces::Value> input, list builders) = 0;

        const std::string& name() const { return name_; }
        const std::map<std::string, interfaces::ValueType>& inputs() const { return input_types_; }
        const Sequence::ptr build(YAML::Node node, list builders);
    protected:
        SequenceBuilderBase(const std::string& name, std::map<std::string, interfaces::ValueType> input_types = {}): name_(name), input_types_(input_types) {}
    private:
        std::string name_;
        std::map<std::string, interfaces::ValueType> input_types_;
    };

    Sequence::ptr build_sequence(YAML::Node node, SequenceBuilderBase::list builders);
    Sequence::list build_sequences(YAML::Node node, SequenceBuilderBase::list builders);

}

// standard sequence builders implementation
namespace sequence{

    class SeriesBuilder: public SequenceBuilderBase {
    public:
        SeriesBuilder(): SequenceBuilderBase("Series", {{"sequences", interfaces::ValueType::SEQUENCES}}) {}
        Sequence::ptr build(std::map<std::string, interfaces::Value> input, list builders) override {
            if(input["sequences"].get_type() != interfaces::ValueType::SEQUENCES) {
                std::cerr << "sequences not found" << std::endl;
                // return nullptr;
            }
            Sequence::list sequences = input["sequences"].get<interfaces::ValueType::SEQUENCES>();
            return std::make_shared<SequenceSeries>(sequences);
        }
    };

    class ParallelBuilder: public SequenceBuilderBase {
    public:
        ParallelBuilder(): SequenceBuilderBase("Parallel", {{"sequences", interfaces::ValueType::SEQUENCES}}) {}
        Sequence::ptr build(std::map<std::string, interfaces::Value> input, list builders) override {
            if(input["sequences"].get_type() != interfaces::ValueType::SEQUENCES) {
                std::cerr << "sequences not found" << std::endl;
                // return nullptr;
            }
            Sequence::list sequences = input["sequences"].get<interfaces::ValueType::SEQUENCES>();
            return std::make_shared<SequenceParallel>(sequences);
        }
    };

    class LoopBuilder: public SequenceBuilderBase {
    public:
        LoopBuilder(): SequenceBuilderBase("Loop", {{"sequence", interfaces::ValueType::SEQUENCE}}) {}
        Sequence::ptr build(std::map<std::string, interfaces::Value> input, list builders) override {
            if(input["sequence"].get_type() != interfaces::ValueType::SEQUENCE) {
                std::cerr << "sequence not found" << std::endl;
                // return nullptr;
            }
            Sequence::ptr sequence = input["sequence"].get<interfaces::ValueType::SEQUENCE>();
            return std::make_shared<Loop>(sequence);
        }
    };

    inline SequenceBuilderBase::list get_standard_builders() {
        return {
            std::make_shared<SeriesBuilder>(),
            std::make_shared<ParallelBuilder>(),
            std::make_shared<LoopBuilder>(),
        };
    }

}

// instant sequence builders
namespace sequence{
    /// @brief InstantBuilder is a builder that creates a sequence instantly
    class InstantBuilder: public SequenceBuilderBase {
        public:
        InstantBuilder( const std::string& name, std::function<Sequence::ptr()> func): SequenceBuilderBase(name) {
            func_ = func;
        }
        Sequence::ptr build(std::map<std::string, interfaces::Value> input, list builders) override {
            return func_();
        }
        private:
        std::function<Sequence::ptr()> func_;
    };
}
