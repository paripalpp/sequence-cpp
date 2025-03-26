#include "sequence/builder.hpp"

namespace sequence{
    const Sequence::ptr SequenceBuilderBase::build(YAML::Node node, list builders){
        if(!node.IsMap()) {
            std::cerr << "node is not a map" << std::endl;
            return nullptr;
        }
        std::map<std::string, interfaces::Value> input;
        for (const auto& input_type : input_types_) {
            if (node[input_type.first]) {
                if(!node[input_type.first]) {
                    std::cerr << "input not found: " << input_type.first << std::endl;
                    continue;
                }
                switch (input_type.second) {
                    case interfaces::ValueType::NONE:
                    input[input_type.first] = interfaces::Value();
                    break;
                    case interfaces::ValueType::NUMBER:
                    input[input_type.first] = interfaces::Value(node[input_type.first].as<double>());
                    break;
                    case interfaces::ValueType::STRING:
                    input[input_type.first] = interfaces::Value(node[input_type.first].as<std::string>());
                    break;
                    case interfaces::ValueType::SEQUENCE:
                    input[input_type.first] = interfaces::Value(build_sequence(node[input_type.first], builders));
                    break;
                    case interfaces::ValueType::SEQUENCES:
                    input[input_type.first] = interfaces::Value(build_sequences(node[input_type.first], builders));
                    break;
                }
            }
        }
        return build(input, builders);
    }

    Sequence::ptr build_sequence(YAML::Node node, SequenceBuilderBase::list builders) {
        if(!node.IsMap()) {
            std::cerr << "entry is not a map" << std::endl;
            return nullptr;
        }
        if(node.size() != 1) {
            std::cerr << "entry size is not 1" << std::endl;
            return nullptr;
        }
        std::string name = node.begin()->first.as<std::string>();
        for(auto& builder: builders) {
            if(builder->name() == name) {
                auto value = node.begin()->second;
                if(!value.IsMap()) {
                    std::cerr << "value is not a map" << std::endl;
                    return nullptr;
                }
                return builder->build(value, builders);
                break;
            }
        }
        std::cerr << "sequence builder \"" << name <<"\" not found: " << name << std::endl;
        return nullptr;
    }
    Sequence::list build_sequences(YAML::Node node, SequenceBuilderBase::list builders) {
        if(!node.IsSequence()) {
            std::cerr << "entry is not a sequence" << std::endl;
            return {};
        }
        Sequence::list seqs;
        for(const auto& entry: node) {
            seqs.push_back(build_sequence(entry, builders));
        }
        return seqs;
    }

}